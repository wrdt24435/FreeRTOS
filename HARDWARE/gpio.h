#ifndef __GPIO_H
#define __GPIO_H

#include "stm32f4xx.h"

typedef enum {
	RISING, 
	FALLING,
}TRIGGER;

typedef enum {
	LCD_BK,
	KEY_0,
	KEY_1
}PIN;	//ȷ��ö�����pin�ж�Ӧ�ĺ죬��LRD_0_PORT

typedef struct {
	GPIO_TypeDef *port;	//�˿�
	GPIO_InitTypeDef sys;
}MY_IO;


//LED���Ŷ���
#define LED_0_PIN	GPIO_Pin_6  
#define LED_1_PIN	GPIO_Pin_7  
#define LED_0_PORT	GPIOA 
#define LED_1_PORT	GPIOA 

//�ⲿflash���Ŷ���
#define W25Q16_CS_PORT	GPIOB
#define W25Q16_CS_PIN	GPIO_Pin_0
#define W25Q16_SPI_PORT	GPIOB
#define W25Q16_SPI_CLK_PIN		
#define W25Q16_SPI_I_PIN		
#define W25Q16_SPI_O_PIN		
#define W25Q16_SPI_PIN	GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5

//LCD���Ŷ���
#define LCD_BK_PORT	GPIOB
#define LCD_BK_PIN	GPIO_Pin_1

//USART���Ŷ���
#define DEBUG_USART_TX_PORT                GPIOA
#define DEBUG_USART_TX_PIN                 GPIO_Pin_10
#define DEBUG_USART_RX_PORT                GPIOA
#define DEBUG_USART_RX_PIN                 GPIO_Pin_9

//D2���Ŷ���
#define UART_DEVICE2_TX_PORT                GPIOC
#define UART_DEVICE2_TX_PIN                 GPIO_Pin_10
#define UART_DEVICE2_RX_PORT                GPIOC
#define UART_DEVICE2_RX_PIN                 GPIO_Pin_11

//�������Ŷ���
#define KEY_0_PORT	GPIOE
#define KEY_1_PORT	GPIOE
#define KEY_PIN		KEY_0_PIN|KEY_1_PIN
#define KEY_0_PIN	GPIO_Pin_4
#define KEY_1_PIN	GPIO_Pin_3
#define KEY_UP_PORT	GPIOA
#define KEY_UP_PIN	GPIO_Pin_0

//���������Ŷ���
#define TOUCH_IR_PORT	GPIOC
#define TOUCH_IR_PIN	GPIO_Pin_5
#define TOUCH_RST_PORT	GPIOB
#define TOUCH_RST_PIN	GPIO_Pin_12
#define TOUCH_SCL_PORT	GPIOB
#define TOUCH_SCL_PIN	GPIO_Pin_13
#define TOUCH_SDA_PORT	GPIOB
#define TOUCH_SDA_PIN	GPIO_Pin_15





extern MY_IO temp_config;

void gpio_out(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, u32 out);
#define io_set_out(IO, x)	gpio_out(IO##_PORT, IO##_PIN, x)
#define io_read(IO)			GPIO_ReadInputDataBit(IO##_PORT, IO##_PIN)

void gpio_init(MY_IO *config);//��ʼ��		 	
int get_io_status(int num);
u32 check_keyboard_flag(void);
void clean_keyboard_flag(u32 flag);
void change_EXTIX(TRIGGER trigger);
void io_interrupt_init(void);


/*//����ʵ��˼��,�ο�<<CM3Ȩ��ָ��>>������(87ҳ~92ҳ).M4ͬM3����,ֻ�ǼĴ�����ַ����.
//IO�ڲ����궨��
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO�ڵ�ַӳ��
#define GPIOA_ODR_Addr    (GPIOA_BASE+20) //0x40020014
#define GPIOB_ODR_Addr    (GPIOB_BASE+20) //0x40020414 
#define GPIOC_ODR_Addr    (GPIOC_BASE+20) //0x40020814 
#define GPIOD_ODR_Addr    (GPIOD_BASE+20) //0x40020C14 
#define GPIOE_ODR_Addr    (GPIOE_BASE+20) //0x40021014 
#define GPIOF_ODR_Addr    (GPIOF_BASE+20) //0x40021414    
#define GPIOG_ODR_Addr    (GPIOG_BASE+20) //0x40021814   
#define GPIOH_ODR_Addr    (GPIOH_BASE+20) //0x40021C14    
#define GPIOI_ODR_Addr    (GPIOI_BASE+20) //0x40022014     

#define GPIOA_IDR_Addr    (GPIOA_BASE+16) //0x40020010 
#define GPIOB_IDR_Addr    (GPIOB_BASE+16) //0x40020410 
#define GPIOC_IDR_Addr    (GPIOC_BASE+16) //0x40020810 
#define GPIOD_IDR_Addr    (GPIOD_BASE+16) //0x40020C10 
#define GPIOE_IDR_Addr    (GPIOE_BASE+16) //0x40021010 
#define GPIOF_IDR_Addr    (GPIOF_BASE+16) //0x40021410 
#define GPIOG_IDR_Addr    (GPIOG_BASE+16) //0x40021810 
#define GPIOH_IDR_Addr    (GPIOH_BASE+16) //0x40021C10 
#define GPIOI_IDR_Addr    (GPIOI_BASE+16) //0x40022010 
 
//IO�ڲ���,ֻ�Ե�һ��IO��!
//ȷ��n��ֵС��16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //��� 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //���� 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //��� 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //���� 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //��� 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //���� 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //��� 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //���� 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //��� 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //����

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //��� 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //����

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //��� 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //����

#define PHout(n)   BIT_ADDR(GPIOH_ODR_Addr,n)  //��� 
#define PHin(n)    BIT_ADDR(GPIOH_IDR_Addr,n)  //����

#define PIout(n)   BIT_ADDR(GPIOI_ODR_Addr,n)  //��� 
#define PIin(n)    BIT_ADDR(GPIOI_IDR_Addr,n)  //����
*/
/* ֱ�Ӳ����Ĵ����ķ�������IO */
/*#define	digitalHi(p,i)			{p->BSRRH=i;}			  //����Ϊ�ߵ�ƽ		
#define digitalLo(p,i)			{p->BSRRL=i;}				//����͵�ƽ
#define digitalToggle(p,i)		{p->ODR ^=i;}			//�����ת״̬
*/

#endif
