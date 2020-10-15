/*-------------------------------------------------*/
/*            超纬电子STM32系列开发板              */
/*-------------------------------------------------*/
/*                                                 */
/*            操作602Wifi功能的源文件              */
/*                                                 */
/*-------------------------------------------------*/

#include "stm32f4xx.h"  //包含需要的头文件
#include "fourG.h"	    //包含需要的头文件
//#include "iic.h"        //包含需要的头文件
//#include "24c02.h" 		//包含需要的头文件
#include "delay.h"	    //包含需要的头文件
#include "usart1.h"	    //包含需要的头文件
#include "led.h"        //包含需要的头文件
#include "mqtt.h"       //包含需要的头文件
#include "key.h"        //包含需要的头文件

char fourG_mode = 0;     //联网模式 0：SSID和密码写在程序里   1：Smartconfig方式用APP发送
	
/*-------------------------------------------------*/
/*函数名：初始化WiFi的复位IO                       */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void fourG_ResetIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;                      //定义一个设置IO端口参数的结构体
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);   //使能PA端口时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;                 //准备设置PA4
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;         //速率100Mhz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   	//普通输出方式
	GPIO_InitStructure.GPIO_OType =GPIO_OType_PP;     //推挽输出
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;        //上拉电阻模式
	GPIO_Init(GPIOA, &GPIO_InitStructure);            		  //设置PA4
	RESET_IO(1);                                              //复位IO拉高电平
}
/*-------------------------------------------------*/
/*函数名：WiFi发送设置指令                         */
/*参  数：cmd：指令                                */
/*参  数：timeout：超时时间（100ms的倍数）         */
/*返回值：0：正确   其他：错误                     */
/*-------------------------------------------------*/
char fourG_SendCmd(char *cmd, int timeout)
{
	fourG_RxCounter=0;                           //WiFi接收数据量变量清零                        
	memset(fourG_RX_BUF,0,fourG_RXBUFF_SIZE);     //清空WiFi接收缓冲区 
	fourG_printf("%s\r\n",cmd);                  //发送指令
	while(timeout--){                           //等待超时时间到0
		delay_ms(100);                          //延时100ms
		if(strstr(fourG_RX_BUF,"OK"))            //如果接收到OK表示指令成功
			break;       						//主动跳出while循环
		u1_printf("%d ",timeout);               //串口输出现在的超时时间
	}
	u1_printf("\r\n");                          //串口输出信息
	if(timeout<=0)return 1;                     //如果timeout<=0，说明超时时间到了，也没能收到OK，返回1
	else return 0;		         				//反之，表示正确，说明收到OK，通过break主动跳出while
}
/*-------------------------------------------------*/
/*函数名：fourG_Reset                               */
/*函数作用：4G模块重启                               */
/*参  数：timeout：超时时间（100ms的倍数）         */
/*返回值：0：正确   其他：错误                     */
/*-------------------------------------------------*/
char fourG_Reset(int timeout)
{
	//RESET_IO(0);                                    //复位IO拉低电平
	delay_ms(500);                                    //延时500ms
  fourG_printf("AT+Z\r\n");                          //发送指令		
	//RESET_IO(1);                                    //复位IO拉高电平	
	while(timeout--){                               //等待超时时间到0
		delay_ms(1000);                              //延时100ms
		if(strstr(fourG_RX_BUF,"G405tf"))             //如果接收到G405tf表示复位成功
			break;       						    //主动跳出while循环
		u1_printf("%d ",timeout);                   //串口输出现在的超时时间
	}
	u1_printf("\r\n");                              //串口输出信息
	if(timeout<=0)return 1;                         //如果timeout<=0，说明超时时间到了，也没能收到ready，返回1
	else return 0;		         				    //反之，表示正确，说明收到ready，通过break主动跳出while
}

