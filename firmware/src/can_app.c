#include "can_app.h"

/**
 * @brief Prints a can message via usart
 */
inline void can_app_print_msg(can_t *msg)
{
#ifdef USART_ON
    usart_send_string("ID: ");
    usart_send_uint16(msg->id);
    usart_send_string(". D: ");

    for(uint8_t i = 0; i < msg->length; i++){
      usart_send_uint16(msg->data[i]);
      usart_send_char(' ');
    }

    usart_send_string(". ERR: ");
    can_error_register_t err = can_read_error_register();
    usart_send_uint16(err.rx);
    usart_send_char(' ');
    usart_send_uint16(err.tx);
    usart_send_char('\n');
#endif
}

/**
* @brief Manages the canbus application protocol
*/
inline void can_app_task(void)
{
    check_can();

    if(can_app_send_state_clk_div++ >= CAN_APP_SEND_STATE_CLK_DIV){
#ifdef USART_ON
        //VERBOSE_MSG_CAN_APP(usart_send_string("state msg was sent.\n"));
#endif
        can_app_send_state();
        can_app_send_state_clk_div = 0;
    }

    if(can_app_send_adc_clk_div++ >= CAN_APP_SEND_ADC_CLK_DIV){
#ifdef USART_ON
        //VERBOSE_MSG_CAN_APP(usart_send_string("adc msg was sent.\n"));
#endif
        can_app_send_bat();
        can_app_send_cap();
        can_app_send_adc_clk_div = 0;
    }
    if(can_app_send_relay_clk_div++ >= CAN_APP_SEND_RELAY_CLK_DIV){
        can_app_send_relay();
        can_app_send_relay_clk_div = 0;
#ifdef USART_ON
        VERBOSE_MSG_CAN_APP(usart_send_string("relay msg was sent.\n"));
#endif
    }

}

inline void can_app_send_state(void)
{
    can_t msg;
    msg.id                                  = CAN_FILTER_MSG_MCS19_STATE;
    msg.length                              = CAN_LENGTH_MSG_STATE;
    msg.flags.rtr = 0;

    msg.data[CAN_SIGNATURE_BYTE]            = CAN_SIGNATURE_SELF;
    msg.data[CAN_STATE_MSG_STATE_BYTE]      = (uint8_t) state_machine;
    msg.data[CAN_STATE_MSG_ERROR_BYTE]      = error_flags.all;

    can_send_message(&msg);
#ifdef VERBOSE_MSG_CAN_APP
    //VERBOSE_MSG_CAN_APP(usart_send_string("state msg was send.\n"));
//    VERBOSE_MSG_CAN_APP(can_app_print_msg(&msg));
#endif
}

inline void can_app_send_relay(void)
{
    can_t msg;
    msg.id                                  = CAN_FILTER_MSG_MCS19_RELAY;
    msg.length                              = CAN_LENGTH_MSG_MCS19_RELAY;
    msg.flags.rtr = 0;

    for(uint8_t i = msg.length; i; i--)     msg.data[i-1] = 0;

    msg.data[CAN_SIGNATURE_BYTE]                = CAN_SIGNATURE_SELF;
    if(system_flags.boat_charging)
        msg.data[CAN_MSG_MCS19_CHARGE_RELAY_BYTE] = 0xFF;
    else
        msg.data[CAN_MSG_MCS19_CHARGE_RELAY_BYTE] = 0x00;

    if(system_flags.boat_running)
        msg.data[CAN_MSG_MCS19_MAIN_RELAY_BYTE] = 0xFF; 
    else
        msg.data[CAN_MSG_MCS19_MAIN_RELAY_BYTE] = 0x00; 

    can_send_message(&msg); 
#ifdef VERBOSE_MSG_CAN_APP
    VERBOSE_MSG_CAN_APP(usart_send_string("relay state was sent.\n"));
    VERBOSE_MSG_CAN_APP(can_app_print_msg(&msg));
#endif

}


inline void can_app_send_bat(void)
{
    can_t msg;
    msg.id                                  = CAN_FILTER_MSG_MSC19_ADC;
    msg.length                              = CAN_LENGTH_MSG_MSC19_ADC;
    msg.flags.rtr = 0;
    
    uint16_t avg_adc0 = 
        measurements.adc0_avg_sum / measurements.adc0_avg_sum_count;


    msg.data[CAN_SIGNATURE_BYTE]            = CAN_SIGNATURE_SELF;
    msg.data[CAN_MSG_MCS19_BAT_AVG_BYTE_L] = LOW(avg_adc0);
    msg.data[CAN_MSG_MCS19_BAT_AVG_BYTE_H] = HIGH(avg_adc0);
    msg.data[CAN_MSG_MCS19_BAT_MIN_BYTE_L]  = LOW(measurements.adc0_min);
    msg.data[CAN_MSG_MCS19_BAT_MIN_BYTE_H]  = HIGH(measurements.adc0_min);
    msg.data[CAN_MSG_MCS19_BAT_MAX_BYTE_L]  = LOW(measurements.adc0_max);
    msg.data[CAN_MSG_MCS19_BAT_MAX_BYTE_H]  = HIGH(measurements.adc0_max);

    can_send_message(&msg); 
#ifdef VERBOSE_MSG_CAN_APP
    //VERBOSE_MSG_CAN_APP(usart_send_string("adc bat msg was sent.\n"));
//    VERBOSE_MSG_CAN_APP(can_app_print_msg(&msg));
#endif

    reset_measurements();
}


