/* --COPYRIGHT--,BSD
 * Copyright (c) 2023, Mehrab Tech Incorporated V1.1
 * All rights reserved.
 *
 * Instructions use of file:
 * 1.Config a timer with 1 ms interrupt in your microcontroller  
 * 2.Config a PWM in your microcontroller
 * 3.Read "Attention" segments of file and modify it
 * 4.Copy buzzer.h and buzzer.c file in your project 
 * 5. include buzzer.h in your main file
 *
 * --/COPYRIGHT--*/
#ifndef __BUZZER_H__
#define __BUZZER_H__

#ifdef __cplusplus
extern "C"
{
#endif

/*************************** include libraries ****************************************************************************/
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/////////////////////////////////////////// "Attention" /////////////////////////////////////////////////////////////////////
// Modify for your microcontroller :( 
#include "stm32h7xx_hal.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* 
										 td																															tb
							|<------------>|                        								|<--------------------------->|	
							|              |																				|															|
							|              |																				|                             |
							| ------------ |                           ------------	|															|	------------
							|-            -|                          -            -|														  |-						-
90%<--------- -              - ---------->90%          -              -														  -              -
		{        -						   |-	         }            - |              -        						       -                -
 tr {       -                | -         } tf        -  |               -													-                  -
		{      -                 |  -        }          -   |                -                       -                    -
10%<----- -                  |   - ----->10%       -    |                 -										  -                      -
				 -                   |    -               -     |                  -                   -                        -
				-										 |     --------------- 			|                   -                 -                          -
														 |	 												|                   end               start 
														 |<------------------------>|                   of                of
																					ts                                birst							birst
*/

/*************************** List of general defines "Attention" ************************************************************/
// Modify for your alarm pulses
// Low alarm 
#define LOW_ALARM_tr	16   						// m sec  ,  rising time in standard					      
#define LOW_ALARM_tf	LOW_ALARM_tr 	  // m sec  ,  falling time in standard          
#define LOW_ALARM_td	200	 						// m sec  ,  duration time in standard           
#define LOW_ALARM_ts	200  						// m sec  ,  spacing pulse time in standard     
#define LOW_ALARM_tb  16   						// Second ,  interburst time in standard				

// Medium alarm 
#define MEDIUM_ALARM_tr  16   						 // m sec  ,  rising time in standard
#define MEDIUM_ALARM_tf	 MEDIUM_ALARM_tr   // m sec  ,  falling time in standard
#define MEDIUM_ALARM_td	 200							 // m sec  ,  duration time in standard
#define MEDIUM_ALARM_ts	 200  						 // m sec  ,  spacing pulse time in standard
#define MEDIUM_ALARM_tb  8                 // Second ,  interburst time in standard

// High alarm
#define HIGH_ALARM_tr  			 16   						 // m sec  ,  rising time in standard
#define HIGH_ALARM_tf				 HIGH_ALARM_tr  	 // m sec  ,  falling time in standard
#define HIGH_ALARM_td				 100 							 // m sec  ,  duration time in standard
#define HIGH_ALARM_BASIC_ts	 100  						 // m sec  ,  spacing pulse time in standard
#define HIGH_ALARM_tb  			 4                 // Second ,  interburst time in standard


/**************************** List of privet defines ***********************************/
// Low alarm pulse defines
#define	LOW_ALARM_RISING_TIME								round( (double)((LOW_ALARM_tr)*100/80) )    					  	  
#define LOW_ALARM_FALLING_TIME							round( (double)((LOW_ALARM_tf)*100/80) )    					   		
#define	LOW_ALARM_DURATION_TIME							(LOW_ALARM_td)-2*round( (double)(LOW_ALARM_RISING_TIME*10/100) )  		 
#define LOW_ALARM_PULSE_SPASING_TIME				(LOW_ALARM_ts)-2*round( (double)(LOW_ALARM_RISING_TIME*90/100) )     
#define LOW_ALARM_INTERBURST_TIME						((LOW_ALARM_tb)*1000)-2*round( (double)(LOW_ALARM_RISING_TIME*90/100) ) 

// Medium alarm pulse defines
#define	MEDIUM_ALARM_RISING_TIME							round( (double)((MEDIUM_ALARM_tr)*100/80) )    					  	  
#define MEDIUM_ALARM_FALLING_TIME							round( (double)((MEDIUM_ALARM_tf)*100/80) )    					   		
#define	MEDIUM_ALARM_DURATION_TIME						(MEDIUM_ALARM_td)-2*round( (double)(MEDIUM_ALARM_RISING_TIME*10/100) )  		 
#define MEDIUM_ALARM_PULSE_SPASING_TIME				(MEDIUM_ALARM_ts)-2*round( (double)(MEDIUM_ALARM_RISING_TIME*90/100) )     
#define MEDIUM_ALARM_INTERBURST_TIME					((MEDIUM_ALARM_tb)*1000)-2*round( (double)(MEDIUM_ALARM_RISING_TIME*90/100) ) 
	
// High alarm pulse defines
#define HIGH_ALARM_RISING_TIME									round( (double)((HIGH_ALARM_tr)*100/80) )
#define HIGH_ALARM_FALLING_TIME									round( (double)((HIGH_ALARM_tf)*100/80) )
#define	HIGH_ALARM_DURATION_TIME								(HIGH_ALARM_td)-2*round( (double)(HIGH_ALARM_RISING_TIME*10/100) )
#define HIGH_ALARM_BASIC_PULSE_SPASING_TIME     (HIGH_ALARM_BASIC_ts)-2*round( (double)(HIGH_ALARM_RISING_TIME*90/100) )
#define HIGH_ALARM_THIRD_PULSE_SPASING_TIME			HIGH_ALARM_BASIC_PULSE_SPASING_TIME + 2*HIGH_ALARM_tr
#define HIGH_ALARM_FIFTH_PULSE_SPASING_TIME			700	// it can be change between 0.35 s , 1.30 s
#define HIGH_ALARM_EIGHTH_PULSE_SPASING_TIME		HIGH_ALARM_THIRD_PULSE_SPASING_TIME
#define HIGH_ALARM_INTERBURST_TIME						  ((HIGH_ALARM_tb)*1000)-2*round( (double)(HIGH_ALARM_RISING_TIME*90/100) ) 


/***************************** List of general functions *********************************/
/** 
	* @brief 		Build every type alarm and will countinus generated
	* @Note			This function does not have a time out
	* @param 		1. alarmtype: 1.Low	2.Medium	3.High
	* @param		2. frequency: The frequency of PWM pulse
	* @param		3. volume: volume of sound that is Duty Cycle of PWM pulse
	* @param		4. timer channel PWM generation
	* @retval   None 
**/
void Generate_Alarm(uint8_t alarmtype , uint16_t frequency ,uint8_t volume,TIM_HandleTypeDef htim, uint32_t tim_channel);

/** 
	* @brief 		Stop Buzzer Alarm after end a cycle
	* @param 		htim timer handler
	* @retval   None 
**/
void Stop_Alarm_Generated(TIM_HandleTypeDef htim);

/**
  * @brief	  check state of alarm	 
  * @note 		use this function in your 1 ms timer interrupt
  * @param		htim timer handler
  * @retval		None
**/
void Check_Alarm_State(TIM_HandleTypeDef htim);

/***************************** List of private functions ***********************************/
/** 
	* @brief 		Quantification variables that is used in alarm timer schedule
	* @param 		alarmtype
	* @retval   None 
**/
void Quantification_Variables(uint8_t alarmtype);

/** 
	* @brief 		Rising buzzer sound
	* @param 		htim timer handler
	* @retval   None 
**/
void Rising_Alarm_Pulse(TIM_HandleTypeDef htim);

/** 
	* @brief 		Duration buzzer sound
	* @param 		htim timer handler
	* @retval   None 
**/
void Duration_Alarm_Pulse(TIM_HandleTypeDef htim);

/** 
	* @brief 		Falling buzzer sound
	* @param 		htim timer handler
	* @retval   None 
**/
void Falling_Alarm_Pulse(TIM_HandleTypeDef htim);

/** 
	* @brief 		Silence buzzer sound
	* @param 		htim timer handler
	* @retval   None 
**/
void Silence_Alarm(TIM_HandleTypeDef htim);

/** 
	* @brief 		Interburst buzzer sound
	* @param 		htim timer handler
	* @retval   None 
**/
void Interburst_Alarm(TIM_HandleTypeDef htim);


#ifdef __cplusplus
}
#endif

#endif //__BUZZER_H__


// :) 
// Developed by M.N 
