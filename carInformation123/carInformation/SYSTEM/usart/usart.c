#include "sys.h"
#include "usart.h"
#include "delay.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos ʹ��	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F4̽���߿�����
//����1��ʼ��		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2014/6/10
//�汾��V1.5
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved
//********************************************************************************
//V1.3�޸�˵�� 
//֧����Ӧ��ͬƵ���µĴ��ڲ���������.
//�����˶�printf��֧��
//�����˴��ڽ��������.
//������printf��һ���ַ���ʧ��bug
//V1.4�޸�˵��
//1,�޸Ĵ��ڳ�ʼ��IO��bug
//2,�޸���USART_RX_STA,ʹ�ô����������ֽ���Ϊ2��14�η�
//3,������USART_REC_LEN,���ڶ��崮�����������յ��ֽ���(������2��14�η�)
//4,�޸���EN_USART1_RX��ʹ�ܷ�ʽ
//V1.5�޸�˵��
//1,�����˶�UCOSII��֧��
////////////////////////////////////////////////////////////////////////////////// 	  
 

//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif
 
#if EN_USART1_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;       //����״̬���	

//��ʼ��IO ����1 
//bound:������
void uart_init(u32 bound){
   GPIO_InitTypeDef  GPIO_InitStructure;
	 USART_InitTypeDef USART_InitStructure;
	 NVIC_InitTypeDef NVIC_InitStructure;
	
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);//ʹ�ܴ���ʱ��
	
	 USART_DeInit(USART1);//��λ����1
	 GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
	 GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);//����PA9��PA10���ù��ܴ���1
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����жϹ������ȼ�����Ϊ2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	
	 GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9|GPIO_Pin_10; //��������λPin9��pin10
	 GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;          //ģʽ����Ϊ���ù���
	 GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;        //�������ģʽ
	 GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;     //����ģʽ
	 GPIO_InitStructure.GPIO_Speed=GPIO_Fast_Speed; //IO����50Mhz
	 GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	 USART_InitStructure.USART_BaudRate=bound;  //����������Ϊbound
	 USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None; //��Ӳ�������Ʒ�ʽ
	 USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;//���ý��ն˺ͷ��Ͷ�
	 USART_InitStructure.USART_Parity=USART_Parity_No;//����żУ��λ
	 USART_InitStructure.USART_StopBits=USART_StopBits_1;//1��ֹͣλ
	 USART_InitStructure.USART_WordLength=USART_WordLength_8b;//�˸�������Чλ
	 USART_Init(USART1,&USART_InitStructure);//��ʼ������1
	 
	  
	
#if EN_USART1_RX	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//���ڽ����ж�ʹ��
	NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;//�ж�ͨ��ΪUSART1
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;//�ж�ͨ��ʹ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�Ϊ2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;//��Ӧ���ȼ�Ϊ2
	NVIC_Init(&NVIC_InitStructure);//�жϳ�ʼ��
#endif
  USART_Cmd(USART1,ENABLE);//����ʹ��
}


void USART1_IRQHandler(void)                	//����1�жϷ������
{
  u8 res=0;
	if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET){
		res=USART_ReceiveData(USART1);
		if((USART_RX_STA&0x8000)==0){
			if((USART_RX_STA&0x4000)){
				if(res!=0x0a){
				USART_RX_STA=0;
			  }
			  else{
				USART_RX_STA|=0x8000;
			  }
		 }
		 else{
			 if(res==0x0d){
				 USART_RX_STA|=0x4000;
			 }
			 else{
				 USART_RX_BUF[(USART_RX_STA&0x3fff)]=res;
				 USART_RX_STA++;
				 if((USART_RX_STA&0x3fff)>(USART_REC_LEN-1)){ 
					  USART_RX_STA=0;
				 }
			 }
		 }
	 }
 }				
} 
#endif	


void uart_watching(void){
	int i=0;
	static int times=0;
	if((USART_RX_STA&0x8000)!=0){//����������ɣ�׼����������
		printf("\r\n�����͵�����Ϊ:\r\n");
		for(;i<(USART_RX_STA&0x3fff);i++){
      USART_SendData(USART1,USART_RX_BUF[i]);
			while((USART1->SR&0x40)==0);
		}
		USART_RX_STA=0;
	}
	else{
		if(times%2000==0){
			printf("\r\nֻ��������\r\n");
		}
		if(times%200==0){
			printf("\r\n����������\r\n");
		}
		if(times>100000){
			times=0;
		}
		times++;
	  delay_ms(20);
	}
}
	
