#include "stdint.h"
#define RCC_BASE    0x40021000UL
#define RCC_APB2ENR (*(volatile uint32_t *)(RCC_BASE + 0x18))
#define RCC_APB1ENR (*(volatile uint32_t *)(RCC_BASE + 0x1C))
#define RCC_CFGR    (*(volatile uint32_t *)(RCC_BASE + 0x04))

#define GPIOA_BASE  0x40010800UL
#define GPIOA_CRL   (*(volatile uint32_t *)(GPIOA_BASE + 0x00))
#define GPIOA_CRH   (*(volatile uint32_t *)(GPIOA_BASE + 0x04))

#define GPIOB_BASE  0x40010C00UL
#define GPIOB_CRL   (*(volatile uint32_t *)(GPIOB_BASE + 0x00))

#define I2C1_BASE   0x40005400UL
#define I2C1_CR1         (*(volatile uint32_t *)(I2C1_BASE + 0x00))
#define I2C1_CR2         (*(volatile uint32_t *)(I2C1_BASE + 0x04))
#define I2C1_DR          (*(volatile uint32_t *)(I2C1_BASE + 0x10))
#define I2C1_SR1         (*(volatile uint32_t *)(I2C1_BASE + 0x14))
#define I2C1_SR2         (*(volatile uint32_t *)(I2C1_BASE + 0x18))
#define I2C1_CCR         (*(volatile uint32_t *)(I2C1_BASE + 0x1C))
#define I2C1_TRISE       (*(volatile uint32_t *)(I2C1_BASE + 0x20))

#define ADC1_BASE    0x40012400UL
#define ADC1_SR     (*(volatile uint32_t *)(ADC1_BASE + 0x00))
#define ADC1_CR2    (*(volatile uint32_t *)(ADC1_BASE + 0x08))
#define ADC1_SMPR2  (*(volatile uint32_t *)(ADC1_BASE + 0x10))
#define ADC1_SQR1   (*(volatile uint32_t *)(ADC1_BASE + 0x2C))
#define ADC1_SQR3   (*(volatile uint32_t *)(ADC1_BASE + 0x34))
#define ADC1_DR     (*(volatile uint32_t *)(ADC1_BASE + 0x4C))

#define USART1_BASE     0x40013800UL
#define USART1_SR       (*(volatile uint32_t *)(USART1_BASE + 0x00))
#define USART1_DR       (*(volatile uint32_t *)(USART1_BASE + 0x04))
#define USART1_BRR      (*(volatile uint32_t *)(USART1_BASE + 0x08))
#define USART1_CR1      (*(volatile uint32_t *)(USART1_BASE + 0x0C))

#define IOPAEN          (1 << 2)
#define IOPBEN          (1 << 3)
#define ADC1EN          (1 << 9)
#define UART1EN         (1 << 14)
#define I2C1EN          (1 << 21)

#define LCD_I2C_ADDR    0x27
#define BACKLIGHT_BIT   0x08
#define EN_BIT          0x04
#define RS_BIT          0x01

void delay(uint32_t ms)
{
	volatile uint32_t i,j;
	for(i=0; i<ms; i++)
		for(j=0; j<1000; j++);
}

void I2C1_init(void)
{
	RCC_APB2ENR |= IOPBEN;
	RCC_APB1ENR |= I2C1EN;

	GPIOB_CRL &= ~(0xFF000000);
	GPIOB_CRL |=  (0xFF000000);

	I2C1_CR1 |=  (1 << 15);
	I2C1_CR1 &= ~(1 << 15);

	I2C1_CR2 = 8;
	I2C1_CCR = 40;
	I2C1_TRISE = 9;

	I2C1_CR1 |= (1 << 0);
}

void I2C1_write(uint8_t dev_addr, uint8_t data)
{
	I2C1_CR1 |= (1 << 8);
	while(!(I2C1_SR1 & (1 << 0)));

	I2C1_DR = (dev_addr << 1);
	while(!(I2C1_SR1 & (1 << 1)));
	(void)I2C1_SR2;

	while(!(I2C1_SR1 & (1 << 7)));
	I2C1_DR = data;
	while(!(I2C1_SR1 & (1 << 2)));

	I2C1_CR1 |= (1 << 9);
}

