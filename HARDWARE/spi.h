#ifndef __SPI_H
#define __SPI_H

#define W25Q16_SPI	1

void flash_spi_init(void);			 //��ʼ��SPI1��
//void SPI1_SetSpeed(u8 SpeedSet); //����SPI1�ٶ�   
u8 SPI_ReadWriteByte(u32 port, u8 TxData);//SPI1���߶�дһ���ֽ�
		 
#endif

