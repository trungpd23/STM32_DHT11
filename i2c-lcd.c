#include "i2c-lcd.h"

// Bien cuc bo
static uint8_t _backlight;

// Ham gui byte qua I2C
static void I2C_WriteByte(uint8_t addr, uint8_t data) {
    // Cho I2C san sang
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
    
    // Bat dau truyen
    I2C_GenerateSTART(I2C1, ENABLE);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
    
    // Gui dia chi module I2C
    I2C_Send7bitAddress(I2C1, addr << 1, I2C_Direction_Transmitter);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    
    // Gui du lieu
    I2C_SendData(I2C1, data);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    
    // Ket thuc truyen
    I2C_GenerateSTOP(I2C1, ENABLE);
}

// Ham gui nibble (4 bit) qua I2C
static void LCD_I2C_WriteNibble(uint8_t nibble, uint8_t rs) {
    uint8_t data = nibble << 4;     // Du lieu o 4 bit cao
    data |= rs;                     // RS bit
    data |= _backlight;             // Backlight bit
    
    // Gui voi Enable = 0
    I2C_WriteByte(LCD_I2C_ADDR, data);
    
    // Gui voi Enable = 1
    I2C_WriteByte(LCD_I2C_ADDR, data | LCD_EN);
    
    // Doi 1ms
    for (int i = 0; i < 1000; i++);
    
    // Gui voi Enable = 0
    I2C_WriteByte(LCD_I2C_ADDR, data & ~LCD_EN);
    
    // Doi 50µs
    for (int i = 0; i < 50; i++);
}

// Ham gui byte (8 bit) qua I2C (che do 4 bit)
static void LCD_I2C_WriteByte(uint8_t data, uint8_t rs) {
    LCD_I2C_WriteNibble(data >> 4, rs);  // Gui 4 bit cao
    LCD_I2C_WriteNibble(data & 0x0F, rs); // Gui 4 bit thap
}

// Kh?i t?o I2C
static void I2C_Config(void) {
    GPIO_InitTypeDef GPIO_InitStruct;
    I2C_InitTypeDef I2C_InitStruct;
    
    // C?p clock cho GPIO và I2C
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
    
    // C?u hình chân GPIO I2C
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; // SDA & SCL
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD; // Alternate function - Open drain
    GPIO_Init(GPIOB, &GPIO_InitStruct);
    
    // C?u hình I2C
    I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStruct.I2C_OwnAddress1 = 0x00; // Ð?a ch? c?a master
    I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStruct.I2C_ClockSpeed = 100000; // 100kHz
    
    // Kh?i t?o I2C
    I2C_Init(I2C1, &I2C_InitStruct);
    
    // Kích ho?t I2C
    I2C_Cmd(I2C1, ENABLE);
}

// Khoi tao LCD
void LCD_I2C_Init(void) {
    // Kh?i t?o I2C
    I2C_Config();
    
    // Ð?i LCD kh?i d?ng
    for(int i = 0; i < 100000; i++);
    
    // Mac dinh bat den nen
    _backlight = LCD_BACKLIGHT;
    
    // Khoi tao trong che do 4 bit
    LCD_I2C_WriteNibble(0x03, 0);
    for(int i = 0; i < 50000; i++);
    LCD_I2C_WriteNibble(0x03, 0);
    for(int i = 0; i < 50000; i++);
    LCD_I2C_WriteNibble(0x03, 0);
    for(int i = 0; i < 50000; i++);
    LCD_I2C_WriteNibble(0x02, 0);
    
		// Cau hinh LCD: 4-bit, 2 dong, font 5x8
    LCD_I2C_SendCommand(LCD_FUNCTION_SET | LCD_4BIT_MODE | LCD_2LINE | LCD_5x8_DOTS);
    
    // Tat hien thi
    LCD_I2C_SendCommand(LCD_DISPLAY_CONTROL | LCD_DISPLAY_OFF);
    
    // Xoa man hinh
    LCD_I2C_Clear();
    
    // Dat che do nhap tu trai sang phai
    LCD_I2C_SendCommand(LCD_ENTRY_MODE | LCD_ENTRY_LEFT);
    
    // Bat hien thi
    LCD_I2C_SendCommand(LCD_DISPLAY_CONTROL | LCD_DISPLAY_ON | LCD_CURSOR_OFF | LCD_BLINK_OFF);
}

// Gui lenh
void LCD_I2C_SendCommand(uint8_t cmd) {
    LCD_I2C_WriteByte(cmd, 0);
}

// Gui du lieu
void LCD_I2C_SendData(uint8_t data) {
    LCD_I2C_WriteByte(data, LCD_RS);
}

// Xoa man hinh
void LCD_I2C_Clear(void) {
    LCD_I2C_SendCommand(LCD_CLEAR);
    for(int i = 0; i < 200000; i++); // Doi 2ms
}

// Ve vi tri dau
void LCD_I2C_Home(void) {
    LCD_I2C_SendCommand(LCD_HOME);
    for(int i = 0; i < 200000; i++); // Doi 2ms
}

// Dat vi tri con tro
void LCD_I2C_SetCursor(uint8_t col, uint8_t row) {
    uint8_t row_offsets[] = {0x00, 0x40}; // Offset cho dong 0 và 1
    LCD_I2C_SendCommand(LCD_SET_DDRAM_ADDR | (col + row_offsets[row]));
}

// Tat hien thi
void LCD_I2C_NoDisplay(void) {
    LCD_I2C_SendCommand(LCD_DISPLAY_CONTROL | LCD_DISPLAY_OFF);
}

// Bat hien thi
void LCD_I2C_Display(void) {
    LCD_I2C_SendCommand(LCD_DISPLAY_CONTROL | LCD_DISPLAY_ON);
}

// Tat con tro
void LCD_I2C_NoCursor(void) {
    LCD_I2C_SendCommand(LCD_DISPLAY_CONTROL | LCD_DISPLAY_ON | LCD_CURSOR_OFF);
}

// Bat con tro
void LCD_I2C_Cursor(void) {
    LCD_I2C_SendCommand(LCD_DISPLAY_CONTROL | LCD_DISPLAY_ON | LCD_CURSOR_ON);
}

// Tat nhap nhay
void LCD_I2C_NoBlink(void) {
    LCD_I2C_SendCommand(LCD_DISPLAY_CONTROL | LCD_DISPLAY_ON | LCD_BLINK_OFF);
}

// Bat nhap nhay
void LCD_I2C_Blink(void) {
    LCD_I2C_SendCommand(LCD_DISPLAY_CONTROL | LCD_DISPLAY_ON | LCD_BLINK_ON);
}

// Tat den nen
void LCD_I2C_NoBacklight(void) {
    _backlight = 0;
    I2C_WriteByte(LCD_I2C_ADDR, _backlight);
}

// Bat den nen
void LCD_I2C_Backlight(void) {
    _backlight = LCD_BACKLIGHT;
    I2C_WriteByte(LCD_I2C_ADDR, _backlight);
}

// In chuoi
void LCD_I2C_Print(char *str) {
    while(*str) {
        LCD_I2C_PrintChar(*str++);
    }
}

// In ky tu
void LCD_I2C_PrintChar(char c) {
    LCD_I2C_SendData(c);
}
