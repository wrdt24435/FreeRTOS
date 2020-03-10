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
}PIN;	//确保枚举里的pin有对应的红，如LRD_0_PORT

typedef struct {
	GPIO_TypeDef *port;	//端口
	GPIO_InitTypeDef sys;
}MY_IO;


//LED引脚定义
#define LED_0_PIN	GPIO_Pin_6  
#define LED_1_PIN	GPIO_Pin_7  
#define LED_0_PORT	GPIOA 
#define LED_1_PORT	GPIOA 

//外部flash引脚定义
#define W25Q16_CS_PORT	GPIOB
#define W25Q16_CS_PIN	GPIO_Pin_0
#define W25Q16_SPI_PORT	GPIOB
#define W25Q16_SPI_CLK_PIN		
#define W25Q16_SPI_I_PIN		
#define W25Q16_SPI_O_PIN		
#define W25Q16_SPI_PIN	GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5

//LCD引脚定义
#define LCD_BK_PORT	GPIOB
#define LCD_BK_PIN	GPIO_Pin_1

//USART引脚定义
#define DEBUG_USART_TX_PORT                GPIOA
#define DEBUG_USART_TX_PIN                 GPIO_Pin_10
#define DEBUG_USART_TX_PORT                GPIOA
#define DEBUG_USART_RX_PIN                 GPIO_Pin_9

//按键引脚定义
#define KEY_0_PORT	GPIOE
#define KEY_1_PORT	GPIOE
#define KEY_PIN		KEY_0_PIN|KEY_1_PIN
#define KEY_0_PIN	GPIO_Pin_4
#define KEY_1_PIN	GPIO_Pin_3
#define KEY_UP_PORT	GPIOA
#define KEY_UP_PIN	GPIO_Pin_0

//触摸屏引脚定义
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

void gpio_init(MY_IO *config);//初始化		 	
int get_io_status(int num);
u32 check_keyboard_flag(void);
void clean_keyboard_flag(u32 flag);
void change_EXTIX(TRIGGER trigger);
void io_interrupt_init(void);


/*//具体实现思想,参考<<CM3权威指南>>第五章(87页~92页).M4同M3类似,只是寄存器地址变了.
//IO口操作宏定义
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO口地址映射
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
 
//IO口操作,只对单一的IO口!
//确保n的值小于16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入

#define PHout(n)   BIT_ADDR(GPIOH_ODR_Addr,n)  //输出 
#define PHin(n)    BIT_ADDR(GPIOH_IDR_Addr,n)  //输入

#define PIout(n)   BIT_ADDR(GPIOI_ODR_Addr,n)  //输出 
#define PIin(n)    BIT_ADDR(GPIOI_IDR_Addr,n)  //输入
*/
/* 直接操作寄存器的方法控制IO */
/*#define	digitalHi(p,i)			{p->BSRRH=i;}			  //设置为高电平		
#define digitalLo(p,i)			{p->BSRRL=i;}				//输出低电平
#define digitalToggle(p,i)		{p->ODR ^=i;}			//输出反转状态
*/

#endif