inline void can_app_send_cap(void)
{
    can_t msg;
    msg.id                                  = CAN_FILTER_MSG_MSC19_ADC;
    msg.length                              = CAN_LENGTH_MSG_MSC19_ADC;
    msg.flags.rtr = 0;
    
    uint16_t avg_adc1 = 
        measurements.adc1_avg_sum / measurements.adc1_avg_sum_count;

    msg.data[CAN_SIGNATURE_BYTE]            = CAN_SIGNATURE_SELF;
    msg.data[CAN_MSG_MCS19_CAP_AVG_BYTE_L] = LOW(avg_adc1);
    msg.data[CAN_MSG_MCS19_CAP_AVG_BYTE_H] = HIGH(avg_adc1);
    msg.data[CAN_MSG_MCS19_CAP_MIN_BYTE_L]  = LOW(measurements.adc1_min);
    msg.data[CAN_MSG_MCS19_CAP_MIN_BYTE_H]  = HIGH(measurements.adc1_min);
    msg.data[CAN_MSG_MCS19_CAP_MAX_BYTE_L]  = LOW(measurements.adc1_max);
    msg.data[CAN_MSG_MCS19_CAP_MAX_BYTE_H]  = HIGH(measurements.adc1_max);

    can_send_message(&msg); 
#ifdef VERBOSE_MSG_CAN_APP
    //VERBOSE_MSG_CAN_APP(usart_send_string("adc cap msg was sent.\n"));
//    VERBOSE_MSG_CAN_APP(can_app_print_msg(&msg));
#endif

    reset_measurements();
}


inline void can_app_extractor_mic17_mcs(can_t *msg)
{
    if(msg->data[CAN_SIGNATURE_BYTE] == CAN_SIGNATURE_MIC17){
        
        // can_app_checks_without_mic17_msg = 0;

        if(msg->data[CAN_MSG_MIC17_MCS_BOAT_ON_BYTE] == 0xFF){
            system_flags.boat_on = 1;
        }else if(msg->data[CAN_MSG_MIC17_MCS_BOAT_ON_BYTE] == 0x00){
            system_flags.boat_on = 0;
        }

        /*system_flags.boat_on       = bit_is_set(msg->data[
            CAN_MSG_MIC17_MCS_BOAT_ON_BYTE], 
            CAN_MSG_MIC17_MCS_BOAT_ON_BIT);*/


        VERBOSE_MSG_CAN_APP(usart_send_string("boat on bit: "));
        VERBOSE_MSG_CAN_APP(usart_send_uint16(system_flags.boat_on));
        VERBOSE_MSG_CAN_APP(usart_send_char('\n'));


    }
}


/**
 * @brief extracts the specific MIC17 STATE message
 * @param *msg pointer to the message to be extracted
 */
inline void can_app_extractor_mic17_state(can_t *msg)
{
    // TODO:
    //  - se tiver em erro, desligar acionamento
    if(msg->data[CAN_SIGNATURE_BYTE] == CAN_SIGNATURE_MIC17){
        // zerar contador
        if(msg->data[CAN_STATE_MSG_ERROR_BYTE]){
            //ERROR!!!
        }
        /*if(contador == maximo)*/{
            //ERROR!!!
        }
    }
}
 
/**
 * @brief redirects a specific message extractor to a given message
 * @param *msg pointer to the message to be extracted
 */
inline void can_app_msg_extractors_switch(can_t *msg)
{
    if(msg->data[CAN_SIGNATURE_BYTE] == CAN_SIGNATURE_MIC17){
        switch(msg->id){
            case CAN_FILTER_MSG_MIC17_MCS:
                //VERBOSE_MSG_CAN_APP(usart_send_string("got a boat_on msg: "));
                //VERBOSE_MSG_CAN_APP(can_app_print_msg(msg));
                can_app_extractor_mic17_mcs(msg);

            case CAN_FILTER_MSG_MIC17_STATE:
#ifdef USART_ON
                //VERBOSE_MSG_CAN_APP(usart_send_string("got a state msg: "));
#endif
                //VERBOSE_MSG_CAN_APP(can_app_print_msg(msg));
                can_app_extractor_mic17_state(msg);
                break;
            default:
#ifdef USART_ON
                //VERBOSE_MSG_CAN_APP(usart_send_string("got a unknown msg:\n "));
                               

#endif
                //VERBOSE_MSG_CAN_APP(can_app_print_msg(msg));
                break;
        }    
    }
}

/**
 * @brief Manages to receive and extract specific messages from canbus
 */
inline void check_can(void)
{
    // If no messages is received from mic17 for
    // CAN_APP_CHECKS_WITHOUT_MIC17_MSG cycles, than it go to a specific error state. 
    //VERBOSE_MSG_CAN_APP(usart_send_string("checks: "));
    //VERBOSE_MSG_CAN_APP(usart_send_uint16(can_app_checks_without_mic17_msg));
// #ifdef CAN_DEPENDENT
//     if(can_app_checks_without_mic17_msg++ >= CAN_APP_CHECKS_WITHOUT_MIC17_MSG){
// #ifdef USART_ON
//         VERBOSE_MSG_CAN_APP(usart_send_string("Error: too many cycles withtou message.\n"));
// #endif
//         can_app_checks_without_mic17_msg = 0;
//         error_flags.no_canbus = 1;
//         set_state_error();
//     }
// #endif
    
    if(can_check_message()){
        can_t msg;
        if(can_get_message(&msg)){
            can_app_msg_extractors_switch(&msg);
        }
    }
}

