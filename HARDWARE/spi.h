#ifndef __SPI_H
#define __SPI_H
#include "sys.h"

#define W25Q16_SPI	SPI1

void flash_spi_init(void);			 //��ʼ��SPI1��
void SPI1_SetSpeed(u8 SpeedSet); //����SPI1�ٶ�   
u8 SPI1_ReadWriteByte(u8 TxData);//SPI1���߶�дһ���ֽ�
		 
#endif

