/* 
 * File:   OC.h
 * Author: ryan
 *
 * Created on August 15, 2015, 9:20 PM
 */

#ifndef OC_H
#define	OC_H

#ifdef	__cplusplus
extern "C" {
#endif

    
    typedef enum {
        PWM_CH_1,
        PWM_CH_2,
        PWM_CH_3,
        PWM_CH_4,
        PWM_CH_5
    }PWM_Channel;
    
    
    typedef struct {
        float dutyCycle; // Only used in PWM mode
        Timer_Config timer;
        Pin_Output pin; //our output pin
        uint8 enable;
        PWM_Channel channel;
        void *callback; //A callback function for the OCx interrupt
    }PWM_Config;

    
    //functions
    void initialize_PWM(PWM_Config config);
    void enable_PWM(PWM_Config config);
    void disable_PWM(PWM_Config config);
    
    void update_PWM(float dutyCycle);
    
    
    


#ifdef	__cplusplus
}
#endif

#endif	/* OC_H */

