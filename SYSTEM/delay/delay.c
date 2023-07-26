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
 *@brief    滴答定时器初始化    
 *@param    None
 *@retval   None
*******************************************************************************/
static u32  fac_us=0;//us延时倍乘数		 	   
static u32 fac_ms=0; //ms延时倍乘数
static volatile u32 count_ms=0;//系统运行的时间	

void delay_init(void)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);//选择外部时钟  HCLK/8
#if USE_DELAY_INT
	//滴答定时器中断配置
	fac_us=SystemCoreClock/8000000;		        //fac_us 每us需要的时钟数 48000000/8000000=6 clk
	SysTick->VAL = TIMING_MS*1000*fac_us*20;
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;   	//开启SYSTICK中断
	SysTick->LOAD = TIMING_MS*1000*fac_us; 	    //每 Count_ms 中断一次	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;   	//开启SYSTICK   
	fac_ms=(u16)fac_us*1000;                    //每ms需要的时钟数
#endif
}	


//每个MS中断一次
void SysTick_Handler_Deal(void)
{				   
	count_ms += TIMING_MS;	
	//g_u8SchedTicksCnt++;//系统调度器需要
}

#if USE_DELAY_INT
/*******************************************************************************
 *@brief    延时us级函数    
 *@param    需要要是的us数
 *@retval   None
*******************************************************************************/   								   
void delay_us(u32 nus)
{		
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;	//LOAD的值	    	 
	
	ticks=nus*fac_us; 			//需要的节拍数	  		 
	told=SysTick->VAL;        	//刚进入时的计数器值
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told) tcnt+=told-tnow;//这里注意一下SYSTICK是一个递减的计数器就可以了.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks) break;//时间超过/等于要延迟的时间,则退出.
		}  
	}							    
}

/*******************************************************************************
 *@brief    延时ms级函数    
 *@param    需要要是的ms数
 *@retval   None
*******************************************************************************/  
void delay_ms(uint32_t nms)
{	 		  	  
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;	//LOAD的值	
	u8  ms_500=nms/500;
	while(ms_500--)
	{
		ticks=fac_ms*500; 			//需要的节拍数	
		tcnt=0;
		told=SysTick->VAL;        	//刚进入时的计数器值
		while(1)
		{
			tnow=SysTick->VAL;	
			if(tnow!=told)
			{	    
				if(tnow<told) tcnt+=told-tnow;//这里注意一下SYSTICK是一个递减的计数器就可以了.
				else tcnt+=reload-tnow+told;	    
				told=tnow;
				if(tcnt>=ticks) break;//时间超过/等于要延迟的时间,则退出.
			}  
		}
	}
	if(nms%500)
	{
		ticks=nms*fac_ms; 			//需要的节拍数	
		tcnt=0;
		told=SysTick->VAL;        	//刚进入时的计数器值
		while(1)
		{
			tnow=SysTick->VAL;	
			if(tnow!=told)
			{	    
				if(tnow<told) tcnt+=told-tnow;//这里注意一下SYSTICK是一个递减的计数器就可以了.
				else tcnt+=reload-tnow+told;	    
				told=tnow;
				if(tcnt>=ticks) break;//时间超过/等于要延迟的时间,则退出.
			}  
		}
	}
		
} 

#else
void delay_us(uint32_t nus)
{
	uint32_t temp;	    	 
	SysTick->LOAD=nus*6; 					//时间加载	  		 
	SysTick->VAL=0x00;        					//清空计数器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//开始倒数	  
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		//等待时间到达   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//关闭计数器
	SysTick->VAL =0X00;      					 //清空计数器	
}

void delay_ms(uint32_t nms)
{
	uint32_t temp;	
	for(;nms>0;nms--){	   
		SysTick->LOAD=(uint32_t)1*6*1000;				//时间加载(SysTick->LOAD为24bit)
		SysTick->VAL =0x00;							//清空计数器
		SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//开始倒数  
		do
		{
			temp=SysTick->CTRL;
		}while((temp&0x01)&&!(temp&(1<<16)));		//等待时间到达   
		SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//关闭计数器
		SysTick->VAL =0X00;       					//清空计数器	 
	}
}
#endif

/*******************************************************************************
 *@brief    读取系统运行的时间
 *@param    none
 *@retval   时间（单位us）
*******************************************************************************/  
u32 GetTime_us(void)
{
	return count_ms*1000+((SysTick->LOAD-SysTick->VAL)/fac_us);
}	

/*******************************************************************************
 *@brief    读取系统运行的时间
 *@param    none
 *@retval   时间（单位ms）
*******************************************************************************/  
u32 GetTime(void)
{
	return count_ms+(SysTick->LOAD-SysTick->VAL)/fac_ms;
}
			 

/****************************************************
  * @brief  计算时间差值 
  * @param  lasttime 上次的时间
  * @retval 当前时间与上一次时间差值
****************************************************/
u32 Delta_T(u32 lasttime)
{
    u32 temp;
    temp = GetTime();
    temp = (temp>lasttime)?(temp-lasttime):(U32_MAX-lasttime+temp);//带溢出的时间差计算
    return temp+1;//为什么要加一 ？？？
}


