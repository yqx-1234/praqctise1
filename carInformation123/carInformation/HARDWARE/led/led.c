/*-------------------------------------------------*/
/*            超纬电子STM32系列开发板              */
/*-------------------------------------------------*/
/*                                                 */
/*              实现LED功能的源文件                */
/*                                                 */
/*-------------------------------------------------*/

#include "stm32f4xx.h"  //包含需要的头文件
#include "led.h"        //包含需要的头文件

/*-------------------------------------------------*/
/*函数名：初始化LED函数                            */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void LED_Init(void)
{    	 
	GPIO_InitTypeDef GPIO_InitStructure;                     //定义一个设置GPIO的变量
	
	  RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOB, ENABLE);   //使能GPIOB端口时钟
    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOC, ENABLE);   //使能GPIOC端口时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;   //设置PB0 PB1
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;       //IO速率50Mhz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   		     //输出模式
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;             //推挽模式
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;               //上拉电阻
	
	GPIO_Init(GPIOB, &GPIO_InitStructure);            		 //设置PB0 PB1
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;   //设置PC4 PC5
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;       //IO速率50Mhz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   		     //输出模式
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;             //推挽模式
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;               //上拉电阻
	GPIO_Init(GPIOC, &GPIO_InitStructure);            		 //设置PC4 PC5
	
	LED_AllOff();                                            //所有LED熄灭
}

/*-------------------------------------------------*/
/*函数名：点亮所有LED                              */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void LED_AllOn(void)
{
	LED1_ON;
	LED2_ON;
	LED3_ON;
	LED4_ON;
}

/*-------------------------------------------------*/
/*函数名：熄灭所有LED                              */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void LED_AllOff(void)
{
	LED1_OFF;
	LED2_OFF;
	LED3_OFF;
	LED4_OFF;
}



