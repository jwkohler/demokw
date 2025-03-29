#include "pico/stdlib.h"
#include "hardware/claim.h"
#include "hw_claims.h"

static uint8_t bits[2] = {0};

bool hardware_claim(hardware_resource_t rsrc, const char *user)
{
    if (hardware_is_claimed(rsrc))
        return false;
    hw_claim_or_assert(bits, rsrc, user);
    return true;
}

bool hardware_claim_wait_ms(hardware_resource_t rsrc, const char *user, uint32_t wait_ms)
{
    absolute_time_t time_limit = make_timeout_time_ms(wait_ms);
    do
    {
        if (!hardware_is_claimed(rsrc))
        {
            hw_claim_or_assert(bits, rsrc, user);
            return true;
        }
        busy_wait_us(250);
    } while (absolute_time_diff_us(get_absolute_time(), time_limit) > 0);
    return false;
}

void hardware_release(hardware_resource_t rsrc)
{
    hw_claim_clear(bits, rsrc);
}

/// @brief test if the given resource is currently claimed
/// @param rsrc
/// @return true if the resource is claimed
bool hardware_is_claimed(hardware_resource_t rsrc)
{
    return hw_is_claimed(bits, rsrc);
}
