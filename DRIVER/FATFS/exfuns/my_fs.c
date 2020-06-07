#include "common.h"	
#include "string.h"
#include "my_fs.h"
#include "ff.h"

u8 fsbuf[60];
static u8 buf[_MAX_LFN * 2 + 1];

//获取盘符（磁盘名字）
//path:磁盘路径，比如"0:"、"1:"  
void mf_getlabel(u8 *path)
{
	u8 buf[20];
	u32 sn=0;
	u8 res;
	res=f_getlabel((const TCHAR *)path,(TCHAR *)buf,(DWORD*)&sn);
	if(res==FR_OK)
	{
		PRINTF("\r\n disk syb:%s\r\n",buf);
		PRINTF("disk ID:%X\r\n\r\n",sn); 
	}else 
		PRINTF("\r\nGetlabelERR:%X\r\n",res);
}

u8 mf_scan_files(u8 * path, FILINFO* filinfo_t)
{
	FRESULT res;	  
    char *fn;   /* This function is assuming non-Unicode cfg. */
	DIR dir;
#if _USE_LFN
 	filinfo_t->lfsize = _MAX_LFN * 2 + 1;
	filinfo_t->lfname = buf;
#endif		  
    res = f_opendir(&dir,(const TCHAR*)path); //打开一个目录
    if (res == FR_OK) 
	{	
		printf("opendir OK\r\n"); 
		while(1)
		{
	        res = f_readdir(&dir, filinfo_t);                   //读取目录下的一个文件
	        if (res != FR_OK || filinfo_t->fname[0] == 0) break;  //错误了/到末尾了,退出
	        //if (filinfo_t->fname[0] == '.') continue;             //忽略上级目录
#if _USE_LFN
        	fn = *filinfo_t->lfname ? filinfo_t->lfname : filinfo_t->fname;
#else							   
        	fn = filinfo_t->fname;
#endif	                                              /* It is a file. */
			printf("%s/%s\r\n", path, fn);//打印路径、文件名	  
		} 
    }	  
    return res;	  
}

FIL file;
char str[] = "abc777abc777";
void fs_test(void)
{
	int ret;
	FATFS fs;
	FATFS *p_fs;
	FILINFO fileinfo;
	//u32 u32_ret;
	
	//sd_cart_test();
	ret = f_mount(&fs,"0:",1);
	if (ret) {
		ERR;
	}
	/*res=f_mount(fs[1],"1:",1); 				//挂载FLASH.	
	if(res==0X0D)//FLASH磁盘,FAT文件系统错误,重新格式化FLASH
	{
		LCD_ShowString(30,150,200,16,16,"Flash Disk Formatting...");	//格式化FLASH
		res=f_mkfs("1:",1,4096);//格式化FLASH,1,盘符;1,不需要引导区,8个扇区为1个簇
		if(res==0)
		{
			f_setlabel((const TCHAR *)"1:ALIENTEK");	//设置Flash磁盘的名字为：ALIENTEK
			LCD_ShowString(30,150,200,16,16,"Flash Disk Format Finish");	//格式化完成
		}else LCD_ShowString(30,150,200,16,16,"Flash Disk Format Error ");	//格式化失败
		delay_ms(1000);
	}*/
	u32 get_free,total,free;
	ret = f_getfree("0",(DWORD*)&get_free,&p_fs);
	if (ret) 
		return;
	total = (p_fs->n_fatent - 2) * p_fs->csize;
	free = get_free * p_fs->csize;
	PRINTF("total: %dKB  free: %dKB\r\n",total/2, free/2);
	//mf_getlabel("0:");
	mf_scan_files("0:", &fileinfo);

	if (f_open(&file,(const TCHAR*)"abc.txt",FA_READ | FA_WRITE)) {	//FA_CREATE_NEW
		ERR;
	}
	
	/*if (f_write(&file, str, sizeof(str), &ret)) {
		ERR;
	}*/
	if (f_read(&file, fsbuf, 12, &ret)) {
		ERR;
	}
	PRINTF("read num: %d  %s\r\n", ret, fsbuf);
	f_close(&file);

}


	


