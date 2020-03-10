#include "gpio.h"
#include "spi.h"

//������SPIģ��ĳ�ʼ�����룬���ó�����ģʽ 						  
//SPI�ڳ�ʼ��
//�������Ƕ�SPI1�ĳ�ʼ��
void flash_spi_init(void)
{	 
	SPI_InitTypeDef  SPI_InitStructure;
	MY_IO *gpio_config = &temp_config;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);//ʹ��SPI1ʱ��

	//GPIOFB3,4,5��ʼ������
	gpio_config->port = W25Q16_SPI_PORT;
	gpio_config->sys.GPIO_Pin = W25Q16_SPI_PIN;//PB3~5���ù������	
	gpio_config->sys.GPIO_Mode = GPIO_Mode_AF;//���ù���
	gpio_config->sys.GPIO_OType = GPIO_OType_PP;//�������
	gpio_config->sys.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	gpio_config->sys.GPIO_PuPd = GPIO_PuPd_UP;//����
	gpio_init(gpio_config);//��ʼ��

	GPIO_PinAFConfig(W25Q16_SPI_PORT,GPIO_PinSource3,GPIO_AF_SPI1); //PB3����Ϊ SPI1
	GPIO_PinAFConfig(W25Q16_SPI_PORT,GPIO_PinSource4,GPIO_AF_SPI1); //PB4����Ϊ SPI1
	GPIO_PinAFConfig(W25Q16_SPI_PORT,GPIO_PinSource5,GPIO_AF_SPI1); //PB5����Ϊ SPI1

	//����ֻ���SPI�ڳ�ʼ��
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,ENABLE);//��λSPI1
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,DISABLE);//ֹͣ��λSPI1

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
	SPI_Init(W25Q16_SPI, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���

	SPI_Cmd(W25Q16_SPI, ENABLE); //ʹ��SPI����

	SPI1_ReadWriteByte(0xff);//��������		 
	SPI_Cmd(W25Q16_SPI,ENABLE); //ʹ��SPI1
} 

//SPI1 ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
u8 SPI1_ReadWriteByte(u8 TxData)
{		 			 
 
	while (SPI_I2S_GetFlagStatus(W25Q16_SPI, SPI_I2S_FLAG_TXE) == RESET){}//�ȴ���������  
	SPI_I2S_SendData(W25Q16_SPI, TxData); //ͨ������SPIx����һ��byte  ����	
	while (SPI_I2S_GetFlagStatus(W25Q16_SPI, SPI_I2S_FLAG_RXNE) == RESET); //�ȴ�������һ��byte  
	return SPI_I2S_ReceiveData(W25Q16_SPI); //����ͨ��SPIx������յ�����	
 		    
}








