# OLED_Memo_IoT

 基于物联网的备忘录 OLED 显示

## 项目基础

本项目在 [THMS_IoT](https://github.com/Deng-Jiajun/THMS_IoT) 的基础上建立

## 硬件基础

MCU：stmf103c8t6  
温湿度检测：DHT11  
联网：esp-01s

## 基本工作流程

esp-01s 联网，与阿里云物联网平台 MQTT 服务器建立 TCP 连接，保持连接状态。  
当收到 MQTT 服务器下发的报文时，解析报文内容，更新 OLED 显示屏

## 重点模块

- IIC：实现 MCU 与 OLED 的通信协议支持
- OLED_IIC：实现 MCU 与 OLED 的实际通信
- USART：实现 MCU 的串口通信
- WiFi：实现 MCU 与 esp-01s 的通信（基于 USART2 串口通信）
- MQTT：实现 MCU 与 MQTT 服务器的通信（在 WiFi 模块的基础上实现）

## 运行结果



## 修改内容



## 其他问题

