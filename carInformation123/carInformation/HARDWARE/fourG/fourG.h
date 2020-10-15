/*-------------------------------------------------*/
/*            超纬电子STM32系列开发板              */
/*-------------------------------------------------*/
/*                                                 */
/*              操作Wifi功能的头文件               */
/*                                                 */
/*-------------------------------------------------*/

#ifndef __FOURG_H
#define __FOURG_H

#include "usart2.h"	    //包含需要的头文件

#define RESET_IO(x)    GPIO_WriteBit(GPIOA, GPIO_Pin_4, (BitAction)x)  //PA4控制WiFi的复位

#define fourG_printf       u2_printf           //串口2控制 WiFi
#define fourG_RxCounter    Usart2_RxCounter    //串口2控制 WiFi
#define fourG_RX_BUF       Usart2_RxBuff       //串口2控制 WiFi
#define fourG_RXBUFF_SIZE  USART2_RXBUFF_SIZE  //串口2控制 WiFi

//#define SSID   "userCWDZ"                     //路由器SSID名称
//#define PASS   "wc19871103.."                 //路由器密码

void fourG_ResetIO_Init(void);
char fourG_SendCmd(char *cmd, int timeout);
char fourG_Reset(int timeout);
char fourG_JoinAP(int timeout);
char fourG_Connect_Server(int timeout);
char fourG_commandMode(int timeout);
char fourG_WaitAP(int timeout);
char fourG_Connect_IoTServer(void);


#endif


