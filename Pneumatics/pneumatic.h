/* 
 * File:   pneumatic.h
 * Author: james
 *
 * Created on April 10, 2015, 10:38 PM
 */

#ifndef PNEUMATIC_H
#define	PNEUMATIC_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "System.h"


    typedef enum PNEUMATIC
    {
        PNEU_1,
        PNEU_2,
        PNEU_3,
        PNEU_4,
        PNEU_5,
        PNEU_6,
        PNEU_7,
        PNEU_8,

        PNEU_COUNT
                
    }Pneumatic;


    void initialize_pneumatics(void);
    Error toggle_pneumatic(Pneumatic which_pneumatic, boolean enable);


#ifdef	__cplusplus
}
#endif

#endif	/* PNEUMATIC_H */

