/***************
 * MBSlaave.c
****************/
#include <stdio.h>

#include "MBSlave.h"
#include "MBcrc.h"

u16 HostEndianType = 0; //本机类型

typedef struct MBSlaveInfo[
    int         com;
    MBSlave     *Slave;
    u8          *BitMap;
    u8          *WordMap;
]MBSlaveInfo;

//modbus从机设备列表
MBSlaveInfo MBSlaveList[MAX_MBSlave_NUM] = {0};

/*****************************************
 * 创建一个modbus从机
 * 参数：   Slave： 从机类
 *          com：   使用的端口号
******************************************/
int MBSlave_Create(MBSlave *Slave, int com, int *DriverId)
{
    static int DriverNum = 0;
    if(NULL == Slave)
    {
        return PtrEmpty;
    }
    if(MAX_MBSlave_NUM == DriverNum)
    {
        return FALSE;
    }
    MBSlaveList[DriverNum].Slave = Slave;
    MBSlaveList[DriverNum].com = com;
    MBSlaveList[DriverNum].BitMap = malloc(MBSlaveList[DriverNum].Slave->BitRegNum/8);    //申请位寄存器内存
    MBSlaveList[DriverNum].WordMap = malloc(MBSlaveList[DriverNum].Slave->BitRegNum*2);   //申请字寄存器内存
    DriverNum++;
    *DriverId = DriverNum - 1;
    return TRUE;
}


/****************************************************
 * 获取本机大小端类型
*****************************************************/
void getHostEndianTyoe()
{
    union test{
        u32 a;
        u8 b;
    }T;

    T.a = 1;
    if(b == a)
    {
        HostEndianType = LITTLE_ENDIAN; //本机是小端模式
    }
    else 
    {
        HostEndianType = BIG_ENDIAN; //本机是大端模式
    }
}

/****************************************************
 * 大小端转换: 主机字节序转换为网络字节序（modbus字节序）
*****************************************************/
void HtoMBs(const u16 DestEndianType, u8 *data, int lens)
{
    int i=0;
    if(0 == lens)
    {
        return ;
    }
    if(DestEndianType == HostEndianType)
    {
        return ;
    }
    else
    {
       for(i=0; i<lens; i+=2)
       {
           data[i]^=data[i+1]^=data[i]^=data[i+1];
       }
    }
    return ;
}

/****************************************************
 * 大小端转换: 网络字节序转换为主机字节序
*****************************************************/
void MBtoHs(const u16 DestEndianType, u8 *data, int lens)
{
    if(0 == lens)
    {
        return ;
    }
    if(DestEndianType == BIG_ENDIAN)
    {
        return ;
    }
    else
    {
        for(i=0; i<lens; i+=2)
        {
            data[i]^=data[i+1]^=data[i]^=data[i+1];
        }
        
    }
    return ;
}


/***************************************************************
 * 从Buff读取数据
****************************************************************/
int ReadMBRegData(int DriverId, u16 addr, enum DataType type, u8 *data int lens)
{
    if(NULL == slave || NULL == data)
    {
        return PtrEmpty;
    }
    switch(type)
    {
        case MB_BIT:{
            memcpy(data, MBSlaveList[DriverId].BitMap+addr,  lens);
            break;
        }
        case MB_CHAR:{
            memcpy(data, MBSlaveList[DriverId].WordMap+addr,  lens);
            break;
        }
        case MB_FLOAT:
        case MB_INT:
        case MB_SHORT:
        {
            memcpy(data, MBSlaveList[DriverId].WordMap+addr,  lens);     //这里有错误，待改正
            MBtoHs(HostEndianType, data, lens);
            break;
        }
        default :
        {
           
            break;
        }
    }

    data = 
}

/***************************************************************
 * 写入数据到Buff
****************************************************************/
int WriteMBRegData(int DriverId, u16 addr, enum DataType type, u8 *data, int lens)
{
    if(NULL == slave || NULL == data)
    {
        return PtrEmpty;
    }
    switch(type)
    {
        case MB_BIT:{
            memcpy(MBSlaveList[DriverId].BitMap+addr, data,  lens);
            break;
        }
        case MB_CHAR:{
            memcpy(MBSlaveList[DriverId].WordMap+addr, data,  lens);
            break;
        }
        case MB_FLOAT:
        case MB_INT:
        case MB_SHORT:
        {
            HtoMBs(BIG_ENDIAN, data, lens);
            memcpy(MBSlaveList[DriverId].WordMap+addr, data,  lens);
            break;
        }
        default :
        {
           
            break;
        }
    }
}

/****************************************************
 * 串口数据处理
*****************************************************/
void ComDataProcess(const int com, u8 *Data, int DataLen)
{
    int i;
    for(i=0;i<MAX_MBSlave_NUM; i++)
    {
        if(com == MBSlaveList[i].com)
        {
            
        }
    }
}

