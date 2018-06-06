#include "laser.h"

void GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//使能PORTA,PORTE时钟


	//初始化 	    浮空输入
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //PB12设置成输入，默认下拉	  
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB.12
}