/*-------------------------------------------------*/
/*函数名：fourG_callMode*/
/*函数作用:4G模块进入透传模式                      */
/*参  数：timeout：超时时间（1s的倍数）            */
/*返回值：0：正确   其他：错误                     */
/*-------------------------------------------------*/
char fourG_callMode(int timeout)
{		
	fourG_RxCounter=0;                               //WiFi接收数据量变量清零                        
	memset(fourG_RX_BUF,0,fourG_RXBUFF_SIZE);         //清空WiFi接收缓冲区 
	fourG_printf("AT+ENTM\r\n"); //发送指令	
	while(timeout--){                               //等待超时时间到0
		delay_ms(1000);                             //延时1s
		if(strstr(fourG_RX_BUF,"OK")) //如果接收到WIFI GOT IP表示成功
			break;       						    //主动跳出while循环
		u1_printf("%d ",timeout);                   //串口输出现在的超时时间
	}
	u1_printf("\r\n");                              //串口输出信息
	if(timeout<=0)return 1;                         //如果timeout<=0，说明超时时间到了，也没能收到WIFI GOT IP，返回1
	return 0;                                       //正确，返回0
}

/*-------------------------------------------------*/
/*函数名：连接TCP服务器，并进入透传模式            */
/*参  数：timeout： 超时时间（100ms的倍数）        */
/*返回值：0：正确  其他：错误                      */
/*-------------------------------------------------*/
char fourG_Connect_Server(int timeout)
{	
	char a=0;
	fourG_RxCounter=0;                               //WiFi接收数据量变量清零                        
	memset(fourG_RX_BUF,0,fourG_RXBUFF_SIZE);         //清空WiFi接收缓冲区   
	fourG_printf("AT+SOCKA=TCP,%s,%d\r\n",ServerIP,ServerPort);//发送连接服务器指令
	while(timeout--){                               //等待超时与否
		delay_ms(100);                              //延时100ms	
		if(strstr(fourG_RX_BUF ,"aliyuncs"))          //如果接受到CONNECT表示连接成功
			break; 		//跳出while循环
		/*
		if(strstr(fourG_RX_BUF ,"CLOSED"))           //如果接受到CLOSED表示服务器未开启
			return 1;                               //服务器未开启返回1
		if(strstr(fourG_RX_BUF ,"ALREADY CONNECTED"))//如果接受到ALREADY CONNECTED已经建立连接
			return 2;                               //已经建立连接返回2
		*/
		u1_printf("%d ",timeout);                   //串口输出现在的超时时间  
	}
	u1_printf("\r\n");                        //串口输出信息
	if(timeout<=0)return 1;                   //超时错误，返回1
	else
	  return 0;
	/*
	else                                      //连接成功，准备进入透传
	{
		u1_printf("连接服务器成功，准备进入透传\r\n");  //串口显示信息
		a=fourG_callMode(2);
		
		fourG_RxCounter=0;                               //WiFi接收数据量变量清零                        
		memset(fourG_RX_BUF,0,fourG_RXBUFF_SIZE);         //清空WiFi接收缓冲区     
		fourG_printf("AT+CIPSEND\r\n");                  //发送进入透传指令
		while(timeout--){                               //等待超时与否
			delay_ms(100);                              //延时100ms	
			if(strstr(fourG_RX_BUF,"\r\nOK\r\n\r\n>"))   //如果成立表示进入透传成功
				break;                          //跳出while循环
			u1_printf("%d ",timeout);           //串口输出现在的超时时间  
		}
		if(timeout<=0)return 4;                 //透传超时错误，返回4	
		
	}
	if(a==0)
	  return 0;	                                //成功返回0	
	*/

}
/*-------------------------------------------------*/
/*函数名：fourG_commandMode                      */
/*参  数：timeout           */
/*返回值：0：正确   其他：错误     */
/*功能：进入AT指令模式*/
/*-------------------------------------------------*/
char fourG_commandMode(int timeout)
{
	
	fourG_RxCounter=0;                           //4G接收数据量变量清零                        
	memset(fourG_RX_BUF,0,fourG_RXBUFF_SIZE);     //清空4G接收缓冲区     
	while(timeout--){		//等待超时时间到0
		fourG_printf("+++");
		delay_ms(1000); 		//延时1s
		fourG_printf("a");
		delay_ms(1000); 		//延时1s
		if(strstr(fourG_RX_BUF,"ok"))     //如果串口接受到ok表示成功进入AT指令模式
			break;                              //跳出while循环  
		u1_printf("%d ",timeout);               //串口输出现在的超时时间  
	}	
	u1_printf("\r\n");                          //串口输出信息
	if(timeout<=0)return 1;                     //超时错误，返回1
	return 0;                                   //正确返回0
}

