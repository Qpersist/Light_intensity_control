#ifndef __LASER_H
#define __LASER_H	 
#include "sys.h"
 	 

#define laser  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12)//��ȡ����0


 void GPIO_Config(void);




				    
#endif


