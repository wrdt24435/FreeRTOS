#ifndef __SPI_H
#define __SPI_H

#define W25Q16_SPI	1

void flash_spi_init(void);			 //初始化SPI1口
//void SPI1_SetSpeed(u8 SpeedSet); //设置SPI1速度   
u8 SPI_ReadWriteByte(u32 port, u8 TxData);//SPI1总线读写一个字节
		 
#endif

