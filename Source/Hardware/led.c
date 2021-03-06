#include "sys.h"


/* ð åé¨ä½¿ç¨çéç¨å½æ°ï¼ä¸å¯¹å¤æ´é²ï¼ä¸å¨ .h éå£°æ)------- */
void __LED_Init(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

/* --- å¯¹å¤æ´é²çå½æ°ï¼å¦æ LED çéèåæ°éæ¹åäºï¼ä¿®æ¹è¿é¨åå½æ° --- */

/**
 * @brief LED åå§åå½æ°ã
 *        å¦æ LED çéèåæ°éæ¹åäºï¼ä¿®æ¹è¿ä¸ªå½æ°å³å¯
 */
void LED_Init(void)
{
    /* åå§å LED1ï¼PA7ï¼å LED2ï¼PA8ï¼ */
    __LED_Init(LED1_GPIO, LED1_PIN | LED2_PIN);

    /* åå§åä¹åå³ç¯ */
    GPIO_SetBits(LED1_GPIO, LED1_PIN | LED2_PIN);
}

/**
 * @brief å¼å¯ LEDã
 *        å¦æ LED çéèåæ°éæ¹åäºï¼ä¿®æ¹è¿ä¸ªå½æ°å³å¯
 *
 * @param led_num è¦å¼å¯ç LED ç¼å·ï¼ä» 1 å¼å§
 */
void LED_On(uint8_t led_num)
{
    if (led_num == 1)
    {
        GPIO_ResetBits(LED1_GPIO, LED1_PIN);
    }
    else if (led_num == 2)
    {
        GPIO_ResetBits(LED2_GPIO, LED2_PIN);
    }
}

/**
 * @brief å³é­ LEDã
 *        å¦æ LED çéèåæ°éæ¹åäºï¼ä¿®æ¹è¿ä¸ªå½æ°å³å¯
 *
 * @param led_num è¦å¼å¯ç LED ç¼å·ï¼ä» 1 å¼å§
 */
void LED_Off(uint8_t led_num)
{
    if (led_num == 1)
    {
        GPIO_SetBits(LED1_GPIO, LED1_PIN);
    }
    else if (led_num == 2)
    {
        GPIO_SetBits(LED2_GPIO, LED2_PIN);
    }
}

/**
 * @brief ç¿»è½¬ LEDã
 *        å¦æ LED çéèåæ°éæ¹åäºï¼ä¿®æ¹è¿ä¸ªå½æ°å³å¯
 *
 * @param led_num è¦å¼å¯ç LED ç¼å·ï¼ä» 1 å¼å§
 */
void LED_Toggle(uint8_t led_num)
{
    /* âstm32f10x_gpio æ²¡æå®ä¹ GPIO_ToggleBits() è¿ä¸ªå½æ° */

    if (led_num == 1)
    {
        if (GPIO_ReadOutputDataBit(LED1_GPIO, LED1_PIN) == 0)
        {
            GPIO_SetBits(LED1_GPIO, LED1_PIN);
        }
        else
        {
            GPIO_ResetBits(LED1_GPIO, LED1_PIN);
        }
    }
    else if (led_num == 2)
    {
        if (GPIO_ReadOutputDataBit(LED2_GPIO, LED2_PIN) == 0)
        {
            GPIO_SetBits(LED2_GPIO, LED2_PIN);
        }
        else
        {
            GPIO_ResetBits(LED2_GPIO, LED2_PIN);
        }
    }
}

/* --- ðåé¨éç¨å½æ°ï¼åºè¯¥ä¸éè¦ä¿®æ¹ï¼åªéè¦ä¿®æ¹ä¸é¢çå½æ°å³å¯ --- */
/**
 * @brief åå§å LED
 *
 * @param GPIOx LED ç«¯å£ï¼å¯å¡« GPIOxï¼x=A..Eï¼
 *
 * @param GPIO_Pin LED ç«¯å£å¯¹åºçå¼èï¼å¯å¡« GPIO_Pin_x (x=0..15)ã
 *                 å¯ä»¥å¡«å¤ä¸ªå¼èçç»åï¼æ¯å¦ GPIO_Pin_0 | GPIO_Pin_1
 */
void __LED_Init(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    /* GPIO åå§åç»æä½ */
    GPIO_InitTypeDef GPIO_InitStructure;

    /* åæ°æ£æ¥ */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GPIO_PIN(GPIO_Pin));

    /* å¼å¯ GPIO æ¶é */
    RCC_APB2PeriphClockCmd(Get_RCC_APB2Periph_GPIOx(GPIOx), ENABLE);

    /* éç½® GPIO å¼è */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin;
    /* éç¨æ¨æ½è¾åº */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    /* è¾åºéåº¦ 50MHz */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    /* åå§å GPIO */
    GPIO_Init(GPIOx, &GPIO_InitStructure);
}



