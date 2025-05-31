
#include "adc.h"

volatile uint8_t adc_data_ready;
#ifdef ADC_8BITS
volatile uint8_t avg_adc0;
volatile uint8_t avg_adc1;
#else
volatile uint16_t avg_adc0;
volatile uint16_t avg_adc1;
#endif

volatile adc_cbuf_adc0_t cbuf_adc0;
volatile adc_cbuf_adc1_t cbuf_adc1;

volatile uint16_t adc_debug_clk_div = 0; 

// Coefficients for linearization, example coefficients
static const int16_t adc0_a = 1;
static const int16_t adc0_b = 1;
static const int16_t adc1_a = 0;   
static const int16_t adc1_b = 0;

// Define the linearization polynomial as a macro or function

/**
 * @brief initializes all adc circular buffers.
 */
void init_buffers(void)
{
    CBUF_Init(cbuf_adc0);
    CBUF_Init(cbuf_adc1);
}

/**
 * @brief Linearize adcs 
 */
static inline uint16_t linearize_adc(uint16_t x, int16_t a, int16_t b) {
    uint16_t result;

    result = a * x + b;

    return result;
}

/**
 * @brief computes the average of a given adc channel
 *
 * Ma = (1/N)*Summation of x[i] from i=0 to N, 
 * if N = 2^k, then Ma = (Summation of x[i] from i=0 to N) >> k
 *
 */
#ifdef ADC_8BITS
uint8_t ma_adc0(void)
#else
uint16_t ma_adc0(void)
#endif
{
    uint32_t sum = 0;
    for (uint8_t i = cbuf_adc0_SIZE; i; i--) {
        uint16_t raw_value = CBUF_Get(cbuf_adc0, i);
        uint16_t linearized_value = linearize_adc(raw_value, adc0_a, adc0_b);  // Apply polynomial

        // if linearized_value > max_adc0_value {
        //     max_adc0_value = linearized_value;
        // }
        // if linearized_value < min_adc0_value {
        //     min_adc0_value = linearized_value;
        // }
        sum += linearized_value;
    }
    // if(adc_debug_clk_div++ >= ADC_DEBUG_CLK_DIV/50){
        // usart_send_string("\ntestando SUM: ");
        // usart_send_uint16(sum);
        // adc_debug_clk_div = 0;
    // } 
    avg_adc0 = sum >> cbuf_adc0_SIZE_LOG2;
    return avg_adc0;
}

/**
* @brief computes the average of a given adc channel
*
* Ma = (1/N)*Summation of x[i] from i=0 to N, 
* if N = 2^k, then Ma = (Summation of x[i] from i=0 to N) >> k
*
*/
#ifdef ADC_8BITS
uint8_t ma_adc1(void)
#else
uint16_t ma_adc1(void)
#endif
{
    uint32_t sum = 0;
    for (uint8_t i = cbuf_adc1_SIZE; i; i--) {
        uint16_t raw_value = CBUF_Get(cbuf_adc1, i);
        uint16_t linearized_value = linearize_adc(raw_value, adc1_a, adc1_b);  // Apply polynomial
        sum += linearized_value;
    }
    avg_adc1 = sum >> cbuf_adc1_SIZE_LOG2;
    return avg_adc1;
}

/**
 * @brief Changes ADC channel
 * @param __ch is the channel to be switched to
 * @return return the selected channel
 */
uint8_t adc_select_channel(adc_channels_t __ch)
{
    if(__ch < ADC_LAST_CHANNEL ) ADC_CHANNEL = __ch;

    ADMUX = (ADMUX & 0xF8) | ADC_CHANNEL; // clears the bottom 3 bits before ORing
    return ADC_CHANNEL;
}

/**
 * @brief inicializa o ADC, configurado para conversão engatilhada com o timer0.
 */
