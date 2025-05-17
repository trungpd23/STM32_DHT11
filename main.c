#include "stm32f10x.h"
#include "i2c-lcd.h"
#include "dht.h"
#include "delay.h"
#include <stdio.h> // dùng sprintf()

DHT_DataTypedef DHT11_Data;
float Temp, Humi;
char lcd_temp[20], lcd_humi[20];

int main(void) {
	LCD_I2C_Init();
	LCD_I2C_Print("HELLO");
	delay_us(2000);
	LCD_I2C_Clear();
	
	while(1) {
		DHT_GetData(&DHT11_Data);
		Temp = DHT11_Data.Temperature;
		Humi = DHT11_Data.Humidity;
		delay_us(3000);
		
		LCD_I2C_SetCursor(0, 0);
		sprintf(lcd_temp, "T: %.1f", Temp);
		LCD_I2C_Print(lcd_temp);
		LCD_I2C_SetCursor(0, 1);
		sprintf(lcd_temp, "H: %.1f", Humi);
		LCD_I2C_Print(lcd_humi);
		delay_us(5000);
	}
}