#include "machine.h"

volatile state_machine_t state_machine;
volatile system_flags_t system_flags;
volatile error_flags_t error_flags;
volatile measurements_t measurements;
volatile uint16_t charge_count_error;
volatile uint8_t relay_clk;
volatile uint8_t first_boat_off;
volatile uint8_t machine_clk;
volatile uint8_t machine_clk_divider;
volatile uint8_t total_errors; // Contagem de ERROS
volatile uint8_t reset_clk;
volatile uint8_t led_clk_div;
volatile uint16_t print_clk_div;

/**
 * @brief
 */
void machine_init(void) {
  clr_bit(PRR, PRTIM2); // Activates clock

  // MODE 2 -> CTC with TOP on OCR1
  TCCR2A = (1 << WGM21) | (0 << WGM20)      // mode 2
           | (0 << COM2B1) | (0 << COM2B0)  // do nothing
           | (0 << COM2A1) | (0 << COM2A0); // do nothing

  TCCR2B =
#if MACHINE_TIMER_PRESCALER == 1
      (0 << CS22) | (0 << CS21) | (1 << CS20) // Prescaler N=1
#elif MACHINE_TIMER_PRESCALER == 8
      (0 << CS22) | (1 << CS21) | (0 << CS20) // Prescaler N=8
#elif MACHINE_TIMER_PRESCALER == 32
      (0 << CS22) | (1 << CS21) | (1 << CS20) // Prescaler N=32
#elif MACHINE_TIMER_PRESCALER == 64
      (1 << CS22) | (0 << CS21) | (0 << CS20) // Prescaler N=64
#elif MACHINE_TIMER_PRESCALER == 128
      (1 << CS22) | (0 << CS21) | (1 << CS20) // Prescaler N=128
#elif MACHINE_TIMER_PRESCALER == 256
      (1 << CS22) | (1 << CS21) | (0 << CS20) // Prescaler N=256
#elif MACHINE_TIMER_PRESCALER == 1024
      (1 << CS22) | (1 << CS21) | (1 << CS20) // Prescaler N=1024
#else
      0
#endif
      | (0 << WGM22); // mode 2

  OCR2A = MACHINE_TIMER_TOP; // OCR2A = TOP = fcpu/(N*2*f) -1

  TIMSK2 |= (1 << OCIE2A); // Activates interruption

  set_machine_initial_state();
  set_state_initializing();
}

/**
 * @brief set machine initial state
 */
inline void set_machine_initial_state(void) {
  error_flags.all = 0;
  relay_clk = machine_clk = machine_clk_divider = led_clk_div = 0;
}

/**
 * @brief set error state
 */
inline void set_state_error(void) {
  VERBOSE_MSG_MACHINE(usart_send_string("\n>>>STATE ERROR\n"));
  state_machine = STATE_ERROR;
}

/**
 * @brief set initializing state
 */
inline void set_state_initializing(void) {
  VERBOSE_MSG_MACHINE(usart_send_string("\n>>>INITIALIZING STATE\n"));
  state_machine = STATE_INITIALIZING;
}

/**
 * @brief set idle state
 */
inline void set_state_idle(void) {
  VERBOSE_MSG_MACHINE(usart_send_string("\n>>>IDLE STATE\n"));
  state_machine = STATE_IDLE;
  relay_clk = 0;
  charge_count_error = 0;
}

/**
 * @brief set running state
 */
inline void set_state_running(void) {
  VERBOSE_MSG_MACHINE(usart_send_string("\n>>>RUNNING STATE\n"));
  state_machine = STATE_RUNNING;
  relay_clk = 0;
}

/**
 * @brief set reset state
 */
inline void set_state_reset(void) {
  VERBOSE_MSG_MACHINE(usart_send_string("\n>>>RESET STATE\n"));
  state_machine = STATE_RESET;
  error_flags.no_charge = 0;
}

inline void set_state_waiting_reset(void) {
  VERBOSE_MSG_MACHINE(usart_send_string("\n>>>WAITING RESET STATE\n"));
  state_machine = STATE_WAITING_RESET;
}

/**
 * @breif prints the configurations and definitions
 */
