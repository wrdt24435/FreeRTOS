#include "gpio.h"
#include "spi.h"

//以下是SPI模块的初始化代码，配置成主机模式 						  
//SPI口初始化
//这里针是对SPI1的初始化
void flash_spi_init(void)
{	 
	SPI_InitTypeDef  SPI_InitStructure;
	MY_IO *gpio_config = &temp_config;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);//使能SPI1时钟

	//GPIOFB3,4,5初始化设置
	gpio_config->port = W25Q16_SPI_PORT;
	gpio_config->sys.GPIO_Pin = W25Q16_SPI_PIN;//PB3~5复用功能输出	
	gpio_config->sys.GPIO_Mode = GPIO_Mode_AF;//复用功能
	gpio_config->sys.GPIO_OType = GPIO_OType_PP;//推挽输出
	gpio_config->sys.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	gpio_config->sys.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	gpio_init(gpio_config);//初始化

	GPIO_PinAFConfig(W25Q16_SPI_PORT,GPIO_PinSource3,GPIO_AF_SPI1); //PB3复用为 SPI1
	GPIO_PinAFConfig(W25Q16_SPI_PORT,GPIO_PinSource4,GPIO_AF_SPI1); //PB4复用为 SPI1
	GPIO_PinAFConfig(W25Q16_SPI_PORT,GPIO_PinSource5,GPIO_AF_SPI1); //PB5复用为 SPI1

	//这里只针对SPI口初始化
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,ENABLE);//复位SPI1
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,DISABLE);//停止复位SPI1

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//串行同步时钟的空闲状态为高电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//串行同步时钟的第二个跳变沿（上升或下降）数据被采样
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;		//定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	SPI_Init(W25Q16_SPI, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器

	SPI_Cmd(W25Q16_SPI, ENABLE); //使能SPI外设

	SPI1_ReadWriteByte(0xff);//启动传输		 
	SPI_Cmd(W25Q16_SPI,ENABLE); //使能SPI1
} 

//SPI1 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPI1_ReadWriteByte(u8 TxData)
{		 			 
 
	while (SPI_I2S_GetFlagStatus(W25Q16_SPI, SPI_I2S_FLAG_TXE) == RESET){}//等待发送区空  
	SPI_I2S_SendData(W25Q16_SPI, TxData); //通过外设SPIx发送一个byte  数据	
	while (SPI_I2S_GetFlagStatus(W25Q16_SPI, SPI_I2S_FLAG_RXNE) == RESET); //等待接收完一个byte  
	return SPI_I2S_ReceiveData(W25Q16_SPI); //返回通过SPIx最近接收的数据	
 		    
}








