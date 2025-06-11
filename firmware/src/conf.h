/**
 * @file conf.h
 *
 * @defgroup CONF Configurations 
 *
 * @brief General configuration of the system.
 *
 */

#ifndef CONF_H
//#define CONF_H

#ifndef F_CPU
#define F_CPU 16000000UL    //define a frequencia do microcontrolador - 16MHz
#endif /* ifndef F_CPU */


// CONFIGURACOES DE COMPILACAO
#define DEBUG_ON
#define VERBOSE_ON
// #define VERBOSE_ON_CAN_APP
#define VERBOSE_ON_MACHINE
//#define VERBOSE_ON_ADC
#define VERBOSE_ON_INIT
#define VERBOSE_ON_ERROR
#define VERBOSE_ON_RELAY


// MODULES ACTIVATION
// #define RELAY_TEST_MODE
#define USART_ON
#define CAN_ON
#define CAN_DEPENDENT
#define ADC_ON
#define MACHINE_ON
#define LED_ON
#define WATCHDOG_ON
#define SLEEP_ON	
#define PRINT_INFOS


#define CAN_SIGNATURE_SELF      CAN_SIGNATURE_MCS19

#ifdef ADC_ON
// ADC CONFIGURATION
// note that changing ADC_FREQUENCY may cause problems with avg_sum_samples
// #define ADC_8BITS
#define ADC_FREQUENCY                       10000 // 20000
#define ADC_TIMER_PRESCALER                 64
#define ADC_TOP_CTC                         F_CPU/(ADC_TIMER_PRESCALER * 2UL * ADC_FREQUENCY) -1

#if ADC_TOP_CTC >= 256
    #error "Value for ADC timer top is greater than 8 bits"
#elif ADC_TOP_CTC < 2
    #error "Value for ADC timer top is too low, increase the prescaler"
#endif

// #define VSCALE_FACTOR                       10000L  // Scaling to maintain precision, result uint16 for can msg

/** @brief Circular buffer size definitions 
 * Using equal size for adc structures...
 * one could replicate the structure delacartion with different sizes
 * or even, % TODO use dynamic allocation for strucutres 
 */
#define ADC_AVG_SIZE_10                     64
#if (ADC_AVG_SIZE_10 == 0 || (ADC_AVG_SIZE_10 & (ADC_AVG_SIZE_10 - 1)) != 0)
    #error "ADC_AVG_SIZE_10 must be a power of 2!"
#endif
#define cbuf_adc0_SIZE                      ADC_AVG_SIZE_10  /**< Buffer size for ADC0 */
#define cbuf_adc0_SIZE_LOG2                 6; //log2_function(cbuf_adc0_SIZE)   /**< Log2 of buffer size */
#define cbuf_adc1_SIZE                      ADC_AVG_SIZE_10  /**< Buffer size for ADC1 */
#define cbuf_adc1_SIZE_LOG2                 6; //log2_function(cbuf_adc1_SIZE)    /**< Log2 of buffer size */

#define V_BAT 								1	//Battery adc channel 1
#define V_CAP 								0	//Capacitor adc channel 0


//#define FAKE_ADC_ON
#ifdef FAKE_ADC_ON
#define FAKE_ADC                            1
#endif // FAKE_ADC_ON


#endif //ADC_ON



#define CHARGING_TIME_CAPACITOR_ms 2500
#define CHARGING_TIME_CAPACITOR CHARGING_TIME_CAPACITOR_ms/100
#define CAPACITOR_CHARGE_PERCENTAGE 98	

//if isn't possible charge the sistem, wait user turn off the boat for TIME_TO_RESET_ms ms 
#define TIME_TO_RESET_ms 20000
#define TIME_TO_RESET TIME_TO_RESET_ms/100

#ifdef MACHINE_ON
// The machine frequency may not be superior of ADC_FREQUENCY/ADC_AVG_SIZE_10
#define MACHINE_FREQUENCY             100           //<! machine timer frequency in Hz
#define MACHINE_TIMER_PRESCALER             1024          //<! machine timer prescaler
// Equations for mode 2 (CTC with TOP OCR2A)
// Note the resolution. For example.. at 150hz, ICR1 = PWM_TOP = 159, so it
#define MACHINE_TIMER_TOP       (F_CPU/(MACHINE_TIMER_PRESCALER * 2UL * MACHINE_FREQUENCY)) -1
#if MACHINE_TIMER_TOP >= 256
    #error "Value for Machine timer top is greater than 8 bits"