inline void print_configurations(void) {
  VERBOSE_MSG_MACHINE(usart_send_string("CONFIGURATIONS:\n"));

  VERBOSE_MSG_MACHINE(usart_send_string("\nadc_f: "));
  VERBOSE_MSG_MACHINE(usart_send_uint16(ADC_FREQUENCY));
  VERBOSE_MSG_MACHINE(usart_send_char(','));
  VERBOSE_MSG_MACHINE(usart_send_uint16(ADC_AVG_SIZE_10));
  VERBOSE_MSG_MACHINE(usart_send_string("\nmachine_f: "));
  VERBOSE_MSG_MACHINE(usart_send_uint16(MACHINE_FREQUENCY));

  VERBOSE_MSG_MACHINE(usart_send_char('\n'));
}

/**
 * @brief prints the system flags
 */
inline void print_system_flags(void) {
  usart_send_string("\tB_on: ");
  usart_send_uint16(system_flags.boat_on);
  usart_send_string(" B_charg: ");
  usart_send_uint16(system_flags.boat_charging);
  usart_send_string(" B_running ");
  usart_send_uint16(system_flags.boat_running);
}

/**
 * @brief prints the error flags
 */

inline void print_error_flags(void) {
  VERBOSE_MSG_MACHINE(usart_send_string("\nError no canbus: "));
  VERBOSE_MSG_MACHINE(usart_send_uint16(error_flags.no_canbus));
  VERBOSE_MSG_MACHINE(usart_send_string(" Error no charge: "));
  VERBOSE_MSG_MACHINE(usart_send_uint16(error_flags.no_charge));
}

/**
 * @brief Checks if the system is OK to run
 */

void pulse_mainrelay_on(void) {
  clr_mainrelay_off(); // having shure to not set both coils at the same time
  _delay_ms(10);

  // apply a pulse of 250ms
  set_mainrelay_on();
  _delay_ms(100);
  clr_mainrelay_on();
}

/**
 * @brief turns off the mainrealy (a laching relay) by using a pulse of 250ms
 */
void pulse_mainrelay_off(void) {
  clr_chargerelay();

  clr_mainrelay_on(); // having shure to not set both coils at the same time
  _delay_ms(10);

  // apply a pulse of 250ms
  set_mainrelay_off();
  _delay_ms(100);
  clr_mainrelay_off();
}

/*
 * @
 * @breif turns the boat on, slowly charging the caps before activate the
 * main relay
 */

void turn_boat_on(void) {
#ifdef VERBOSE_ON_RELAY
  VERBOSE_MSG_MACHINE(usart_send_string("Turning boat on"));
#endif
  pulse_mainrelay_on();
  _delay_ms(200);
  clr_chargerelay();
}

/*
b2rief turns the boat off.
 */
void turn_boat_off(void) {
#ifdef VERBOSE_ON_RELAY
  VERBOSE_MSG_MACHINE(usart_send_string("Turning boat off"));
#endif

  clr_chargerelay();
  _delay_ms(50);
  pulse_mainrelay_off();
  _delay_ms(50);
  pulse_mainrelay_off();
  _delay_ms(50);
  pulse_mainrelay_off();
  _delay_ms(50);
}
void turn_charge_boat_on(void) {
  set_chargerelay();
  system_flags.boat_charging = 1;
}

void turn_charge_boat_off(void) {
  clr_chargerelay();
  system_flags.boat_charging = 0;
}

/**
 * @brief read and checks current levels
 */
inline void read_and_check_adcs(void) {
#ifdef ADC_ON
  measurements.cap_voltage =
      MA_CAP_VOLTAGE; //* CONVERSION_BATTERY_VOLTAGE_VALUE;
  measurements.bat_voltage =
      MA_BATTERY_VOLTAGE; //* CONVERSION_BATTERY_VOLTAGE_VALUE;

  switch (state_machine) {
  case STATE_INITIALIZING:
    // check_battery_voltage();

    break;
  case STATE_IDLE:
    // check_battery_voltage();

    break;
  case STATE_RUNNING:
    // check_battery_voltage();

    break;
  default:
    break;
  }
#endif
}

