/**
 * @file can_app.h
 *
 * @defgroup CANAPP    Application Module for canbus
 *
 * @brief The application layer module for canbus
 *
 */

#ifndef CAN_APP_H
#define CAN_APP_H 

#include "conf.h"
#include "dbg_vrb.h"
#include "../lib/bit_utils.h"
#include "can.h"
#include "can_ids.h"
#include "machine.h"
#include "usart.h"

void can_app_print_msg(can_t *msg);
void can_app_task(void);
void can_app_send_state(void);
void can_app_send_bat(void);
void can_app_send_cap(void);
void can_app_send_relay(void);
void can_app_msg_extractors_switch(can_t *msg);
void can_app_extractor_mic17_state(can_t *msg);
void can_app_extractor_mic17_mcs(can_t *msg);
void check_can(void);

#ifdef CAN_ON
#define CAN_APP_SEND_STATE_CLK_DIV  (MACHINE_TIMER_FREQUENCY/CAN_APP_SEND_STATE_FREQ)
#define CAN_APP_SEND_ADC_CLK_DIV    (MACHINE_TIMER_FREQUENCY/CAN_APP_SEND_ADC_FREQ)
#define CAN_APP_SEND_RELAY_CLK_DIV    (MACHINE_TIMER_FREQUENCY/CAN_APP_SEND_RELAY_FREQ)
#else
#define CAN_APP_SEND_STATE_CLK_DIV  1
#define CAN_APP_SEND_ADC_CLK_DIV    1
#endif

extern uint32_t can_app_send_state_clk_div;
extern uint32_t can_app_send_adc_clk_div;
extern uint32_t can_app_send_relay_clk_div;

#endif /* ifndef CAN_APP_H */
