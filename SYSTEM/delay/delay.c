/*************************************************************************
 * @file    Delay.h
 * @author  lhs
 * @version V1.0
 * @date    2020.7.20
 * @brief   
**************************************************************************/

#include "delay.h"
#include "usart.h"
//#include "schedule.h"

/*******************************************************************************
 *@brief    �δ�ʱ����ʼ��    
 *@param    None
 *@retval   None
*******************************************************************************/
static u32  fac_us=0;//us��ʱ������		 	   
static u32 fac_ms=0; //ms��ʱ������
static volatile u32 count_ms=0;//ϵͳ���е�ʱ��	

void delay_init(void)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);//ѡ���ⲿʱ��  HCLK/8
#if USE_DELAY_INT
	//�δ�ʱ���ж�����
	fac_us=SystemCoreClock/8000000;		        //fac_us ÿus��Ҫ��ʱ���� 48000000/8000000=6 clk
	SysTick->VAL = TIMING_MS*1000*fac_us*20;
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;   	//����SYSTICK�ж�
	SysTick->LOAD = TIMING_MS*1000*fac_us; 	    //ÿ Count_ms �ж�һ��	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;   	//����SYSTICK   
	fac_ms=(u16)fac_us*1000;                    //ÿms��Ҫ��ʱ����
#endif
}	


//ÿ��MS�ж�һ��
void SysTick_Handler_Deal(void)
{				   
	count_ms += TIMING_MS;	
	//g_u8SchedTicksCnt++;//ϵͳ��������Ҫ
}

#if USE_DELAY_INT
/*******************************************************************************
 *@brief    ��ʱus������    
 *@param    ��ҪҪ�ǵ�us��
 *@retval   None
*******************************************************************************/   								   
void delay_us(u32 nus)
{		
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;	//LOAD��ֵ	    	 
	
	ticks=nus*fac_us; 			//��Ҫ�Ľ�����	  		 
	told=SysTick->VAL;        	//�ս���ʱ�ļ�����ֵ
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told) tcnt+=told-tnow;//����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks) break;//ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
		}  
	}							    
}

/*******************************************************************************
 *@brief    ��ʱms������    
 *@param    ��ҪҪ�ǵ�ms��
 *@retval   None
*******************************************************************************/  
void delay_ms(uint32_t nms)
{	 		  	  
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;	//LOAD��ֵ	
	u8  ms_500=nms/500;
	while(ms_500--)
	{
		ticks=fac_ms*500; 			//��Ҫ�Ľ�����	
		tcnt=0;
		told=SysTick->VAL;        	//�ս���ʱ�ļ�����ֵ
		while(1)
		{
			tnow=SysTick->VAL;	
			if(tnow!=told)
			{	    
				if(tnow<told) tcnt+=told-tnow;//����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
				else tcnt+=reload-tnow+told;	    
				told=tnow;
				if(tcnt>=ticks) break;//ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
			}  
		}
	}
	if(nms%500)
	{
		ticks=nms*fac_ms; 			//��Ҫ�Ľ�����	
		tcnt=0;
		told=SysTick->VAL;        	//�ս���ʱ�ļ�����ֵ
		while(1)
		{
			tnow=SysTick->VAL;	
			if(tnow!=told)
			{	    
				if(tnow<told) tcnt+=told-tnow;//����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
				else tcnt+=reload-tnow+told;	    
				told=tnow;
				if(tcnt>=ticks) break;//ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
			}  
		}
	}
		
} 

#else
void delay_us(uint32_t nus)
{
	uint32_t temp;	    	 
	SysTick->LOAD=nus*6; 					//ʱ�����	  		 
	SysTick->VAL=0x00;        					//��ռ�����
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//��ʼ����	  
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		//�ȴ�ʱ�䵽��   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//�رռ�����
	SysTick->VAL =0X00;      					 //��ռ�����	
}

void delay_ms(uint32_t nms)
{
	uint32_t temp;	
	for(;nms>0;nms--){	   
		SysTick->LOAD=(uint32_t)1*6*1000;				//ʱ�����(SysTick->LOADΪ24bit)
		SysTick->VAL =0x00;							//��ռ�����
		SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//��ʼ����  
		do
		{
			temp=SysTick->CTRL;
		}while((temp&0x01)&&!(temp&(1<<16)));		//�ȴ�ʱ�䵽��   
		SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//�رռ�����
		SysTick->VAL =0X00;       					//��ռ�����	 
	}
}
#endif

/*******************************************************************************
 *@brief    ��ȡϵͳ���е�ʱ��
 *@param    none
 *@retval   ʱ�䣨��λus��
*******************************************************************************/  
u32 GetTime_us(void)
{
	return count_ms*1000+((SysTick->LOAD-SysTick->VAL)/fac_us);
}	

/*******************************************************************************
 *@brief    ��ȡϵͳ���е�ʱ��
 *@param    none
 *@retval   ʱ�䣨��λms��
*******************************************************************************/  
u32 GetTime(void)
{
	return count_ms+(SysTick->LOAD-SysTick->VAL)/fac_ms;
}
			 

/****************************************************
  * @brief  ����ʱ���ֵ 
  * @param  lasttime �ϴε�ʱ��
  * @retval ��ǰʱ������һ��ʱ���ֵ
****************************************************/
u32 Delta_T(u32 lasttime)
{
    u32 temp;
    temp = GetTime();
    temp = (temp>lasttime)?(temp-lasttime):(U32_MAX-lasttime+temp);//�������ʱ������
    return temp+1;//ΪʲôҪ��һ ������
}


