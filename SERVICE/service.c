/*	ϵͳ���: IC����HARDWARE�����蹦��driver: ��������:service
*	����HARDWARE����������ֲ���øĶ���service�ֿ�if(interface)��Ϊ��
*	���ֽӿںͻ�������ִ�й��̣������޸ġ�
*/
#include "common.h" 
#include "Service.h" 

#if DEBUG
#define SER_PRINTF PRINTF
#else
#define SER_PRINTF
#endif

//int led_service(SERVICE_PACKET *sp);
//int rtc_service(SERVICE_PACKET *sp);
//int flash_service(SERVICE_PACKET *sp);
service service_core[TYPE_MAX] = {		//��TYPE_MAX��Ӧ
	NULL,
};




void Service_init(void)
{
}

int submit_service(SERVICE_PACKET *sp)
{
	if(service_core[sp->type]) {
		return service_core[sp->type](sp);
	}
	else {
		ERR;
	}
	return -1;
}




