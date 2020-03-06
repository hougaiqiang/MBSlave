
#ifndef __MBDISPLAY_H
#define __MBDISPLAY_H

#include "MBSlave.h"
#include "../Common/type.h"

#define DisplayDriverAddr 1

#define DISPALY_MAX_BIT_REG 100

const Reg BitRegList[DISPALY_MAX_BIT_REG] = {
// addr,R,W,calltype, callbackfunptr
    {50,ON,ON,POSTPONE,nullptr},

}

#define DISPALY_MAX_WORD_REG 100

const Reg WordRegList [DISPALY_MAX_WORD_REG] = {
// addr,R,W,calltype, callbackfunptr
    {50,ON,ON,POSTPONE,nullptr},

}

MBSlave MBSlaveDisplay = {
    DisplayDriverAddr,
    DISPALY_MAX_BIT_REG,
    DISPALY_MAX_WORD_REG,
    BitRegList,
    WordRegList,
};




#endif