inline void task_initializing(void) {
#ifdef LED_ON
  set_led(LED1);
  set_led(LED2);
#endif

#ifdef RELAY_TEST_MODE
  VERBOSE_MSG_INIT(
      usart_send_string("System initializing in RELAY_TEST_MODE.\n"));
#endif

  system_flags.boat_on = 0;
  turn_boat_off();

  set_machine_initial_state();

  VERBOSE_MSG_INIT(usart_send_string("System initialized without errors.\n"));
  set_state_idle();
}

/**
 * @brief waits for commands while checking the system
 */
inline void task_idle(void) {

// This module in particular has delays on idle state
// So there is no idle_led_clk_div
#ifdef LED_ON
  //   if (led_clk_div++ >= 1) {
  cpl_led(LED1);
  set_led(LED2);
  // led_clk_div = 0;
//   }
#endif

#ifdef RELAY_TEST_MODE
  VERBOSE_MSG_MACHINE(
      usart_send_string("\nTEST MODE: IDLE STATE ===> BOAT ON!!!"));
  turn_boat_on();
  _delay_ms(1000);
  set_state_running();
#else // RELAY_TEST_MODE

  system_flags.boat_running = 0;

  if (system_flags.boat_on) {

    turn_charge_boat_on();

    // secure time and mesure cap voltage
    if(relay_clk++ <= CHARGING_TIME_CAPACITOR/* || ((100 * adc.channel[V_CAP].avg) <= (CAPACITOR_CHARGE_PERCENTAGE * adc.channel[V_BAT].avg))*/){
      _delay_ms(100);
      /*
                  if(++charge_count_error > 2 * CHARGING_TIME_CAPACITOR){
                      error_flags.no_charge = 1;
                      turn_charge_boat_off();
                      set_state_error();
                  }
      */
    } else {
      VERBOSE_MSG_MACHINE(usart_send_string("\nIDLE STATE ===> BOAT ON!!!"));
      turn_boat_on();
      _delay_ms(50);
      relay_clk = 0;
      turn_charge_boat_off();
      set_state_running();
    }
  } else if (system_flags.boat_charging) {
    VERBOSE_MSG_MACHINE(usart_send_string("\nBOAT OFF BEFORE CHARGING"));
    relay_clk = 0;
    turn_charge_boat_off();
    set_state_running();
  }

#endif // RELAY_TEST_MODE
}

/**
 * @brief running task checks the system and apply the control action to pwm.
 */
inline void task_running(void) {

#ifdef LED_ON
  if (led_clk_div++ >= ATTENTION_CLK_DIV) {
    cpl_led(LED1);
    clr_led(LED2);
    led_clk_div = 0;
  }
#endif // LED_ON

  system_flags.boat_running = 1;

#ifdef RELAY_TEST_MODE
  VERBOSE_MSG_MACHINE(
      usart_send_string("\nIDLE STATE !RELAY_TEST_MODE! ===> BOAT OFF!!!"));
  turn_boat_off();
  _delay_ms(1000);
  set_state_idle();

#else  // RELAY_TEST_MODE
  if (!system_flags.boat_on) {
    VERBOSE_MSG_MACHINE(usart_send_string("\nIDLE STATE ===> BOAT OFF!!!"));
    turn_boat_off();
    set_state_idle();
  }
#endif // RELAY_TEST_MODE
}

/**
 * @brief error task checks the system and tries to medicine it.
 */