void LCD_I2C_out(uint8_t nibble_data)
{
	I2C1_write(LCD_I2C_ADDR, (nibble_data | EN_BIT) | BACKLIGHT_BIT);
	delay(2);
	I2C1_write(LCD_I2C_ADDR, (nibble_data & ~EN_BIT) | BACKLIGHT_BIT);
	delay(2);
}

void LCD_command(uint8_t cmd)
{
	uint8_t high_nibble = (cmd & (0xF0));
	uint8_t low_nibble  = ((cmd << 4) & 0xF0);
	LCD_I2C_out(high_nibble & ~RS_BIT);
	LCD_I2C_out(low_nibble  & ~RS_BIT);
}

void LCD_data(uint8_t data)
{
	uint8_t high_nibble = (data & (0xF0));
	uint8_t low_nibble  = ((data << 4) & 0xF0);
	LCD_I2C_out(high_nibble | RS_BIT);
	LCD_I2C_out(low_nibble | RS_BIT);
}

void LCD_init(void)
{
	delay(50);

	LCD_I2C_out(0x30 & ~RS_BIT);
	delay(5);
	LCD_I2C_out(0x30 & ~RS_BIT);
	delay(1);
	LCD_I2C_out(0x30 & ~RS_BIT);
	delay(1);
	LCD_I2C_out(0x20 & ~RS_BIT);
	delay(1);

	LCD_command(0x28);
	LCD_command(0x0C);
	LCD_command(0x01);
	delay(10);
	LCD_command(0x06);
}

void LCD_string(char *str)
{
	while(*str)
	{
		LCD_data(*str++);
	}
}

void LCD_float(float value)
{
	uint16_t integer,decimal;

	integer = (uint16_t)value;
	decimal = (uint16_t)((value - (float)integer) * 100.0f);
	LCD_data(integer + '0');
	LCD_data('.');
	LCD_data((decimal /10) + '0');
	LCD_data((decimal % 10) + '0');
}

void ADC1_init(void)
{
	RCC_APB2ENR |= IOPAEN;
	RCC_APB2ENR |= ADC1EN;
	GPIOA_CRL &= ~(0xF << 0);

	ADC1_SMPR2 &= ~(7 << 0);
	ADC1_SMPR2 |=  (7 << 0);

	ADC1_SQR1 = 0;
	ADC1_SQR3 = 0;

	ADC1_CR2 |= (1 << 0);
	delay(1);

	ADC1_CR2 |= (1 << 3);
	while((ADC1_CR2 & (1 << 3)));

	ADC1_CR2 |= (1 << 2);
	while((ADC1_CR2 & (1 << 2)));

	ADC1_CR2 &= ~(7 << 17);
	ADC1_CR2 |= (7 << 17);

	ADC1_CR2 |= (1 << 20);
}

float ADC_read(void)
{
	uint16_t adc_value;
	ADC1_CR2 |= (1 << 22);
	while(!(ADC1_SR & (1 << 1)));
	adc_value = (uint16_t)ADC1_DR;
	return ((float) adc_value * 3.3f)/4095.0f;
}

void UART_init(void)
{
	RCC_APB2ENR |= UART1EN;
	GPIOA_CRH &= ~(0xF << 4);
	GPIOA_CRH |=  (0xB << 4);

	USART1_BRR = 0x341;
	USART1_CR1 |= (1 << 13);
	USART1_CR1 |= (1 << 3);
}

void UART_char(char data)
{
	while(!(USART1_SR & (1 << 7)));
	USART1_DR = data;
}

void UART_string(char *str)
{
	while(*str)
	{
		UART_char(*str++);
	}
}

void UART_float(float value)
{
	uint16_t integer,decimal;

	integer = (uint16_t)value;
	decimal = (uint16_t)((value - integer)*100);

	UART_char(integer + '0');
	UART_char('.');
	UART_char((decimal / 10) + '0');
	UART_char((decimal % 10) + '0');
}

int main(void)
{
	float voltage;

	for(volatile uint32_t wait = 0; wait<100; wait++);

	I2C1_init();
	ADC1_init();
	UART_init();
	LCD_init();

	while(1)
	{
		voltage = ADC_read();

		LCD_command(0x80);
		LCD_string("ADC_POT_VALUE: ");
		LCD_command(0xC0);
		LCD_float(voltage);

		UART_string("ADC_POT_VALUE: ");
		UART_float(voltage);
		UART_string("\r\n");
		delay(300);
	}
}
