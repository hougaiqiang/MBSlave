#ifndef __MBSLAVE_H
#define __MBSLAVE_H

#include "MBDesplay.h"
#include "../Common/type.h"

typedef enum enCallType
{
    POSTPONE = 0,
    INSTANTLY,
};

typedef struct Reg{
    u16 RegAddr;
    u16 Red:1;
    u16 Write:1;
    u16 CallType:2;
    void *CallBackFun;
}Reg;

typedef struct MBSlave{
    u32 DriverAddr;
    u16 BitRegNum;
    u16 wordRegNum;
    Reg *BitRegMap;
    Reg *WordRegMap;
}MBSlave;

void 


#endif