inline void task_error(void) {
#ifdef LED_ON
  if (led_clk_div++ >= ATTENTION_CLK_DIV) {
    cpl_led(LED2);
    clr_led(LED1);
    led_clk_div = 0;
  }
#endif
  set_state_initializing();

  total_errors++; // incrementa a contagem de erros
  VERBOSE_MSG_ERROR(usart_send_string("The error code is: "));
  VERBOSE_MSG_ERROR(usart_send_uint16(error_flags.all));
  VERBOSE_MSG_ERROR(usart_send_char('\n'));

  if (error_flags.no_canbus)
    VERBOSE_MSG_ERROR(
        usart_send_string("\t - No canbus communication with MIC19!\n"));
  if (!error_flags.all)
    VERBOSE_MSG_ERROR(
        usart_send_string("\t - Oh no, it was some unknown error.\n"));

  VERBOSE_MSG_ERROR(usart_send_string("The error level is: "));
  VERBOSE_MSG_ERROR(usart_send_uint16(total_errors));
  VERBOSE_MSG_ERROR(usart_send_char('\n'));

  if (total_errors < 2) {
    VERBOSE_MSG_ERROR(usart_send_string("I will reset the machine state.\n"));
  }
  if (total_errors >= 20) {
    VERBOSE_MSG_ERROR(
        usart_send_string("The watchdog will reset the whole system.\n"));
    set_state_reset();
  }

  if (error_flags.no_charge) {
    VERBOSE_MSG_ERROR(
        usart_send_string("\t - Capacitors undervolage after charge!\n"));
    set_state_waiting_reset();
  }

#ifdef LED_ON
  cpl_led(LED2);
#endif
}

/**
 * @brief wait the user turn off the boat for a defined time
 */
inline void task_waiting_reset(void) {
  _delay_ms(100);
  if (!system_flags.boat_on) {
    if (reset_clk++ < TIME_TO_RESET) {
      _delay_ms(500);
      VERBOSE_MSG_ERROR(usart_send_uint16(reset_clk));
      VERBOSE_MSG_ERROR(usart_send_char('\n'));
    } else
      set_state_reset();
  } else {
    VERBOSE_MSG_ERROR(
        usart_send_string("I WILL RESET WHEN THE USER TURN OFF THE BOAT AND I "
                          "WIL WAIT FOR STABILIZE MYSELF!\n"));
  }
}

/**
 * @brief reset error task just freezes the processor and waits for watchdog
 */
inline void task_reset(void) {
#ifndef WATCHDOG_ON
  // wdt_init();
#endif

  cli(); // disable interrupts

  VERBOSE_MSG_ERROR(usart_send_string("WAITING FOR A RESET!\n"));
  for (;;)
    ;
}

void print_infos(void) {
  static uint8_t i = 0;

  if (print_clk_div++ >= PRINT_INFOS_CLK_DIV) {
    print_clk_div = 0;
    usart_send_string("\nV_CAP ");
    usart_send_uint16(measurements.cap_voltage);
    usart_send_string("\tV_BAT ");
    usart_send_uint16(measurements.bat_voltage);
    print_system_flags();
    switch (i++) {
    case 0:
      break;
    case 1:
      // print_error_flags();
      __attribute__((fallthrough));
    default:
      // VERBOSE_MSG_MACHINE(usart_send_char('\n'));
      i = 0;
      break;
    }
  }
}

/**
 * @brief this is the machine state itself.
 */
inline void machine_run(void) {

  if (machine_clk) {
    machine_clk = 0;
#ifdef ADC_ON
    if (adc_data_ready) {
      adc_data_ready = 0;
      read_and_check_adcs();
    }

    if (error_flags.all) {
      print_system_flags();
      print_error_flags();
      set_state_error();
      if (error_flags.no_charge)
        set_state_waiting_reset();
    }

    switch (state_machine) {
    case STATE_INITIALIZING:
      task_initializing();

      break;
    case STATE_IDLE:
      task_idle();
#ifdef PRINT_INFOS
      print_infos();
#endif /* PRINT_INFOS */
#ifdef CAN_ON
      can_app_task();
#endif /* CAN_ON */
      break;
    case STATE_RUNNING:
      task_running();
#ifdef PRINT_INFOS
      print_infos();
#endif /* PRINT_INFOS */
#ifdef CAN_ON
      can_app_task();
#endif /* CAN_ON */

      break;
    case STATE_ERROR:
      task_error();
      __attribute__((fallthrough));
    case STATE_WAITING_RESET:
      task_waiting_reset();
#ifdef CAN_ON
      can_app_task();
#endif /* CAN_ON */

      break;

    case STATE_RESET:
      __attribute__((fallthrough));
    default:
      task_reset();
      break;
    }
  }
#endif /* ADC_ON */
}

/**
 * @brief ISR para ações de controle
 */
ISR(TIMER2_COMPA_vect) { machine_clk = 1; }
