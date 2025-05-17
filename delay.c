#include "stm32f10x.h"

static uint8_t dwt_started = 0;

/**
 * @brief  Kh?i t?o DWT d? s? d?ng delay_us(), delay_ms()
 * @retval 0 n?u th�nh c�ng, 1 n?u th?t b?i
 */
uint32_t DWT_Delay_Init(void)
{
    // K�ch ho?t Trace (TRC)
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;

    // B?t b? d?m chu k? CPU (CYCCNT)
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

    // Reset b? d?m
    DWT->CYCCNT = 0;

    // NOP v�i l?n d? d?m b?o DWT ho?t d?ng
    __ASM volatile ("NOP");
    __ASM volatile ("NOP");
    __ASM volatile ("NOP");

    // Ki?m tra CYCCNT d� ch?y chua
    if (DWT->CYCCNT) {
        dwt_started = 1;
        return 0; // OK
    } else {
        return 1; // Fail
    }
}

/**
 * @brief Delay micro gi�y
 * @param us: s? micro gi�y c?n delay
 */
void delay_us(uint32_t us)
{
    if (!dwt_started) return;

    uint32_t start_tick = DWT->CYCCNT;
    uint32_t delay_ticks = us * (SystemCoreClock / 1000000);

    while ((DWT->CYCCNT - start_tick) < delay_ticks);
}

/**
 * @brief Delay mili gi�y
 * @param ms: s? mili gi�y c?n delay
 */
void delay_ms(uint32_t ms)
{
    while (ms--) {
        delay_us(1000);
    }
}
