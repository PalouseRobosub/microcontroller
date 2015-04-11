#include "pneumatic.h"

static Pneumatic pinmap[PNEU_COUNT];

void initialize_pneumatics(void)
{
    //pin setup
    TRISBbits.TRISB3  = 0; //PC1
    TRISBbits.TRISB2  = 0; //PC2
    TRISBbits.TRISB15 = 0; //PC3
    TRISBbits.TRISB14 = 0; //PC4
    TRISBbits.TRISB13 = 0; //PC5
    TRISBbits.TRISB11 = 0; //PC6
    TRISBbits.TRISB10 = 0; //PC7
    TRISBbits.TRISB7  = 0; //PC8

    LATBbits.LATB3  = 0; //PC1
    LATBbits.LATB2  = 0; //PC2
    LATBbits.LATB15 = 0; //PC3
    LATBbits.LATB14 = 0; //PC4
    LATBbits.LATB13 = 0; //PC5
    LATBbits.LATB11 = 0; //PC6
    LATBbits.LATB10 = 0; //PC7
    LATBbits.LATB7  = 0; //PC8

    //assign pin mappings
    pinmap[PNEU_1] = 3;
    pinmap[PNEU_2] = 2;
    pinmap[PNEU_3] = 15;
    pinmap[PNEU_4] = 14;
    pinmap[PNEU_5] = 13;
    pinmap[PNEU_6] = 11;
    pinmap[PNEU_7] = 10;
    pinmap[PNEU_8] = 7;
    
}

Error toggle_pneumatic(Pneumatic which_pneumatic, boolean enable)
{
    if(enable)
    {
        PORTBSET = (1 << pinmap[which_pneumatic]);
    }
    else
    {
        PORTBCLR = (1 << pinmap[which_pneumatic]);
    }

    return ERR_NO_ERR;
}