/*-------------------------------------------------*/
/*函数名：等待加入路由器                           */
/*参  数：timeout：超时时间（1s的倍数）            */
/*返回值：0：正确   其他：错误                     */
/*-------------------------------------------------*/
/*
char WiFi_WaitAP(int timeout)
{		
	while(timeout--){                               //等待超时时间到0
		delay_ms(1000);                             //延时1s
		if(strstr(WiFi_RX_BUF,"WIFI GOT IP"))       //如果接收到WIFI GOT IP表示成功
			break;       						    //主动跳出while循环
		u1_printf("%d ",timeout);                   //串口输出现在的超时时间
	}
	u1_printf("\r\n");                              //串口输出信息
	if(timeout<=0)return 1;                         //如果timeout<=0，说明超时时间到了，也没能收到WIFI GOT IP，返回1
	return 0;                                       //正确，返回0
}
*/
/*-------------------------------------------------*/
/*函数名：4G连接服务器                           */
/*参  数：无                                       */
/*返回值：0：正确   其他：错误                     */
/*-------------------------------------------------*/
char fourG_Connect_IoTServer(void)
{
	u1_printf("准备进入AT指令模式\r\n");	//串口提示数据
	if(fourG_commandMode(50)){                                //复位，100ms超时单位，总计5s超时时间
		u1_printf("进入AT指令模式失败，准备重启\r\n");           //返回非0值，进入if，串口提示数据
		return 1;                                      //返回1
	}else u1_printf("成功进入AT指令模式\r\n");                   //串口提示数据
	u1_printf("准备复位模块\r\n");	//串口提示数据
	if(fourG_Reset(50)){                                //复位，100ms超时单位，总计5s超时时间
		u1_printf("复位失败，准备重启\r\n");           //返回非0值，进入if，串口提示数据
		return 2;                                      //返回1
	}else u1_printf("复位成功\r\n");                   //串口提示数据
	u1_printf("准备进入AT指令模式\r\n");	//串口提示数据
	if(fourG_commandMode(50)){                                //复位，100ms超时单位，总计5s超时时间
		u1_printf("进入AT指令模式失败，准备重启\r\n");           //返回非0值，进入if，串口提示数据
		return 3;                                      //返回1
	}else u1_printf("成功进入AT指令模式\r\n");                   //串口提示数据
	u1_printf("准备设置TCP连接\r\n");                  //串口提示数据
	if(fourG_Connect_Server(50)){                //设置STA模式，100ms超时单位，总计5s超时时间
		u1_printf("设置TCP连接失败，准备重启\r\n");    //返回非0值，进入if，串口提示数据
		return 4;                                      //返回2
	}else u1_printf("设置TCP连接成功\r\n");            //串口提示数据
	/*
	if(wifi_mode==0){                                      //如果联网模式=0：SSID和密码写在程序里 
		u1_printf("准备取消自动连接\r\n");                 //串口提示数据
		if(WiFi_SendCmd("AT+CWAUTOCONN=0",50)){            //取消自动连接，100ms超时单位，总计5s超时时间
			u1_printf("取消自动连接失败，准备重启\r\n");   //返回非0值，进入if，串口提示数据
			return 3;                                      //返回3
		}else u1_printf("取消自动连接成功\r\n");           //串口提示数据
				
		u1_printf("准备连接路由器\r\n");                   //串口提示数据	
		if(WiFi_JoinAP(30)){                               //连接路由器,1s超时单位，总计30s超时时间
			u1_printf("连接路由器失败，准备重启\r\n");     //返回非0值，进入if，串口提示数据
			return 4;                                      //返回4	
		}else u1_printf("连接路由器成功\r\n");             //串口提示数据			
	}else{                                                 //如果联网模式=1：Smartconfig方式,用APP发送
		if(KEY2_IN_STA==0){                                    //如果此时K2是按下的
			u1_printf("准备设置自动连接\r\n");                 //串口提示数据
			if(WiFi_SendCmd("AT+CWAUTOCONN=1",50)){            //设置自动连接，100ms超时单位，总计5s超时时间
				u1_printf("设置自动连接失败，准备重启\r\n");   //返回非0值，进入if，串口提示数据
				return 3;                                      //返回3
			}else u1_printf("设置自动连接成功\r\n");           //串口提示数据	
			
			u1_printf("准备开启Smartconfig\r\n");              //串口提示数据
			if(WiFi_SendCmd("AT+CWSTARTSMART",50)){            //开启Smartconfig，100ms超时单位，总计5s超时时间
				u1_printf("开启Smartconfig失败，准备重启\r\n");//返回非0值，进入if，串口提示数据
				return 4;                                      //返回4
			}else u1_printf("开启Smartconfig成功\r\n");        //串口提示数据

			u1_printf("请使用APP软件传输密码\r\n");            //串口提示数据
			if(WiFi_Smartconfig(60)){                          //APP软件传输密码，1s超时单位，总计60s超时时间
				u1_printf("传输密码失败，准备重启\r\n");       //返回非0值，进入if，串口提示数据
				return 5;                                      //返回5
			}else u1_printf("传输密码成功\r\n");               //串口提示数据

			u1_printf("准备关闭Smartconfig\r\n");              //串口提示数据
			if(WiFi_SendCmd("AT+CWSTOPSMART",50)){             //关闭Smartconfig，100ms超时单位，总计5s超时时间
				u1_printf("关闭Smartconfig失败，准备重启\r\n");//返回非0值，进入if，串口提示数据
				return 6;                                      //返回6
			}else u1_printf("关闭Smartconfig成功\r\n");        //串口提示数据
		}else{                                                 //反之，此时K2是没有按下
			u1_printf("等待连接路由器\r\n");                   //串口提示数据	
			if(WiFi_WaitAP(30)){                               //等待连接路由器,1s超时单位，总计30s超时时间
				u1_printf("连接路由器失败，准备重启\r\n");     //返回非0值，进入if，串口提示数据
				return 7;                                      //返回7	
			}else u1_printf("连接路由器成功\r\n");             //串口提示数据					
		}
	}
	*/
	u1_printf("准备重启模块，进入透传模式\r\n");                     //串口提示数据
	if(fourG_Reset(50)){               //设置透传，100ms超时单位，总计5s超时时间
		u1_printf("设置透传失败，准备重启\r\n");       //返回非0值，进入if，串口提示数据
		return 5;                                      //返回8
	}else u1_printf("设置透传成功\r\n");               //串口提示数据
	/*
	u1_printf("准备关闭多路连接\r\n");                 //串口提示数据
	if(WiFi_SendCmd("AT+CIPMUX=0",50)){                //关闭多路连接，100ms超时单位，总计5s超时时间
		u1_printf("关闭多路连接失败，准备重启\r\n");   //返回非0值，进入if，串口提示数据
		return 9;                                      //返回9
	}else u1_printf("关闭多路连接成功\r\n");           //串口提示数据
	
	u1_printf("准备连接服务器\r\n");                   //串口提示数据
	if(WiFi_Connect_Server(100)){                      //连接服务器，100ms超时单位，总计10s超时时间
		u1_printf("连接服务器失败，准备重启\r\n");     //返回非0值，进入if，串口提示数据
		return 10;                                     //返回10
	}else u1_printf("连接服务器成功\r\n");             //串口提示数据	
	*/
	return 0;                                          //正确返回0
	
}
