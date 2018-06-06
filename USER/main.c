#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
#include "adc.h"
#include "lcd1602.h"
#include "key.h"
#include "laser.h"
/************************************************
 ALIENTEK战舰STM32开发板实验9
 PWM输出实验  
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/
u8 str1[6]="Auto  ";
u8 str2[6]="manual";
u8 str3[4];        //显示光照强度


void display_Lux(void)
{
	str3[0]=led0pwmval/1000+0x30;
	str3[1]=led0pwmval%1000/100+0x30;
	str3[2]=led0pwmval%100/10+0x30;
	str3[3]=led0pwmval%10+0x30;
	LCD1602_Show_Str(0,1,str3);
}

 int main(void)
 {
	int num=0,num1=0;
	u16 t;  
	u16 len;
	vu8 key=0;
	u16 adcx=0;
	u8 dir=1;	

	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(9600);	 //串口初始化为115200
	GPIO_Configuration();
	LCD1602_Init();
	Adc_Init();
 	LED_Init();			    	 //LED端口初始化
 	TIM3_PWM_Init(899,0);	 //不分频。PWM频率=72000000/900=80Khz
	KEY_Init();
   	while(1)
	{
//		if(USART_RX_STA&0x8000)      //如果接收完成
//		{					   
//			len=USART_RX_STA&0x3fff; //得到此次接收到的数据长度


				
//			USART_RX_STA=0;
//		}
		key=KEY_Scan(0);	//得到键值
	   	if(key)
		{						   
			switch(key)
			{				 
				case WKUP_PRES:	//改变自动手动模式
						flag1=!flag1;
						if(flag1==1)
						{
							LCD1602_Write_Cmd(0x80+10);
							LCD1602_Write_Dat(flag+0x30);
						}
					break;
				
				case KEY2_PRES:	//改变自动控制亮度模式
						flag++;

						if(flag>2)
							flag=0;						
						LCD1602_Write_Cmd(0x80+10);
						LCD1602_Write_Dat(flag+0x30);
					break;
						
				case KEY1_PRES:	//手动控制亮度时，+	 
						if(flag1==0)
						{	
							led0pwmval+=100;
							if(led0pwmval>800)
								led0pwmval=899;
						}
					break;
				
				case KEY0_PRES:	//手动控制亮度时，- 
					  if(flag1==0)
						{
							led0pwmval-=100;
							if(led0pwmval<120)
								led0pwmval=0;
						}
					break;
			}
		}
		else
		{
			if(flag1!=0)                   //自动模式
			{	
				LCD1602_Write_Cmd(0x80+10);
				LCD1602_Write_Dat(flag+0x30);
				LCD1602_Write_Cmd(0x80);
				for(num=0;num<=5;num++)
				{
						LCD1602_Write_Dat(str1[num]);	
				}
				if(flag==0) 									//教室模式
				{                
					if(laser==1)
						{
							adcx=Get_Adc_Average(ADC_Channel_9,10);
							led0pwmval=(4096-adcx)*800/(4096-2000);

						}
					else 
					{
//						printf("教室模式待机\r\n");
						led0pwmval=899;
					}
				}
				else if(flag==1)             //走廊模式
				{
							adcx=Get_Adc_Average(ADC_Channel_9,10);
							led0pwmval=(4096-adcx)*800/(4096-2000)+400;
//							printf("走廊模式\r\n");
				}
				else if(flag==2)              //厕所模式
				{
					if(laser==1)
					{
						adcx=Get_Adc_Average(ADC_Channel_9,10);
						led0pwmval=(4096-adcx)*800/(4096-500);
//						printf("厕所模式开灯\r\n");
					}
					else
					{
//						printf("厕所模式待机\r\n");
						led0pwmval=800;
					}
				}
			}
			
			else                          //手动模式
			{	
//				printf("手动模式\r\n");
				LCD1602_Write_Cmd(0x80);		
				for( num1=0;num1<=5;num1++)
				{
						LCD1602_Write_Dat(str2[num1]);	
				}
			}
		}
		if(laser==1)
		{
			GPIO_ResetBits(GPIOB,GPIO_Pin_0);
		}
		else 
			GPIO_SetBits(GPIOB,GPIO_Pin_0);
		display_Lux();
		TIM_SetCompare3(TIM4,led0pwmval);
		LCD1602_Write_Cmd(0x80+0x40+4);
		LCD1602_Write_Dat(0x20);
		LCD1602_Write_Dat(0x20);

//		LCD1602_Show_Str(6,6,str4);
//		printf("光照值")
	}	 
 }

