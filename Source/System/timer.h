#ifndef __TIMER_H
#define __TIMER_H

/* 💡定时器2 和定时器3 的中断处理函数位在 timer.c 中定义 */

/**
 * TIM2 用于区分接收到的是不是同一个 MQTT 报文
 * TIM3 暂时没用到，后续用于定时发送 PINGREQ 报文等
 */

void TIM2_Init(uint16_t arr, uint16_t psc);
void TIM3_Init(uint16_t arr, uint16_t psc);

#endif
