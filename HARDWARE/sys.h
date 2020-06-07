#ifndef __SYS_H
#define __SYS_H	 

//以下为汇编函数
void WFI_SET(void);		//执行WFI指令
void INTX_DISABLE(void);//关闭所有中断
void INTX_ENABLE(void);	//开启所有中断
void MSR_MSP(u32 addr);	//设置堆栈地址 
u8 RNG_Init(void);
void Sys_Enter_Standby(void);
u32 RNG_Get_RandomNum(void);

#endif











