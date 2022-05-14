#include "sys.h"

int main(void)
{
    int i = 0;
    const char *topic_filter_subscribe = "/sys/h6wxwxhbiAn/OLED_01/thing/service/property/set";
    char topic_name_publish[1024] = "/sys/h6wxwxhbiAn/OLED_01/thing/event/property/post";
    char payload_json[1024] = {0};  // 有效载荷的 json 字符串
    char display_string[256] = {0}; // json 中 key 为 Display 的 value

    /* 各种初始化 */
    delay_init();
    NVIC_Config();
    TIM2_Init(400-1, 7200-1);// 40ms 引发一次中断
    OLED_Init();
    USART1_Init();
    USART2_Init();
    WiFi_Init();

    /* OLED 清屏 */
    OLED_Clear();

    /* 连接 MQTT 服务器 */
    if (MQTT_CONNECT("OLED_01", "OLED_01", "h6wxwxhbiAn", "0ca71ab3c494ad6037f2f3134f9d4343", 3, "hmacsha1"))
        printf("mqtt connect success\r\n");
    else
        printf("mqtt connect failed\r\n");

    /* 订阅主题 */
    if (MQTT_SUBSCRIBE(topic_filter_subscribe, 0))
        printf("mqtt subscribe success\r\n");
    else
        printf("mqtt subscribe failed\r\n");

    /* MQTT 输入缓冲区清零 */
    MQTT_Rx_Length = 0;

    while (1)
    {
        /* 如果输入缓冲区有 publish 报文，就读出来
            这个判断可以细化，比如判断 Topic Name 是否一致，
            另外，也可以在中断处理函数对收到的不同报文做不同的处理 */
        if (MQTT_Rx_Length && MQTT_Rx_Buffer_Data_Integrity && MQTT_Rx_Buffer[0] == 0x30)
        {
            /* 从缓冲区里把 有效载荷部分取出来，写入 payload_json */
            Get_Payload_JSON_From_MQTT_Rx_Buffer(payload_json);

            /* 调用 cJSON 库解析 payload_json 字符串，取出 Display 的内容 */
            GET_Display_From_Payload_JSON(payload_json, display_string);

            printf("display_string: %s\r\n", display_string);

            /* OLED 清屏重绘 */
            OLED_Clear();
            OLED_ShowStr(0, 0, display_string);

            /* 发送响应报文，上报当前的 display 属性，表明已设备已做出响应 */
            sprintf(payload_json, "{\"id\":\"1234\",\"version\":\"1.0\",\"method\":\"thing.event.property.post\",\"params\":{\"Display\":\"%s\"}}", display_string);
            MQTT_PUBLISH(topic_name_publish, payload_json, 0);

            /* MQTT 输入缓冲区清零 */
            MQTT_Rx_Length = 0;
        }
        else
        {
            if (i++ > 60)
            {

                /* 隔一段时间发送一次 PINGREQ 保活 */
                if (MQTT_PINGREQ())
                {
                    printf("MQTT PINGREQ success\r\n");
                    i = 0;
                    /* 收到响应了就开始新一轮的计时 */
                }
                else
                {
                    printf("MQTT PINGREQ failed\r\n");
                    /* 没收到响应的话，再发一次 PINGREQ */
                    if (MQTT_PINGREQ())
                    {
                        printf("MQTT PINGREQ success\r\n");
                        i = 0;
                    }
                    else
                    {
                        /* 还没有响应，直接结束重来 */
                        printf("MQTT PINGREQ failed\r\nprogram end.\r\n");
                        return 1;

                        /* 💡实际应用中可以考虑做一些重连的动作 */
                    }
                }

                /* MQTT 输入缓冲区清零 */
                MQTT_Rx_Length = 0;
            }
        }
        delay_ms(1000);
    }
}

// memo1           1. item1        2.item2         3.item3
