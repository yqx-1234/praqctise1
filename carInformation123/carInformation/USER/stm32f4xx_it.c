/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    04-August-2014
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
 

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/
#include "stm32f4xx.h"     //������Ҫ��ͷ�ļ�
#include "stm32f4xx_it.h"  //������Ҫ��ͷ�ļ�
#include "main.h"          //������Ҫ��ͷ�ļ�
#include "delay.h"         //������Ҫ��ͷ�ļ�
#include "usart1.h"        //������Ҫ��ͷ�ļ�
#include "usart2.h"        //������Ҫ��ͷ�ļ�
//#include "timer1.h"        //������Ҫ��ͷ�ļ�
#include "timer4.h"        //������Ҫ��ͷ�ļ�
#include "fourG.h"	       //������Ҫ��ͷ�ļ�
#include "timer3.h"        //������Ҫ��ͷ�ļ�
#include "led.h"           //������Ҫ��ͷ�ļ�
#include "mqtt.h"          //������Ҫ��ͷ�ļ�

/*-------------------------------------------------*/
/*������������2�����жϺ���                        */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void USART2_IRQHandler(void)   
{                      
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET){  //���USART_IT_RXNE��־��λ����ʾ�����ݵ��ˣ�����if��֧
		if(Connect_flag==0){                                //���Connect_flag����0����ǰ��û�����ӷ�����������ָ������״̬
			if(USART2->DR){                                 //����ָ������״̬ʱ������ֵ�ű��浽������	
				Usart2_RxBuff[Usart2_RxCounter]=USART2->DR; //���浽������	
				Usart2_RxCounter ++;                        //ÿ����1���ֽڵ����ݣ�Usart2_RxCounter��1����ʾ���յ���������+1 
			}		
		}else{		                                        //��֮Connect_flag����1�������Ϸ�������	
			Usart2_RxBuff[Usart2_RxCounter] = USART2->DR;   //�ѽ��յ������ݱ��浽Usart2_RxBuff��				
			if(Usart2_RxCounter == 0){    					//���Usart2_RxCounter����0����ʾ�ǽ��յĵ�1�����ݣ�����if��֧				
				TIM_Cmd(TIM4,ENABLE); 
			}else{                        					//else��֧����ʾ��Usart2_RxCounter������0�����ǽ��յĵ�һ������
				TIM_SetCounter(TIM4,0);  
			}	
			Usart2_RxCounter ++;         				    //ÿ����1���ֽڵ����ݣ�Usart2_RxCounter��1����ʾ���յ���������+1 
		}
	}
} 
/*-------------------------------------------------*/
/*����������ʱ��4�жϷ�����                      */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET){                //���TIM_IT_Update��λ����ʾTIM4����жϣ�����if	
		
		memcpy(&MQTT_RxDataInPtr[2],Usart2_RxBuff,Usart2_RxCounter);  //�������ݵ����ջ�����
		MQTT_RxDataInPtr[0] = Usart2_RxCounter/256;                   //��¼���ݳ��ȸ��ֽ�
		MQTT_RxDataInPtr[1] = Usart2_RxCounter%256;                   //��¼���ݳ��ȵ��ֽ�
		MQTT_RxDataInPtr+=RBUFF_UNIT;                                  //ָ������
		if(MQTT_RxDataInPtr==MQTT_RxDataEndPtr)                       //���ָ�뵽������β����
			MQTT_RxDataInPtr = MQTT_RxDataBuf[0];                     //ָ���λ����������ͷ
		Usart2_RxCounter = 0;                                         //����2������������������
		TIM_SetCounter(TIM3, 0);                                      //���㶨ʱ��6�����������¼�ʱping������ʱ��
		TIM_Cmd(TIM4, DISABLE);                        				  //�ر�TIM4��ʱ��
		TIM_SetCounter(TIM4, 0);                        			  //���㶨ʱ��4������
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);     			  //���TIM4����жϱ�־ 	
	}
}
/*-------------------------------------------------*/
/*����������ʱ��3�жϷ�����                      */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET){   //���TIM_IT_Update��λ����ʾTIM3����жϣ�����if	
		switch(Ping_flag){                               //�ж�Ping_flag��״̬
			case 0:										 //���Ping_flag����0����ʾ����״̬������Ping����  
					MQTT_PingREQ(); 					 //���Ping���ĵ����ͻ�����  
					break;
			case 1:										 //���Ping_flag����1��˵����һ�η��͵���ping���ģ�û���յ��������ظ�������1û�б����Ϊ0�������������쳣������Ҫ��������pingģʽ
					TIM3_ENABLE_2S(); 					 //���ǽ���ʱ��6����Ϊ2s��ʱ,���ٷ���Ping����
					MQTT_PingREQ();  					 //���Ping���ĵ����ͻ�����  
					break;
			case 2:										 //���Ping_flag����2��˵����û���յ��������ظ�
			case 3:				                         //���Ping_flag����3��˵����û���յ��������ظ�
			case 4:				                         //���Ping_flag����4��˵����û���յ��������ظ�	
					MQTT_PingREQ();  					 //���Ping���ĵ����ͻ����� 
					break;
			case 5:										 //���Ping_flag����5��˵�����Ƿ����˶��ping�����޻ظ���Ӧ�������������⣬������������
					Connect_flag = 0;                    //����״̬��0����ʾ�Ͽ���û���Ϸ�����
					TIM_Cmd(TIM3,DISABLE);               //��TIM3 				
					break;			
		}
		Ping_flag++;           		             		 //Ping_flag����1����ʾ�ַ�����һ��ping���ڴ��������Ļظ�
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);      //���TIM3����жϱ�־ 	
	}
}
/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
 
}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