#elif MACHINE_TIMER_TOP < 2
    #error "Value for Machine timer top is too low, increase the prescaler"
#endif


// SCALE TO CONVERT ADC DEFINITIONS
#define VSCALE                              (uint16_t)1000

#endif // MACHINE_ON

// INPUT PINS DEFINITIONS
#define     CHARGERELAY_PORT        PORTC
#define     CHARGERELAY_PIN         PINC
#define     CHARGERELAY_DDR         DDRC

#define     MAINRELAY_DDR           DDRC
#define     MAINRELAY_PORT          PORTC
#define     MAINRELAY_PIN           PINC

#define     MAINRELAY_OFF           PC3
#define     MAINRELAY_ON            PC4
#define     set_mainrelay_on()      set_bit(MAINRELAY_PORT, MAINRELAY_ON)
#define     clr_mainrelay_on()      clr_bit(MAINRELAY_PORT, MAINRELAY_ON)
#define     set_mainrelay_off()     set_bit(MAINRELAY_PORT, MAINRELAY_OFF)
#define     clr_mainrelay_off()     clr_bit(MAINRELAY_PORT, MAINRELAY_OFF)

#define     CHARGERELAY             PC2
#define     set_chargerelay()       set_bit(CHARGERELAY_PORT, CHARGERELAY)
#define     clr_chargerelay()       clr_bit(CHARGERELAY_PORT, CHARGERELAY)


#ifdef LED_ON
#define     LED_PORT                PORTD
#define     LED_PIN                 PIND
#define     LED_DDR                 DDRD
#define     LED1                    PD5
#define     LED2                    PD6
#define     cpl_led(y)              cpl_bit(LED_PORT, y)
#define     set_led(y)              set_bit(LED_PORT, y)
#define     clr_led(y)              clr_bit(LED_PORT, y)
#else
#define     cpl_led()               
#define     set_led()               
#define     clr_led()               
#endif // LED_ON


#ifdef CAN_ON
#define SPI_ON
#define CAN_APP_SEND_STATE_FREQ     10//36000     //<! state msg frequency in Hz
#define CAN_APP_SEND_ADC_FREQ       50//6000      //<! adc msg frequency in Hz
#define CAN_APP_SEND_RELAY_FREQ       50//6000      //<! adc msg frequency in Hz
#if MACHINE_FREQUENCY % CAN_APP_SEND_STATE_FREQ != 0
    #warning "CAN_APP_SEND_STATE_FREQ doesn't have a multiple equal to MACHINE_FREQUENCY, this frequency will be truncated"
#endif

#if MACHINE_FREQUENCY % CAN_APP_SEND_ADC_FREQ != 0
    #warning "CAN_APP_SEND_ADC_FREQ doesn't have a multiple equal to MACHINE_FREQUENCY, this frequency will be truncated"
#endif

#if MACHINE_FREQUENCY % CAN_APP_SEND_RELAY_FREQ != 0
    #warning "CAN_APP_SEND_RELAY_FREQ doesn't have a multiple equal to MACHINE_FREQUENCY, this frequency will be truncated"
#endif
// CANBUS DEFINITONS
// ----------------------------------------------------------------------------
/* Global settings for building the can-lib and application program.
 *
 * The following two #defines must be set identically for the can-lib and
 * your application program. They control the underlying CAN struct. If the
 * settings disagree, the underlying CAN struct will be broken, with
 * unpredictable results.
 * If can.h detects that any of the #defines is not defined, it will set them
 * to the default values shown here, so it is in your own interest to have a
 * consistent setting. Ommiting the #defines in both can-lib and application
 * program will apply the defaults in a consistent way too.
 *
 * Select if you want to use 29 bit identifiers.
 */
#define	SUPPORT_EXTENDED_CANID	0

/* Select if you want to use timestamps.
 * Timestamps are sourced from a register internal to the AT90CAN.
 * Selecting them on any other controller will have no effect, they will
 * be 0 all the time.
 */
#define	SUPPORT_TIMESTAMPS		0
#endif // CAN_ON

#endif /* ifndef CONF_H */
