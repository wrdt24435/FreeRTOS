#include "common.h"	 
#include "cti2c.h"
#include "gpio.h"

//IO��������		GPIOB->MODER|=(u32)0<<(2*15);
#define CT_SDA_IN()  {GPIOB->MODER&=~((u32)3<<(2*15));}	//PF11����ģʽ
#define CT_SDA_OUT() {GPIOB->MODER&=~((u32)3<<(2*15));GPIOB->MODER|=((u32)1<<(2*15));} 	//PF11���ģʽ
//IO��������	 
#define CT_IIC_SCL(x)   io_set_out(TOUCH_SCL, x) 	//SCL
#define CT_IIC_SDA(x)   io_set_out(TOUCH_SDA, x) //SDA	 
#define CT_READ_SDA		io_read(TOUCH_SDA)  //����SDA 

//����I2C�ٶȵ���ʱ
void CT_Delay(void)
{
	delay_us(2);
} 
//���ݴ���оƬIIC�ӿڳ�ʼ��
void CT_IIC_Init(void)
{			
	MY_IO *gpio_config = &temp_config;
	gpio_config->port = TOUCH_SCL_PORT;
	gpio_config->sys.GPIO_Pin = TOUCH_SCL_PIN | TOUCH_SDA_PIN;
	gpio_config->sys.GPIO_Mode = GPIO_Mode_OUT;
	gpio_config->sys.GPIO_OType = GPIO_OType_PP;//�������
	gpio_config->sys.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	gpio_config->sys.GPIO_PuPd = GPIO_PuPd_UP;//����
	gpio_init(gpio_config);//��ʼ��
	

}
//����IIC��ʼ�ź�
void CT_IIC_Start(void)
{
	CT_SDA_OUT();     //sda�����
	CT_IIC_SDA(1);	  	  
	CT_IIC_SCL(1);
	delay_us(30);
 	CT_IIC_SDA(0);//START:when CLK is high,DATA change form high to low 
	CT_Delay();
	CT_IIC_SCL(0);//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void CT_IIC_Stop(void)
{
	CT_SDA_OUT();//sda�����
	CT_IIC_SCL(1);
	delay_us(30);
	CT_IIC_SDA(0);//STOP:when CLK is high DATA change form low to high
	CT_Delay();
	CT_IIC_SDA(1);//����I2C���߽����ź�  
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 CT_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	CT_SDA_IN();      //SDA����Ϊ����  
	CT_IIC_SDA(1);	   
	CT_IIC_SCL(1);
	CT_Delay();
	while(CT_READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			CT_IIC_Stop();
			return 1;
		} 
		CT_Delay();
	}
	CT_IIC_SCL(0);//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
void CT_IIC_Ack(void)
{
	CT_IIC_SCL(0);
	CT_SDA_OUT();
	CT_Delay();
	CT_IIC_SDA(0);
	CT_Delay();
	CT_IIC_SCL(1);
	CT_Delay();
	CT_IIC_SCL(0);
}
//������ACKӦ��		    
void CT_IIC_NAck(void)
{
	CT_IIC_SCL(0);
	CT_SDA_OUT();
	CT_Delay();
	CT_IIC_SDA(1);
	CT_Delay();
	CT_IIC_SCL(1);
	CT_Delay();
	CT_IIC_SCL(0);
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void CT_IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	CT_SDA_OUT(); 	    
    CT_IIC_SCL(0);//����ʱ�ӿ�ʼ���ݴ���
	CT_Delay();
	for(t=0;t<8;t++)
    {              
        CT_IIC_SDA((txd&0x80)>>7);
        txd<<=1; 	      
		CT_IIC_SCL(1); 
		CT_Delay();
		CT_IIC_SCL(0);	
		CT_Delay();
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 CT_IIC_Read_Byte(unsigned char ack)
{
	u8 i,receive=0;
 	CT_SDA_IN();//SDA����Ϊ����
	delay_us(30);
	for(i=0;i<8;i++ )
	{ 
		CT_IIC_SCL(0); 	    	   
		CT_Delay();
		CT_IIC_SCL(1);	 
		receive<<=1;
		if(CT_READ_SDA)receive++;   
	}	  				 
	if (!ack)CT_IIC_NAck();//����nACK
	else CT_IIC_Ack(); //����ACK   
 	return receive;
}





























