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

// Equations for mode 2 (CTC with TOP OCR2A)
// Note the resolution. For example.. at 150hz, ICR1 = PWM_TOP = 159, so it
#define MACHINE_TIMER_TOP ((F_CPU/(2*MACHINE_TIMER_PRESCALER))/(MACHINE_TIMER_FREQUENCY) -1)

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
    uint16_t    adc0_avg;       // average value of ADC0
    uint16_t    adc0_avg_sum_count;
    uint64_t    adc0_avg_sum;   // average value of ADC0
    uint16_t    adc0_min;       // period minimum value of ADC0
    uint16_t    adc0_max;       // period maximum value of ADC0

    uint16_t    adc1_avg;       // average value of ADC1
    uint16_t    adc1_avg_sum_count;
    uint64_t    adc1_avg_sum;   // average value of ADC1
    uint16_t    adc1_min;       // period minimum value of ADC1
    uint16_t    adc1_max;       // period maximum value of ADC1
}measurements_t;


// machine checks
void check_buffers(void);
void reset_measurements(void);

// debug functions
void print_configurations(void);
void print_system_flags(void);
void print_error_flags(void);

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
volatile state_machine_t state_machine;
volatile system_flags_t system_flags;
volatile error_flags_t error_flags;
volatile measurements_t measurements;
volatile uint16_t charge_count_error;
volatile uint8_t relay_clk;
volatile uint8_t first_boat_off;
volatile uint8_t machine_clk;
volatile uint8_t machine_clk_divider;
volatile uint8_t total_errors;           // Contagem de ERROS
volatile uint16_t charge_count_error;
volatile uint8_t reset_clk;

// other variables
volatile uint8_t led_clk_div;

// ISRs
ISR(TIMER2_COMPA_vect);
ISR(PCINT2_vect);

#endif /* ifndef MACHINE_H */
