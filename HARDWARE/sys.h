#ifndef __SYS_H
#define __SYS_H	 

//����Ϊ��ຯ��
void WFI_SET(void);		//ִ��WFIָ��
void INTX_DISABLE(void);//�ر������ж�
void INTX_ENABLE(void);	//���������ж�
void MSR_MSP(u32 addr);	//���ö�ջ��ַ 
u8 RNG_Init(void);
void Sys_Enter_Standby(void);
u32 RNG_Get_RandomNum(void);

#endif











