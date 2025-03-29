#pragma once

typedef enum
{
    HARDWARE_I2C0, // 0
    HARDWARE_I2C1,
    HARDWARE_SPI0,
    HARDWARE_SPI1,
    HARDWARE_UART0,
    HARDWARE_UART1, // 5
    HARDWARE_PWM,
    HARDWARE_ADC,
    HARDWARE_PIO0,
    HARDWARE_PIO1,
    HARDWARE_USB, // 10
    HW_CLAIM_MAX_ID = HARDWARE_USB
} hardware_resource_t;

bool hardware_claim(hardware_resource_t rsrc, const char *user);
bool hardware_claim_wait_ms(hardware_resource_t rsrc, const char *user, uint32_t wait_ms);
void hardware_release(hardware_resource_t rsrc);
bool hardware_is_claimed(hardware_resource_t rsrc);
