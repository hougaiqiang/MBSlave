#ifndef __MBSLAVE_H
#define __MBSLAVE_H

#include "MBDisplay.h"
#include "../Common/type.h"

#define MAX_MBSlave_NUM  3      //最大从机数量

#define BIG_ENDIAN      1       //从机大端模式
#define LITTLE_ENDIAN   2       //从机小端模式

//typedef unsigned int DataType;

typedef enum DataType{
    MB_BIT = 1,
    MB_CHAR,
    MB_SHORT,
    MB_INT,
    MB_FLOAT,
};

typedef enum enMbErrCode{
    FALSE       =   0,
    TRUE        =   1,
    PtrEmpty    =   2,     //空指针

};


typedef enum enCallType
{
    POSTPONE = 0,   //延后执行
    INSTANTLY,      //立即执行
};

typedef enum enRwCall
{
    ReadCall = 0,   //读取时候调用
    WriteCall,      //写入时候调用
};

typedef struct Reg{
    u16 RegAddr;
    u16 Red:1;
    u16 Write:1;
    u16 CallType:1;
    u16 RWCall:1;
    void *CallBackFun;
}Reg;

typedef struct MBSlave{
    u16 DriverAddr;
    u16 EndianType;
    u16 BitRegNum;
    u16 wordRegNum;
    Reg *BitRegMap;
    Reg *WordRegMap;
}MBSlave;



/********************* 对外接口 **************************/
void ComDataProcess(const int com, u8 *Data, int DataLen);   //连接串口

int MBSlave_Create(MBSlave *Slave, int com, int *DriverId);  //创建从机

int WriteMBRegData(int DriverId, u16 addr, enum DataType type, u8 *data, int lens);  //模块使用
int ReadMBRegData(int DriverId, u16 addr, enum DataType type, u8 *data int lens);    //模块使用

#endif
