#include "laser.h"

void GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//ʹ��PORTA,PORTEʱ��


	//��ʼ�� 	 ��   ��������
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //PB12���ó����룬Ĭ������	  
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB.12
}


