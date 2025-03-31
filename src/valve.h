#pragma once
#include <pico/stdlib.h>

//
// specifies rate for timer interrupt in microseconds
#define TIMER_INTERVAL_US 100

#define DEBUG_CLK_ENABLED
typedef uint32_t duration_t;
void init_valve_controller();

#ifndef IO_VALVE
#error "IO_VALVE not defined"
#endif
#ifndef IO_RUN
#error "IO_RUN not defined"
#endif
#ifndef IO_BUSY
#error "IO_BUSY not defined"
#endif

#ifdef DEBUG_CLK_ENABLED
#ifndef IO_DEBUG_CLK
#error "IO_DEBUG_CLK not defined"
#endif
#endif

// DotRecipe
// parameters controlling the valve ceposition of a single dot.
// open time:  duration the valve is held in the open position
// close time: duration the valcve is held in the closed
//      position, following an open period
// units: 100usec
//
class DotRecipe
{
private:
    float _open_time;
    float _close_time;
    duration_t _open_time_normal;
    duration_t _close_time_normal;
    float _pre_open_delay;
    duration_t _pre_open_delay_normal;
    friend class Valve;

    // 'normalize' a duration in milliseconds to whatever units
    // the valve controller is working in
    // valve controller sets its time unit with TIMER_INTERVAL_US which is
    // the number of microseconds per 'tick'
    duration_t normalize(float t) { return duration_t((t + 0.00) * (1000 / TIMER_INTERVAL_US)); }

public:
    DotRecipe()
        : _open_time(10),
          _close_time(10),
          _pre_open_delay(0)
    {
    }
    DotRecipe(duration_t ot, duration_t ct)
        : _open_time(ot),
          _close_time(ct),
          _pre_open_delay(0)
    {
    }

    float open_time() const { return _open_time; }
    void open_time(float v)
    {
        _open_time = v;
        _open_time_normal = normalize(_open_time);
    }
    float close_time() const { return _close_time; }
    void close_time(float v)
    {
        _close_time = v;
        _close_time_normal = normalize(_close_time);
    }
    float pre_open_delay() const { return _pre_open_delay; }
    void pre_open_delay(float v) { _pre_open_delay = v; }
};

class Valve
{
private:
    typedef enum state
    {
        IDLE,
        OPEN,
        CLOSE,
        ERROR
    } state_t;

    static Valve *_instance;
    static void timer_isr();

    state_t _state;
    bool _is_initialized;
    uint32_t _state_count;
    bool _force_run;
    const DotRecipe *_recipe;

    void run();

protected:
    void initialize_io();
    void initialize_timer();
    // true when RUN input is asserted - note: active LO
    bool is_run_asserted() { return !gpio_get(IO_RUN); }
    bool is_forced_run() { return _force_run; }
    bool should_run() { return is_run_asserted() || is_forced_run(); }
    void set_busy() { gpio_put(IO_BUSY, true); }
    void clear_busy() { gpio_put(IO_BUSY, false); }
    void set_valve_open() { gpio_put(IO_VALVE, true); }
    void set_valve_close() { gpio_put(IO_VALVE, false); }

#ifdef DEBUG_CLK_ENABLED
    void dbg_clk_on() { gpio_put(IO_DEBUG_CLK, 1); }
    void dbg_clk_off() { gpio_put(IO_DEBUG_CLK, 0); }
    void dbg_clk_set(bool s) { gpio_put(IO_DEBUG_CLK, s); }
#else
    void dbg_clk_on() {}
    void dbg_clk_off() {}
#endif

public:
    Valve();
    void initialize();
    void set_recipe(const DotRecipe *rcp) { _recipe = rcp; }
    void force(bool b) { _force_run = b; }
};