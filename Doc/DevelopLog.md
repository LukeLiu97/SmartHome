# Smart lock project

====================

## Description

---

### Hardwave SmartHome v1.2 

- STM32F103C8T6
  - GPIO
        LED2 PB14

        RGB_CLK PB8
        RGB_DATA PA9

        OLED_RES PB6
        OLED_CS PB7
        OLED_DC PB12

        MOTOR_IA PA7
        MOTOR_I^A PA6
        MOTOR_IB PA5
        MOTOR_I^B PA44

  - SPI
        SPI2_SCLK PB13
        SPI2_MOSI PB15
- P9813
  全彩LED COMS工艺驱动芯片，使用自定义通讯总线

- 128x64 OLED
    驱动为SSD1306,支持6800、8080、三线或四线SPI以及I2C通信
    此处使用了四线SPI

- LB1936 & Step Motor
  使用两相四拍控制步进电机

### Softwave

添加LED2，RGB LED, OLED,step motor and stm32 ChipID 

## Update

---

- ver 0.0.4
    添加步进电机控制，实现窗帘位置控制模拟
- ver 0.0.3
    添加RGB LED 驱动
- ver 0.0.2
    添加OLED驱动，显示芯片序列号
- ver 0.0.1
    创建工程，点亮LED2,检查硬件
