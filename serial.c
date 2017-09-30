 //串口相关的头文件  
#include<stdio.h>      /*标准输入输出定义*/  
#include<stdlib.h>     /*标准函数库定义*/  
#include<unistd.h>     /*Unix 标准函数定义*/  
#include<sys/types.h>   
#include<sys/stat.h>     
#include<fcntl.h>      /*文件控制定义*/  
#include<termios.h>    /*PPSIX 终端控制定义*/  
#include<errno.h>      /*错误号定义*/  
#include<string.h>    
//宏定义  
#define FALSE  -1  
#define TRUE   0  

int main()  
{  
    int fd;                            //文件描述符  
    int err;                           //返回调用函数的状态  
    int len;                          
    char send_buf[8]={0};  
	
    send_buf[2]=0x12;
    send_buf[3]=0x34;
	
    struct termios options;  
    fd = open( "/dev/ttyUSB0", O_RDWR|O_NOCTTY|O_NDELAY); //打开串口，返回文件描述符  
    if  ( tcgetattr( fd,&options)  !=  0)  
       {  
          perror("SetupSerial 1");      
          return(FALSE);   
       }  
   cfsetospeed(&options,B115200);    
  //修改控制模式，保证程序不会占用串口  
    options.c_cflag |= CLOCAL;  
    //修改控制模式，使得能够从串口中读取输入数据  
    options.c_cflag |= CREAD;  
	
	//不使用流控制  
   options.c_cflag &= ~CRTSCTS;  
   
   //设置数据位，8位
    options.c_cflag |= CS8; 

	//无校验位
	 options.c_cflag &= ~PARENB;   
     options.c_iflag &= ~INPCK;     
	
	//一个停止位
	 options.c_cflag &= ~CSTOPB; 
	 
	 
	 //如果发生数据溢出，接收数据，但是不再读取 刷新收到的数据但是不读  
    tcflush(fd,TCIFLUSH);  
     
    //激活配置 (将修改后的termios数据设置到串口中）  
   int test= tcsetattr(fd,TCSANOW,&options) ; 
          

  printf("Set Port Exactly!\n");  
  for(int i=0;i<100; i++)	
  { write(fd,(send_buf),8);
	  sleep(1);
  }
  close(fd);                      
}  
