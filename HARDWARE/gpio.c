//优化失败，修改时会改动大量文件，应该做到只传接口
//可以定义三个IO初始化函数方向调整(输入、输出、复用)
//可以直接操作寄存器
//如gpio_set_out	gpio_set_in
#include "common.h" 
#include "gpio.h" 

#if DEBUG
#define GPIO_PRINTF PRINTF
#else
#define GPIO_PRINTF
#endif


MY_IO temp_config;
static u32 IRQ_flag = 0;
static u8 keyup_status = 1;	//0 ->是按下；  1->抬起


void gpio_init(MY_IO *config)	
{
	u32 rcc_ahb1;
	switch ((u32)config->port) {
		case (u32)GPIOA: 
			rcc_ahb1 = RCC_AHB1Periph_GPIOA;
			break;
		case (u32)GPIOB: 
			rcc_ahb1 = RCC_AHB1Periph_GPIOB;
			break;
		case (u32)GPIOC: 
			rcc_ahb1 = RCC_AHB1Periph_GPIOC;
			break;
		case (u32)GPIOD: 
			rcc_ahb1 = RCC_AHB1Periph_GPIOD;
			break;
		case (u32)GPIOE: 
			rcc_ahb1 = RCC_AHB1Periph_GPIOE;
			break;
		case (u32)GPIOF: 
			rcc_ahb1 = RCC_AHB1Periph_GPIOF;
			break;
		case (u32)GPIOG: 
			rcc_ahb1 = RCC_AHB1Periph_GPIOG;
			break;
		default :
			break;
	}		
	RCC_AHB1PeriphClockCmd(rcc_ahb1, ENABLE);
	GPIO_Init(config->port, &config->sys);
} 



//PA0 WKUP中断初始化
void io_interrupt_init(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);//PA0 连接到中断线0
	/* 配置EXTI_Line0 */	
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;//LINE0
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //上升沿触发 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能LINE0
	EXTI_Init(&EXTI_InitStructure);//配置

	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;//外部中断0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);//配置
}

void change_EXTIX(TRIGGER trigger)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	//if (num == 1) {
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	//}
	if (trigger == RISING) {
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; 
	}
	else {
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; 
	}
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能LINE0
	EXTI_Init(&EXTI_InitStructure);//配置
}

#if 0
  static  const  CPU_INT08U  CPU_CntLeadZerosTbl[256] = {/*   索引           */ 
	8u,7u,6u,6u,5u,5u,5u,5u,4u,4u,4u,4u,4u,4u,4u,4u,   /*   0x00 to 0x0F   */ 
	3u,3u,3u,3u,3u,3u,3u,3u,3u,3u,3u,3u,3u,3u,3u,3u,   /*   0x10 to 0x1F   */ 
	2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,   /*   0x20 to 0x2F   */ 
	2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,   /*   0x30 to 0x3F   */ 
	1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,   /*   0x40 to 0x4F   */ 
	1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,   /*   0x50 to 0x5F   */ 
	1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,   /*   0x60 to 0x6F   */ 
	1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,   /*   0x70 to 0x7F   */ 
	0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,   /*   0x80 to 0x8F   */ 
	0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,   /*   0x90 to 0x9F   */ 
	0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,   /*   0xA0 to 0xAF   */ 
	0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,   /*   0xB0 to 0xBF   */ 
	0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,   /*   0xC0 to 0xCF   */ 
	0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,   /*   0xD0 to 0xDF   */ 
	0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,   /*   0xE0 to 0xEF   */ 
	0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u    /*   0xF0 to 0xFF   */ 
 }; 
#endif

u32 check_keyboard_flag(void)
{
	return IRQ_flag;
}
void clean_keyboard_flag(u32 flag)
{
	IRQ_flag &= flag;
}
	
void gpio_out(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, u32 out)
{
	if (out) {
		GPIO_SetBits(GPIOx, GPIO_Pin);
	}
	else {
		GPIO_ResetBits(GPIOx, GPIO_Pin);
	}
}

void EXTI0_IRQHandler(void)
{
	if (keyup_status) {
		keyup_status = 0;
		//change_EXTIX(FALLING);
		GPIO_PRINTF("KEY_UP Down\n");
	}
	else {
		keyup_status = 1;
		//change_EXTIX(RISING);
		GPIO_PRINTF("KEY_UP Up\n");
	}
	EXTI_ClearITPendingBit(EXTI_Line0); // 清除LINE10上的中断标志位
} 


