/*-------------------------------------------------*/
/*            ��γ����STM32ϵ�п�����              */
/*-------------------------------------------------*/
/*                                                 */
/*              ʵ��LED���ܵ�ͷ�ļ�                */
/*                                                 */
/*-------------------------------------------------*/

#ifndef __LED_H
#define __LED_H

#define LED1_OUT(x)   GPIO_WriteBit(GPIOB, GPIO_Pin_1, (BitAction)x)  //PB1����LED1������PB1�ĵ�ƽ�����Ե���Ϩ��LED1
#define LED2_OUT(x)   GPIO_WriteBit(GPIOB, GPIO_Pin_0, (BitAction)x)  //PB0����LED2������PB0�ĵ�ƽ�����Ե���Ϩ��LED2
#define LED3_OUT(x)   GPIO_WriteBit(GPIOC, GPIO_Pin_5, (BitAction)x)  //PC5����LED3������PC5�ĵ�ƽ�����Ե���Ϩ��LED3
#define LED4_OUT(x)   GPIO_WriteBit(GPIOC, GPIO_Pin_4, (BitAction)x)  //PC4����LED4������PC4�ĵ�ƽ�����Ե���Ϩ��LED4

#define LED1_IN_STA   GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_1) //PB1����LED1,��ȡ��ƽ״̬�������ж�LED1�ǵ�������Ϩ��
#define LED2_IN_STA   GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_0) //PB0����LED2,��ȡ��ƽ״̬�������ж�LED2�ǵ�������Ϩ��
#define LED3_IN_STA   GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_5) //PC5����LED3,��ȡ��ƽ״̬�������ж�LED3�ǵ�������Ϩ��
#define LED4_IN_STA   GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_4) //PC4����LED4,��ȡ��ƽ״̬�������ж�LED4�ǵ�������Ϩ��

#define LED1_ON       GPIO_ResetBits(GPIOB, GPIO_Pin_1)         //PB1����LED1��������������PB1��ƽ������LED1
#define LED1_OFF      GPIO_SetBits(GPIOB, GPIO_Pin_1)           //PB1����LED1��������������PB1��ƽ��Ϩ��LED1
 
#define LED2_ON       GPIO_ResetBits(GPIOB, GPIO_Pin_0)         //PB0����LED2��������������PB0��ƽ������LED2
#define LED2_OFF      GPIO_SetBits(GPIOB, GPIO_Pin_0)           //PB0����LED2��������������PB0��ƽ��Ϩ��LED2

#define LED3_ON       GPIO_ResetBits(GPIOC, GPIO_Pin_5)         //PC5����LED3��������������PC5��ƽ������LED3
#define LED3_OFF      GPIO_SetBits(GPIOC, GPIO_Pin_5)           //PC5����LED3��������������PC5��ƽ��Ϩ��LED3

#define LED4_ON       GPIO_ResetBits(GPIOC, GPIO_Pin_4)         //PC4����LED4��������������PC4��ƽ������LED4
#define LED4_OFF      GPIO_SetBits(GPIOC, GPIO_Pin_4)           //PC4����LED4��������������PC4��ƽ��Ϩ��LED4

void LED_Init(void);               //��ʼ��	
void LED_AllOn(void);              //��������LED
void LED_AllOff(void);             //Ϩ������LED

#endif
