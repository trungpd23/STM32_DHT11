#ifndef I2C_LCD_H
#define I2C_LCD_H

#include "stm32f10x.h"
#include "stm32f10x_i2c.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

// Dinh nghia dia chi I2C
#define LCD_I2C_ADDR 0x27

// Dinh nghia cac bit dieu khien LCD
#define LCD_EN 0x04  // Enable bit
#define LCD_RW 0x02  // Read/Write bit
#define LCD_RS 0x01  // Register select bit
#define LCD_BACKLIGHT 0x08  // Deb nen

// Dinh nghia lenh LCD
#define LCD_CLEAR           0x01  // Xoa man hinh
#define LCD_HOME            0x02  // Ve vi tri dau
#define LCD_ENTRY_MODE      0x04  // Che do nhap
#define LCD_DISPLAY_CONTROL 0x08  // Dieu khien hien thi
#define LCD_FUNCTION_SET    0x20  // Thiet lap chuc nang
#define LCD_SET_DDRAM_ADDR  0x80  // Dat dia hci DDRAM

// Co dieu khien
#define LCD_ENTRY_RIGHT          0x00
#define LCD_ENTRY_LEFT           0x02
#define LCD_ENTRY_SHIFT_INC      0x01
#define LCD_ENTRY_SHIFT_DEC      0x00

#define LCD_DISPLAY_ON           0x04
#define LCD_DISPLAY_OFF          0x00
#define LCD_CURSOR_ON            0x02
#define LCD_CURSOR_OFF           0x00
#define LCD_BLINK_ON             0x01
#define LCD_BLINK_OFF            0x00

#define LCD_4BIT_MODE            0x00
#define LCD_8BIT_MODE            0x10
#define LCD_2LINE                0x08
#define LCD_1LINE                0x00
#define LCD_5x10_DOTS            0x04
#define LCD_5x8_DOTS             0x00

// Ham khoi tao
void LCD_I2C_Init(void);

// Ham gui du lieu/lenh
void LCD_I2C_SendCommand(uint8_t cmd);
void LCD_I2C_SendData(uint8_t data);

// Ham dieu khien
void LCD_I2C_Clear(void);
void LCD_I2C_Home(void);
void LCD_I2C_SetCursor(uint8_t col, uint8_t row);
void LCD_I2C_NoDisplay(void);
void LCD_I2C_Display(void);
void LCD_I2C_NoCursor(void);
void LCD_I2C_Cursor(void);
void LCD_I2C_NoBlink(void);
void LCD_I2C_Blink(void);
void LCD_I2C_NoBacklight(void);
void LCD_I2C_Backlight(void);

// Ham hien thi
void LCD_I2C_Print(char *str);
void LCD_I2C_PrintChar(char c);

#endif // I2C_LCD_H
