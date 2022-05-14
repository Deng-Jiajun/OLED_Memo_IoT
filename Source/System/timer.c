#include "sys.h"

/**
 * @brief 定时器2初始化
 *
 * @param arr 周期（=自动重装器的值）
 * @param psc 预分频器的值
 *
 * @note 以 TIM2_Init(7200-1,10000-1) 为例
 * 	TIM_Prescaler = 7200-1，72MHz 进行 7200 分频，得到 10kHz，即计数频率为 10kHz（1 秒计数 1 万次）
 *	TIM_Period    = 10000-1，以 10000 次为一个周期进行计数，达到该值引发中断
 *  最终得到的定时间隔为 10kHz * 10000 = 1s
 *
 * 💡实用向总结：固定 psc 为 7200 的情况下，中断间隔为 (arr + 1) * 0.1ms
 */
void TIM2_Init(uint16_t arr, uint16_t psc)
{
    /* 定时器时基单元初始化结构体 */
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;

    /* 使能定时器时钟 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    /* 使用内部时钟（没配置的话，默认就是内部时钟） */
    // TIM_InternalClockConfig(TIM2);

    /* 定时器时基单元参数配置 */
    TIM_TimeBaseInitStructure.TIM_Period = arr;                     // 周期（=自动重装器的值）
    TIM_TimeBaseInitStructure.TIM_Prescaler = psc;                  // 预分频器的值
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;     // 采样频率分频系数（不分频）
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数模式
                                                                    // TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0; // 重复计数器的值，TIM1 和 TIM8 才有效

    /* 根据初始化结构体初始化定时器时基单元 */
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);

    /* 清除更新标志（避免一上电就触发中断） */
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);

    /* 使能定时器中断 */
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    /* 不❌使能定时器 */
    TIM_Cmd(TIM2, DISABLE);
    /* 使能开始计数用在 USART2_IRQHandler 中断处理函数中 */
}

/**
 * @brief 定时器3初始化
 *
 * @param arr 周期（=自动重装器的值）
 * @param psc 预分频器的值
 *
 * @note 以 TIM2_Init(7200-1,10000-1) 为例
 * 	TIM_Prescaler = 7200-1，72MHz 进行 7200 分频，得到 10kHz，即计数频率为 10kHz（1 秒计数 1 万次）
 *	TIM_Period    = 10000-1，以 10000 次为一个周期进行计数，达到该值引发中断
 *  最终得到的定时间隔为 10kHz * 10000 = 1s
 *
 * 💡实用向总结：固定 psc 为 7200 的情况下，中断间隔为 (arr + 1) * 0.1ms
 */
void TIM3_Init(uint16_t arr, uint16_t psc)
{
    /* 定时器时基单元初始化结构体 */
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    /* 使能定时器时钟 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    /* 使用内部时钟（没配置的话，默认就是内部时钟） */
    // TIM_InternalClockConfig(TIM3);

    /* 初始化定时器的时基单元 */
    TIM_TimeBaseStructure.TIM_Period = arr;                     // 周期（=自动重装器的值）
    TIM_TimeBaseStructure.TIM_Prescaler = psc;                  // 预分频器的值
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     // 采样频率分频系数（不分频）
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数模式
    // TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0; // 重复计数器的值，TIM1 和 TIM8 才有效

    /* 根据初始化结构体初始化定时器时基单元 */
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    /* 清除更新标志（避免一上电就触发中断） */
    TIM_ClearFlag(TIM3, TIM_FLAG_Update);

    /* 使能定时器中断 */
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

    /* 使能定时器 */
    TIM_Cmd(TIM3, ENABLE);
}

/**
 * @brief 定时器 2 中断处理函数
 *
 */
void TIM2_IRQHandler(void)
{
    /* 判断的定时器中断源是初始化的时设定的 TIM_IT_Update */
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET) // 检查更新中断是否发生
    {
        /* 如果目前接收的是 MQTT 报文，引发了中断 */
        // if (!AT_Instruction_Mode)
        {
            // 👆实际上没有其他情况，直接屏蔽判断

            /* 说明当前收到的报文已完整，可以进行解析 */
            MQTT_Rx_Buffer_Data_Integrity = true;

            /* 定时器状态恢复 */
            TIM_Cmd(TIM4, DISABLE);
            TIM_SetCounter(TIM4, 0);
            TIM_ClearITPendingBit(TIM2, TIM_IT_Update); // 清除中断标志
        }
    }
}

/**
 * @brief 定时器 3 中断处理函数
 *
 */
void TIM3_IRQHandler(void)
{
    /* 判断的定时器中断源是初始化的时设定的 TIM_IT_Update */
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) // 检查更新中断是否发生
    {

        TIM_ClearITPendingBit(TIM3, TIM_IT_Update); // 清除标志位
    }
}