void adc_init(void)
{
    adc_data_ready = 0;
    clr_bit(PRR, PRADC);                           // Activates clock to adc

    // configuracao do ADC
    PORTC   =   0b00000000;                         // disables pull-up for adcs pins
    DDRC    =   0b00000000;                         // all adcs as inputs
    DIDR0   =   0b11111111;                         // ADC0 to ADC2 as adc (digital disable)

    ADMUX   =   (0 << REFS1)                        // AVcc with external capacitor at AREF pin
            | (1 << REFS0)
#ifdef ADC_8BITS
            | (1 << ADLAR);                         // ADC left adjusted -> using 8bits ADCH only
#else
            | (0 << ADLAR);                         // ADC left adjusted -> using all 10 bits
#endif

    ADCSRB  =   (0 << ADTS2)                        // Auto-trigger source: timer0 Compare Match A
            | (1 << ADTS1)
            | (1 << ADTS0);

    init_buffers();
    // adc_select_channel(ADC0);                       // Choose admux
    ADMUX = (ADMUX & 0xF8) | 0;                       // Atribuindo canal
    
    ADCSRA  =   (1 << ADATE)    // ADC Auto Trigger Enable
          | (1 << ADIE)     // ADC Interrupt Enable
          | (1 << ADEN)     // ADC Enable
          | (1 << ADSC)     // Do the first Start of Conversion 
#if ADC_TIMER_PRESCALER == 1
          | (0 << ADPS2) | (0 << ADPS1) | (1 << ADPS0)  // Prescaler N=1
#elif ADC_TIMER_PRESCALER == 8
          | (0 << ADPS2) | (1 << ADPS1) | (0 << ADPS0)  // Prescaler N=8
#elif ADC_TIMER_PRESCALER == 64
          | (0 << ADPS2) | (1 << ADPS1) | (1 << ADPS0)  // Prescaler N=64
#elif ADC_TIMER_PRESCALER == 256
          | (1 << ADPS2) | (0 << ADPS1) | (0 << ADPS0)  // Prescaler N=1256
#elif ADC_TIMER_PRESCALER == 1024
          | (1 << ADPS2) | (0 << ADPS1) | (1 << ADPS0)  // Prescaler N=1024
#else
          | (1 << ADPS1) | (1 << ADPS0)  // Default to N=8 if no valid prescaler is set
#endif
;


    // configuracao do Timer TC0 --> TIMER DO ADC
    clr_bit(PRR, PRTIM0);                          // Activates clock to timer0
    //set_bit(DDRD, PD5);
    //set_bit(DDRD, PD6);
    TCCR0A  =   (1 << WGM01) | (0 << WGM00)         // Timer 0 in Mode 2 = CTC (clear on compare)
            | (0 << COM0B1) | (0 << COM0B0)         // do nothing with OC0B
            | (0 << COM0A1) | (0 << COM0A0);        // Normal port operation
    TCCR0B  =   (0 << WGM02)                        // Timer 0 in Mode 2 = CTC (clear on compare)
            | (0 << FOC0A) | (0 << FOC0B)           // dont force outputs
            | (0 << CS02) | (1 << CS01) | (0 << CS00); // clock enabled, prescaller = 8

	OCR0A  =    ADC_TOP_CTC;
    TIMSK0 |=   (1 << OCIE0A);                      // Ativa a interrupcao na igualdade de comparação do TC0 com OCR0A

}

/**
 * @brief MUX do ADC
 */
ISR(ADC_vect){
    switch(ADC_CHANNEL){
        case ADC0: 
#ifdef ADC_8BITS
            CBUF_Push(cbuf_adc0, ADCH); 
#else
            CBUF_Push(cbuf_adc0, ADC); 
#endif
            ADC_CHANNEL++;
            break;
        case ADC1:
            // VERBOSE_MSG_ADC(usart_send_string(" \tadc1: "));
#ifdef ADC_8BITS2
            CBUF_Push(cbuf_adc1, ADCH); 
#else
            CBUF_Push(cbuf_adc1, ADC); 
#endif
            // Last channel logic 
            adc_data_ready = 1; // Moving this into default might cause a false positive flag
            __attribute__((fallthrough)); // Explicitly calling shared code instead of falling through
        default:
            ADC_CHANNEL = ADC0; // reset to first channel
            break;
    }
    // adc_select_channel(ADC_CHANNEL);
    ADMUX = (ADMUX & 0xF8) | ADC_CHANNEL;


    // if(adc_debug_clk_div++ >= ADC_DEBUG_CLK_DIV){
        // #ifdef ADC_8BITS
            // VERBOSE_MSG_ADC(usart_send_uint8(ADCH));
            // VERBOSE_MSG_ADC(usart_send_char('\n'));
        // #else
            // VERBOSE_MSG_ADC(usart_send_uint16(ADC));
            // VERBOSE_MSG_ADC(usart_send_char('\n'));
        // #endif
        // adc_debug_clk_div = 0;
    // }  
}
 
/**
 * @brief ISR necessária para auto-trigger do ADC. Caso contrário, dispara
 * BADISR_vect.
 */
EMPTY_INTERRUPT(TIMER0_COMPA_vect);

