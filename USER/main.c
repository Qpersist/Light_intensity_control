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
 ALIENTEKս��STM32������ʵ��9
 PWM���ʵ��  
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/
u8 str1[6]="Auto  ";
u8 str2[6]="manual";
u8 str3[4];        //��ʾ����ǿ��


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

	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(9600);	 //���ڳ�ʼ��Ϊ115200
	GPIO_Configuration();
	LCD1602_Init();
	Adc_Init();
 	LED_Init();			    	 //LED�˿ڳ�ʼ��
 	TIM3_PWM_Init(899,0);	 //����Ƶ��PWMƵ��=72000000/900=80Khz
	KEY_Init();
   	while(1)
	{
//		if(USART_RX_STA&0x8000)      //����������
//		{					   
//			len=USART_RX_STA&0x3fff; //�õ��˴ν��յ������ݳ���


				
//			USART_RX_STA=0;
//		}
		key=KEY_Scan(0);	//�õ���ֵ
	   	if(key)
		{						   
			switch(key)
			{				 
				case WKUP_PRES:	//�ı��Զ��ֶ�ģʽ
						flag1=!flag1;
						if(flag1==1)
						{
							LCD1602_Write_Cmd(0x80+10);
							LCD1602_Write_Dat(flag+0x30);
						}
					break;
				
				case KEY2_PRES:	//�ı��Զ���������ģʽ
						flag++;

						if(flag>2)
							flag=0;						
						LCD1602_Write_Cmd(0x80+10);
						LCD1602_Write_Dat(flag+0x30);
					break;
						
				case KEY1_PRES:	//�ֶ���������ʱ��+	 
						if(flag1==0)
						{	
							led0pwmval+=100;
							if(led0pwmval>800)
								led0pwmval=899;
						}
					break;
				
				case KEY0_PRES:	//�ֶ���������ʱ��- 
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
			if(flag1!=0)                   //�Զ�ģʽ
			{	
				LCD1602_Write_Cmd(0x80+10);
				LCD1602_Write_Dat(flag+0x30);
				LCD1602_Write_Cmd(0x80);
				for(num=0;num<=5;num++)
				{
						LCD1602_Write_Dat(str1[num]);	
				}
				if(flag==0) 									//����ģʽ
				{                
					if(laser==1)
						{
							adcx=Get_Adc_Average(ADC_Channel_9,10);
							led0pwmval=(4096-adcx)*800/(4096-2000);

						}
					else 
					{
//						printf("����ģʽ����\r\n");
						led0pwmval=899;
					}
				}
				else if(flag==1)             //����ģʽ
				{
							adcx=Get_Adc_Average(ADC_Channel_9,10);
							led0pwmval=(4096-adcx)*800/(4096-2000)+400;
//							printf("����ģʽ\r\n");
				}
				else if(flag==2)              //����ģʽ
				{
					if(laser==1)
					{
						adcx=Get_Adc_Average(ADC_Channel_9,10);
						led0pwmval=(4096-adcx)*800/(4096-500);
//						printf("����ģʽ����\r\n");
					}
					else
					{
//						printf("����ģʽ����\r\n");
						led0pwmval=800;
					}
				}
			}
			
			else                          //�ֶ�ģʽ
			{	
//				printf("�ֶ�ģʽ\r\n");
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
//		printf("����ֵ")
	}	 
 }

