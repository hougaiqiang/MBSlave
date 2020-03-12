/***************
 * MBSlaave.c
****************/
#include <stdio.h>

#include "MBSlave.h"
#include "MBcrc.h"
#include "../Common/mylib.h"

u16 HostEndianType = 0; //本机类型

struct MBSlaveInfo{
    int         com;
    MBSlave     *Slave;
    u8          *BitMap;
    u8          *WordMap;
};

//modbus从机设备列表
struct MBSlaveInfo MBSlaveList[MAX_MBSlave_NUM] = {0};

/*****************************************
 * 创建一个modbus从机
 * 参数：   Slave： 从机类
 *          com：   使用的端口号
 *         DriverId：返回创建的从机号
 * 返回值： 执行结果与错误码
 *      
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
    MBSlaveList[DriverNum].BitMap = My_malloc(MBSlaveList[DriverNum].Slave->BitRegNum/8);    //申请位寄存器内存
    My_memset(MBSlaveList[DriverNum].BitMap, 0, MBSlaveList[DriverNum].Slave->BitRegNum/8);  //尚待确定是否每次重置

    MBSlaveList[DriverNum].WordMap = My_malloc(MBSlaveList[DriverNum].Slave->BitRegNum*2);   //申请字寄存器内存
    My_memset(MBSlaveList[DriverNum].WordMap, 0, MBSlaveList[DriverNum].Slave->BitRegNum*2);

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
            My_memcpy(data, MBSlaveList[DriverId].BitMap+addr-,  lens);
            break;
        }
        case MB_CHAR:{
            My_memcpy(data, MBSlaveList[DriverId].WordMap+addr,  lens);
            break;
        }
        case MB_FLOAT:
        case MB_INT:
        case MB_SHORT:
        {
            My_memcpy(data, MBSlaveList[DriverId].WordMap+addr,  lens);     //这里有错误，待改正
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
            My_memcpy(MBSlaveList[DriverId].BitMap+addr, data,  lens);
            break;
        }
        case MB_CHAR:{
            My_memcpy(MBSlaveList[DriverId].WordMap+addr, data,  lens);
            break;
        }
        case MB_FLOAT:
        case MB_INT:
        case MB_SHORT:
        {
            HtoMBs(BIG_ENDIAN, data, lens);
            My_memcpy(MBSlaveList[DriverId].WordMap+addr, data,  lens);
            break;
        }
        default :
        {
           
            break;
        }
    }
}

/******************************************************************
 * 位数据拷贝
 * 参数：   dst     目标地址
 *          src     源地址
 *          point   起始位置（位）
 *          num     长度（位数）
*******************************************************************/
void My_BitCpy(u8 *dst, u8 *src, int point, int num)
{
    int i = 0;
    if((NULL == dst) || (NULL == src))
    {
        return PtrEmpty;
    }

    My_memset(dst, 0, (num-1)/8+1);

    for(i=0; i<num; i++)
    {
        if((*(src + point/8)) & ((0x01 << (point % 8)) ))
        {
            (*dst + i/8) |= (0x01 << (i % 8)) 
        }
        point++;
    }
    
    return ;
}

/****************************************************************
 * 串口数据处理  //回调函数响应，
*****************************************************************/
void DriverProcess(int DriverId, u8 *RcvData, int RDataLen, u8 *SendData, int *SLen)
{
    u16 Regaddr = 0xffff;
    u8 functionCode  = 0xff;
    u8 SLens = 0xff;
    u16 RegNum  = 0xffff;

    functionCode = RcvData[1];
    Regaddr = (u16)RcvData[2]<<8 + (u16)RcvData[3];

    SendData[0] = MBSlaveList[DriverId].Slave->DriverAddr;
    SendData[1] = functionCode;

    switch(functionCode)
    {
        case 0x01:
        case 0x02:{ //读bit
            SendData[2] = SLens = (((u16)RcvData[4]<<8 + (u16)RcvData[5])-1)/8 + 1;
            My_memcpy(SendData[3], MBSlaveList[DriverId].BitMap[(Regaddr-1)/8], SLens);     //这里有问题
            //加入地址安全代码
            break;
        }
        case 0x03:
        case 0x04:{ //读word
            SendData[2] = SLens = ((u16)RcvData[4]<<8 + (u16)RcvData[5])*2;
            My_memcpy(SendData[3], MBSlaveList[DriverId].WordMap[Regaddr], SLens);
            break;
        }
        case 0x05:{ //写Bit

            break;
        }
        case 0x06:{ //写word
            SLens = ((u16)RcvData[4]<<8 + (u16)RcvData[5])*2;
            *SLen = RcvData[6];
            My_memcpy(MBSlaveList[DriverId].WordMap[Regaddr], RcvData[7], , SLens);
            break;
        }
        case 0x0f:{ //写多个bit
            
            break;
        }
        case 0x10:{ //写多个word
            SLens = ((u16)RcvData[4]<<8 + (u16)RcvData[5])*2;
            *SLen = RcvData[6];
            My_memcpy(MBSlaveList[DriverId].WordMap[Regaddr], RcvData[7], , SLens);
            break;
        }
    }
       
}


/****************************************************
 * 串口数据处理（集中，分发给从机）
*****************************************************/
void ComDataProcess(const int com, u8 *RcvData, int DataLen)
{
    int i;
    u16 CrcTemp = 0xffff;
    u8 sendData[260] = {0};
    int SendLen = 0;
    /*
    u16 DriverAddr = 0xffff;
    u8  FunctionCode = 0xff;
    u16 Regaddr = 0xffff;
    u16 RegNum  = 0xffff;
    u8  WordNum = 0xff;
    */
    CrcTemp == usMBCRC16(RcvData, DataLen-2);
    if((CrcTemp != (u16)RcvData[DataLen-2]<<8 + (u16)RcvData[DataLen-1])||
       (CrcTemp != (u16)RcvData[DataLen-1]<<8 + (u16)RcvData[DataLen-2]))//CRC检验
    {
        //UartSend(com, "CrcErr\n",8);
        return ;
    }
    for(i=0;i<MAX_MBSlave_NUM; i++)
    {
        if(com == MBSlaveList[i].com)
        {
            if(MBSlaveList[i].Slave->DriverAddr == RcvData[0]) 
            {
                DriverProcess(i, RcvData, DataLen-2, SendData, &SendLen)
                CrcTemp == usMBCRC16(SendData, SendLen);
                SendData[SendLen++] = (u8)(CrcTemp >> 8) | 0xff;
                SendData[SendLen++] = (u8)(CrcTemp) | 0xff;

                //Uart_Send(com, SendData, SendLen);
            }
        }
    }
}



