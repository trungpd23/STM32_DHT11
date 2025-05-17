#include "stm32f10x.h"                  // Device header
#include "dht.h"
#include "delay.h"

#define TYPE_DHT11

#define DHT_PORT GPIOA
#define DHT_PIN GPIO_Pin_1

uint8_t Rh_byte1, Rh_byte2, Temp_byte1, Temp_byte2;
uint16_t sum;
uint8_t presence = 0;
float temp = 0, humi = 0;

void SetPinOutput(GPIO_TypeDef* GPIOx, uint16_t Pin) {
	GPIO_InitTypeDef gpio;
	gpio.GPIO_Pin = Pin;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOx, &gpio);
}

void SetPinInput(GPIO_TypeDef* GPIOx, uint16_t Pin) {
	GPIO_InitTypeDef gpio;
	gpio.GPIO_Pin = Pin;
	gpio.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOx, &gpio);
}

void DHT_Start(void) {
	SetPinOutput(DHT_PORT, DHT_PIN); // cau hinh chan la ouput
	GPIO_ResetBits(DHT_PORT, DHT_PIN); //keo chan xuong muc 0
	delay_ms(20);
	GPIO_SetBits(DHT_PORT, DHT_PIN); // keo chan len muc 1
	delay_ms(30);
	SetPinInput(DHT_PORT, DHT_PIN); // cau hunh chan la input
}

uint8_t DHTCheckResponse(void) {
	uint8_t Response = 0;
	delay_us(40);
	if(!GPIO_ReadInputDataBit(DHT_PORT, DHT_PIN)) { // Neu chan o muc 0
		delay_us(80);
		if(GPIO_ReadInputDataBit(DHT_PORT, DHT_PIN)) Response = 1; // Neu chan o muc 1
		else Response = -1;
	}
	while((GPIO_ReadInputDataBit(DHT_PORT, DHT_PIN))); // Cho cho chan ve muc 0
	return Response;
}

uint8_t DHT_Read(void) {
	uint8_t i, j;
	
	for(j = 0; j < 8; j++) {
		while(!(GPIO_ReadInputDataBit(DHT_PORT, DHT_PIN))); //Cho chan len muc 1
		delay_us(40);
		if(!(GPIO_ReadInputDataBit(DHT_PORT, DHT_PIN))) { //Neu chan o muc 0
			i &= ~(1 << (7 - j));
		}
		else {
			i |= (1 << (7 - j));
		}
		while(GPIO_ReadInputDataBit(DHT_PORT, DHT_PIN)); // cho chan xuong muc 0
	}
	return i;
}

void DHT_GetData(DHT_DataTypedef *DHT_data) {
	DHT_Start();
	presence = DHTCheckResponse();
	Rh_byte1 = DHT_Read();
	Rh_byte2 = DHT_Read();
	Temp_byte1 = DHT_Read();
	Temp_byte2 = DHT_Read();
	sum = DHT_Read();
	
	if(sum == (Rh_byte1 + Rh_byte2 + Temp_byte1 + Temp_byte2)) {
		temp = (float)Temp_byte1 + (float)(Temp_byte2/10.0);
		humi = (float)Rh_byte1 + (float)(Rh_byte2/10.0);
		DHT_data->Temperature = Temp_byte1;
		DHT_data->Humidity = Rh_byte1;
	}
}
