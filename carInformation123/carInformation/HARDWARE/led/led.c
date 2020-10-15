/*-------------------------------------------------*/
/*            ��γ����STM32ϵ�п�����              */
/*-------------------------------------------------*/
/*                                                 */
/*              ʵ��LED���ܵ�Դ�ļ�                */
/*                                                 */
/*-------------------------------------------------*/

#include "stm32f4xx.h"  //������Ҫ��ͷ�ļ�
#include "led.h"        //������Ҫ��ͷ�ļ�

/*-------------------------------------------------*/
/*����������ʼ��LED����                            */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void LED_Init(void)
{    	 
	GPIO_InitTypeDef GPIO_InitStructure;                     //����һ������GPIO�ı���
	
	  RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOB, ENABLE);   //ʹ��GPIOB�˿�ʱ��
    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOC, ENABLE);   //ʹ��GPIOC�˿�ʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;   //����PB0 PB1
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;       //IO����50Mhz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   		     //���ģʽ
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;             //����ģʽ
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;               //��������
	
	GPIO_Init(GPIOB, &GPIO_InitStructure);            		 //����PB0 PB1
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;   //����PC4 PC5
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;       //IO����50Mhz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   		     //���ģʽ
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;             //����ģʽ
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;               //��������
	GPIO_Init(GPIOC, &GPIO_InitStructure);            		 //����PC4 PC5
	
	LED_AllOff();                                            //����LEDϨ��
}

/*-------------------------------------------------*/
/*����������������LED                              */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void LED_AllOn(void)
{
	LED1_ON;
	LED2_ON;
	LED3_ON;
	LED4_ON;
}

/*-------------------------------------------------*/
/*��������Ϩ������LED                              */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void LED_AllOff(void)
{
	LED1_OFF;
	LED2_OFF;
	LED3_OFF;
	LED4_OFF;
}



