//备注：10ms和100ms定时存在误差，与polling是否及时处理相关
//MS任务在tick定时器里插入，更精确
#include "common.h"
#include <string.h>
#include "clk_timer.h"
#include "stm32f4xx.h"
#include "keyboard_driver.h"	//polling有按键检测
#include "wwdg.h"		//polling里有喂狗

#if DEBUG
#define T_PRINTF PRINTF
#else
#define T_PRINTF
#endif


/*
 * m: VCO输入时钟 分频因子，取值2~63
 * n: VCO输出时钟 倍频因子，取值192~432
 * p: PLLCLK时钟分频因子  ，取值2，4，6，8
 * q: OTG FS,SDIO,RNG时钟分频因子，取值4~15
 * 函数调用举例，使用HSE设置时钟
 * SYSCLK=HCLK=168M,PCLK2=HCLK/2=84M,PCLK1=HCLK/4=42M
 * HSE_SetSysClock(8, 336, 2, 7);	//168M跑
 * HSE作为时钟来源，经过PLL倍频作为系统时钟，这是通常的做法
 */
void HSE_SetSysClock(uint32_t m, uint32_t n, uint32_t p, uint32_t q)	
{
  __IO uint32_t HSEStartUpStatus = 0;
  
  // 使能HSE，开启外部晶振，野火F407使用 HSE=25M
  RCC_HSEConfig(RCC_HSE_ON);
	
  // 等待HSE启动稳定
	HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if (HSEStartUpStatus == SUCCESS)
  {
    // 调压器电压输出级别配置为1，以便在器件为最大频率
		// 工作时使性能和功耗实现平衡
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    PWR->CR |= PWR_CR_VOS;
		
		// HCLK = SYSCLK / 1
		RCC_HCLKConfig(RCC_SYSCLK_Div1);
		
		// PCLK2 = HCLK / 2
		RCC_PCLK2Config(RCC_HCLK_Div2);
		
		// PCLK1 = HCLK / 4
		RCC_PCLK1Config(RCC_HCLK_Div4);
		
    // 如果要超频就得在这里下手啦
		// 设置PLL来源时钟，设置VCO分频因子m，设置VCO倍频因子n，
		//  设置系统时钟分频因子p，设置OTG FS,SDIO,RNG分频因子q
		RCC_PLLConfig(RCC_PLLSource_HSE, m, n, p, q);
		
		// 使能PLL
		RCC_PLLCmd(ENABLE);
  
	  // 等待 PLL稳定
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {
    }   

/*-----------------------------------------------------*/
    //开启 OVER-RIDE模式，以能达到更高频率(F429才有该功能)
//    PWR->CR |= PWR_CR_ODEN;
//    while((PWR->CSR & PWR_CSR_ODRDY) == 0)
//    {
//    }
//    PWR->CR |= PWR_CR_ODSWEN;
//    while((PWR->CSR & PWR_CSR_ODSWRDY) == 0)
//    {
//    }      
    // 配置FLASH预取指,指令缓存,数据缓存和等待状态
    FLASH->ACR = FLASH_ACR_PRFTEN 
		            | FLASH_ACR_ICEN 
		            | FLASH_ACR_DCEN 
		            | FLASH_ACR_LATENCY_5WS;
/*-----------------------------------------------------*/
		
		// 当PLL稳定之后，把PLL时钟切换为系统时钟SYSCLK
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    // 读取时钟切换状态位，确保PLLCLK被选为系统时钟
    while (RCC_GetSYSCLKSource() != 0x08)
    {
    }
  }
  else
  { // HSE启动出错处理

    while (1)
    {
    }
  }
}

void SysTick_init(void)
{
	 /*SystemFrequency / 1000    1ms中断一次
	 * SystemFrequency / 100000	 10us中断一次
	 * SystemFrequency / 1000000 1us中断一次
	 */
	if (SysTick_Config(SystemCoreClock / 1000))
	{ 
		while (1);
	}
}


void delay_us(u32 count)
{
	count *= 0xFF;
	while (count--);
}

extern void Mon_polling(void);

void polling(void)
{

}




