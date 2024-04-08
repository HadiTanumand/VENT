#include "buzzer.h"

//**************************************** Variables **********************************************// 			
uint16_t	Timer_periode=0 , // ARR of PWM timer
					Counter_alarm=0 , // a counter for each cycle of alarm from start rising to end interburst
					Timer_compare=0 ; // CCR of PWM Timer
					
uint8_t			  Volume=0 , // volume of your sound (with of your pulse) in Percent
				  Alarm_type=0 , // type of alalrm: 1.Low Priority	2.Medium Priority		3.High Priority
				 Temp_volume=0 ; // temporary of volume for increase or decrease
				 
uint32_t  Timer_channel=0 ; // PWM timer channel
//*************************************************************************************************//

//**************************************** Definition of functions ********************************//
void Generate_Alarm(uint8_t alarmtype , uint16_t frequency ,uint8_t volume,TIM_HandleTypeDef htim, uint32_t tim_channel){
	Alarm_type = alarmtype; 
	Volume = volume;  
	Timer_channel = tim_channel;
	Timer_periode = 1000000 / frequency;
	__HAL_TIM_SetAutoreload(&htim,Timer_periode);
  __HAL_TIM_SetCompare(&htim,Timer_channel,Timer_compare);
		HAL_TIM_PWM_Start(&htim,Timer_channel);
}

void Stop_Alarm_Generated(TIM_HandleTypeDef htim){
		__HAL_TIM_SetCompare(&htim,Timer_channel,Timer_compare);	
		HAL_TIM_PWM_Stop(&htim,Timer_channel);
		Alarm_type = 0;
}
void Rising_Alarm_Pulse(TIM_HandleTypeDef htim){
	Timer_compare  = (Timer_periode * Temp_volume) / 100;
	__HAL_TIM_SetCompare(&htim,Timer_channel,Timer_compare);
	if ((Volume % 20) == 0)		Temp_volume += (Volume / 20);
	else if ((Volume % 20) != 0)
	{
		if ((Counter_alarm % 2) == 0)
		Temp_volume +=  (Volume / 20);
		else if ((Counter_alarm % 2) != 0)
		Temp_volume +=  (Volume / 20) + 1;	
	}
	if (Temp_volume > Volume)		Temp_volume = Volume;	
}

void Duration_Alarm_Pulse(TIM_HandleTypeDef htim){
			if ( Temp_volume != Volume) 
			{
				Temp_volume = Volume;
				Timer_compare  = (Timer_periode * Temp_volume) / 100;
				__HAL_TIM_SetCompare(&htim,Timer_channel,Timer_compare);
			}
}

void Falling_Alarm_Pulse(TIM_HandleTypeDef htim){
	Timer_compare  = (Timer_periode * Temp_volume) / 100;
	__HAL_TIM_SetCompare(&htim,Timer_channel,Timer_compare);
		if ((Volume % 20) == 0)		Temp_volume -= (Volume / 20);
		else if ((Volume % 20) != 0)
		{
			if ((Counter_alarm % 2) == 0)
			Temp_volume -=  (Volume / 20);
			else if ((Counter_alarm % 2) != 0)
			Temp_volume -=  (Volume / 20) + 1;	
		}
		if ((Temp_volume < 0) || (Temp_volume > Volume))		Temp_volume = 0;
}
void Silence_Alarm(TIM_HandleTypeDef htim){
		__HAL_TIM_SetCompare(&htim,TIM_CHANNEL_1,0);
}
void Interburst_Alarm(TIM_HandleTypeDef htim){
	Silence_Alarm(htim);
}
void Check_Alarm_State(TIM_HandleTypeDef htim){
	if (Alarm_type != 0) // The alarm is not off
	{
		if (Alarm_type == 1) // Low alarm is selected
			{
				// Rising puls 1 
				if (Counter_alarm >= LOW_ALARM_START_RISE_PULSE1 && Counter_alarm <= LOW_ALARM_END_RISE_PULSE1)
					Rising_Alarm_Pulse(htim);
				// Duration puls 1
				else if (Counter_alarm >= LOW_ALARM_START_DURATION_PULSE1 && Counter_alarm <= LOW_ALARM_END_DURATION_PULSE1)
					Duration_Alarm_Pulse(htim);
				// Falling puls 1
				else if (Counter_alarm >= LOW_ALARM_START_FALL_PULSE1 && Counter_alarm <= LOW_ALARM_END_FALL_PULSE1)
					Falling_Alarm_Pulse(htim);
				// Silence 
				else if (Counter_alarm >= LOW_ALARM_START_FIRST_PULSE_SPASING && Counter_alarm <= LOW_ALARM_END_FIRST_PULSE_SPASING)
				Silence_Alarm(htim);
				// Rising puls 2
				else if (Counter_alarm >= LOW_ALARM_START_RISE_PULSE2 && Counter_alarm <= LOW_ALARM_END_RISE_PULSE2)
					Rising_Alarm_Pulse(htim);
				// Duration puls 2
				else if (Counter_alarm >= LOW_ALARM_START_DURATION_PULSE2 && Counter_alarm <= LOW_ALARM_END_DURATION_PULSE2)
					Duration_Alarm_Pulse(htim);
				// Falling puls 2
				else if (Counter_alarm >= LOW_ALARM_START_FALL_PULSE2 && Counter_alarm <= LOW_ALARM_END_FALL_PULSE2)
					Falling_Alarm_Pulse(htim);
				// InterBurst low alarm
				else if (Counter_alarm >= LOW_ALARM_START_INTERBURST && Counter_alarm <= LOW_ALARM_END_INTERBURST)
					Interburst_Alarm(htim);
				// Cheking Ending a cycle low alarm 
				Counter_alarm ++;
				if (Counter_alarm > LOW_ALARM_END_INTERBURST)		Counter_alarm = 0;							
		  }
			
	}
		
	
	
	
}
//*************************************************************************************************//






