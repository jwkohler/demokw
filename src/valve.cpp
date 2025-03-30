#include "pico/stdlib.h"
#include "stdio.h"
#include "hardware/irq.h"
#include "hardware/timer.h"
#include "valve.h"

//
// single global recipe
DotRecipe recipe;
//
// single global valve controller
Valve valve;

//
// static instance pointer - set via constructor
Valve *Valve::_instance = nullptr;
//
// Valve ctor
Valve::Valve()
    : _state(IDLE),
      _is_initialized(false),
      _force_run(false)
{
    if (_instance)
        panic("ERROR: Valve instance already exists");
    _instance = this;
}
//
// run the valve control logix
// this function is invoked from the timer ISR, so it executes
// in an ISR context.
// Implements a simple FSM (OPEN/CLOSE/IDLE) which transitions
// based on comparing a counter to the current recipe open/close
// "times".
// __time_critical_func wrapper places this code in RAM instead
// of flash.  execution is much faster...
void __time_critical_func(Valve::run)()
{
    //
    // track count
    // state transitions should zero this out _at_ transition time
    _state_count += 1;

#ifdef DEBUG_CLK_ENABLED
    // toggle debug clock
    gpio_xor_mask(1 << IO_DEBUG_CLK);
#endif

    switch (_state)
    {
        //
        // IDLE state
        // do nothing but monitor the RUN input and
        // the 'force run' flag
        // when either goes et, transition to OPEN
    case IDLE:
        if (should_run())
        {
            set_valve_open();
            set_busy();
            _state_count = 0;
            _state = OPEN;
        }
        else
            clear_busy();
        break;
        //
        // OPEN state
        // count 'ticks' until the recpie open time has expired
        // then transition to CLOSE
    case OPEN:
        if (_state_count >= _recipe->_open_time_normal)
        {
            set_valve_close();
            _state_count = 0;
            _state = CLOSE;
        }
        break;
        //
        // CLOSE state
        // count 'ticks' until the recipe close time has expired
        // then transition to IDLE
    case CLOSE:
        if (_state_count >= _recipe->_close_time_normal)
        {
            _state_count = 0;
            if (should_run())
            {
                set_valve_open();
                _state = OPEN;
            }
            else
                _state = IDLE;
        }
        break;
        //
        // ERROR state
        // TBD
    case ERROR:
        break;
    }
}
//
// initialize Valve controller
// init i/o
// init timer irq
// publish pointer to the sigleton Valve
void Valve::initialize()
{
    initialize_io();
    initialize_timer();
    _instance = this;
    _is_initialized = true;
}
//
// init/config I/O points used
void Valve::initialize_io()
{
#ifdef DEBUG_CLK_ENABLED
    // debug 'clock' used for scope based timing of operations
    gpio_init(IO_DEBUG_CLK);
    gpio_set_dir(IO_DEBUG_CLK, GPIO_OUT);
#endif

    gpio_init(IO_VALVE);
    gpio_set_dir(IO_VALVE, GPIO_OUT);
    gpio_put(IO_VALVE, false);

    gpio_init(IO_BUSY);
    gpio_set_dir(IO_BUSY, GPIO_OUT);
    gpio_put(IO_BUSY, false);

    // pump enable input - pull up
    gpio_init(IO_RUN);
    gpio_set_dir(IO_RUN, GPIO_IN);
    gpio_set_pulls(IO_RUN, true, false);
}
//
// set up timer IRQ and handler
//
void Valve::initialize_timer()
{
    // disable interrupt
    irq_set_enabled(TIMER_IRQ_1, false);
    // install handler
    irq_set_exclusive_handler(TIMER_IRQ_1, timer_isr);
    // set counter - offsets from current count
    timer_hw->alarm[1] = timer_hw->timerawl + TIMER_INTERVAL_US;
    // enable interrupt
    irq_set_enabled(TIMER_IRQ_1, true);
    // enable clock interrupt
    hw_set_bits(&timer_hw->inte, 1 << 1);
}
//
// Timer ISR
// - clears irq flag, sewts time for _next_ interrupt
// - executes run() on Valve instance
void __time_critical_func(Valve::timer_isr)()
{
    // clear interrupt
    hw_clear_bits(&timer_hw->intr, 1 << 1);
    // Schedule the next interrupt
    uint64_t target = timer_hw->timerawl + TIMER_INTERVAL_US;
    timer_hw->alarm[1] = (uint32_t)target;
    if (_instance)
        _instance->run();
}

void init_valve_controller()
{
    recipe.open_time(10);
    recipe.close_time(10);
    valve.set_recipe(&recipe);
    valve.initialize();
}
