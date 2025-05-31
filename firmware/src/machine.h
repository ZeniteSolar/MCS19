/**
 * @file machine.h
 *
 * @defgroup MACHINE State Machine Module
 *
 * @brief Implements the main state machine of the system.
 *
 */

#ifndef MACHINE_H
#define MACHINE_H 

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "conf.h"

#define MA_CAP_VOLTAGE      ma_adc0()
#define MA_BATTERY_VOLTAGE      ma_adc1()

// LED CONSTANTS
#define IDLE_LED_TIME           1 // half a period...time that the LED stays on
#define IDLE_LED_CLK_DIV        IDLE_LED_TIME * 2 * MACHINE_FREQUENCY //  
#define ATTENTION_LED_TIME      0.1 // half a period...time that the LED stays on
#define ATTENTION_CLK_DIV       ATTENTION_LED_TIME * 2 * MACHINE_FREQUENCY //  

// PRINT INFOS CONSTANTS
#define PRINT_INFOS_TIME        1 // seconds 
#define PRINT_INFOS_CLK_DIV     PRINT_INFOS_TIME * 2 * MACHINE_FREQUENCY

#ifdef ADC_ON
#include "adc.h"
#endif
#ifdef USART_ON
#include "usart.h"
#endif
#include "dbg_vrb.h"
#ifdef CAN_ON
#include "can.h"
#include "can_app.h"
extern const uint8_t can_filter[];
#endif

typedef enum state_machine{
    STATE_INITIALIZING,
    STATE_IDLE,
    STATE_RUNNING,
    STATE_ERROR,
    STATE_RESET,
    STATE_WAITING_RESET,
} state_machine_t;

typedef union system_flags{
    struct{
        uint8_t     boat_on        :1;
        uint8_t     boat_charging  :1; 
        uint8_t     boat_running :1;
    };
    uint8_t   all__;
} system_flags_t;

typedef union error_flags{
    struct{
        uint8_t     no_canbus     :1;
        uint8_t     no_charge     :1;
    };
    uint8_t   all;
}error_flags_t;

typedef struct measurements{
    uint16_t    cap_voltage;       // period maximum value of ADC1
    uint16_t    bat_voltage;       // period maximum value of ADC1
}measurements_t;


// machine checks
void check_buffers(void);
void read_and_check_adcs(void);

// debug functions
void print_configurations(void);
void print_system_flags(void);
void print_error_flags(void);
void print_infos(void);

// machine tasks
void task_initializing(void);
void task_idle(void);
void task_running(void);
void task_error(void);
void task_reset(void);
void task_waiting_reset(void);

// the machine itself
void set_machine_initial_state(void);
void machine_init(void);
void machine_run(void);
void set_state_error(void);
void set_state_initializing(void);
void set_state_idle(void);
void set_state_running(void);
void set_state_reset(void);
void set_state_waiting_reset(void);

//relay functions
void pulse_mainrelay_on(void);
void pulse_mainrelay_off(void);
void turn_boat_on(void);
void turn_boat_off(void);
void turn_charge_boat_on(void);
void turn_charge_boat_off(void);

// machine variables
extern volatile state_machine_t state_machine;
extern volatile system_flags_t system_flags;
extern volatile error_flags_t error_flags;
extern volatile measurements_t measurements;
extern volatile uint16_t charge_count_error;
extern volatile uint8_t relay_clk;
extern volatile uint8_t first_boat_off;
extern volatile uint8_t machine_clk;
extern volatile uint8_t machine_clk_divider;
extern volatile uint8_t total_errors;           // Contagem de ERROS
extern volatile uint8_t reset_clk;

// other variables
extern volatile uint8_t led_clk_div;
extern volatile uint16_t print_clk_div;

#endif /* ifndef MACHINE_H */
