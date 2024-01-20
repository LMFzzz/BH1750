//
// Created by LMF on 2024/1/14.
//
#include "gy30.h"
#include "delay.h"
void SDA_OUT(void ){
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.Pull=BH1750_SDA;
    GPIO_InitStructure.Mode=GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Speed=GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(BH1750_Port,&GPIO_InitStructure);
}
void SDA_IN(void ){
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.Pin=BH1750_SDA;
    GPIO_InitStructure.Speed=GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStructure.Mode=GPIO_MODE_INPUT;
    HAL_GPIO_Init(BH1750_Port,&GPIO_InitStructure);
}
void BH1750_Init(void )
{
    GPIO_InitTypeDef GPIO_InitStructure;
    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitStructure.Pin=BH1750_SCL|BH1750_SDA;
    GPIO_InitStructure.Mode=GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Speed=GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(BH1750_Port,&GPIO_InitStructure);
    HAL_GPIO_WritePin(BH1750_Port,BH1750_SDA,GPIO_PIN_SET);
    HAL_GPIO_WritePin(BH1750_Port,BH1750_SCL,GPIO_PIN_SET);
    BH1750_Send_Data(BH1750PowOn);
}
//产生IIC起始信号
void BH1750_IIC_Start(void ){
    SDA_OUT();
    HAL_GPIO_WritePin(BH1750_Port,BH1750_SDA,GPIO_PIN_SET);
    HAL_GPIO_WritePin(BH1750_Port,BH1750_SCL,GPIO_PIN_SET);
    delay_us(4);
    HAL_GPIO_WritePin(BH1750_Port,BH1750_SDA,GPIO_PIN_RESET);
    delay_us(4);
    HAL_GPIO_WritePin(BH1750_Port,BH1750_SCL,GPIO_PIN_RESET);
}
//产生IIC信号
void BH1750_IIC_Stop(void )
{
    SDA_OUT();
    HAL_GPIO_WritePin(BH1750_Port,BH1750_SCL,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(BH1750_Port,BH1750_SDA,GPIO_PIN_RESET);
    delay_us(4);
    HAL_GPIO_WritePin(BH1750_Port,BH1750_SCL,GPIO_PIN_SET);
    HAL_GPIO_WritePin(BH1750_Port,BH1750_SDA,GPIO_PIN_SET);
    delay_us(4);
}
//等待应答信号
uint8_t BH1750_Wait_Ack(void ){
    uint8_t ucErrTime=0;
    SDA_IN();
    HAL_GPIO_WritePin(BH1750_Port,BH1750_SDA,GPIO_PIN_SET);
    delay_us(1);
    HAL_GPIO_WritePin(BH1750_Port,BH1750_SCL,GPIO_PIN_SET);
    delay_us(1);
    while (HAL_GPIO_ReadPin(BH1750_Port,BH1750_SDA))
    {
        ucErrTime++;
        if (ucErrTime>250)
        {
            BH1750_IIC_Stop();
            return 1;
        }
    }
    HAL_GPIO_WritePin(BH1750_Port,BH1750_SCL,GPIO_PIN_RESET);
    return 0;
}
//产生ACK应答
void BH1750_IIC_Ack(void )
{
    HAL_GPIO_WritePin(BH1750_Port,BH1750_SCL,GPIO_PIN_RESET);
    SDA_OUT();
    HAL_GPIO_WritePin(BH1750_Port,BH1750_SDA,GPIO_PIN_RESET);
    delay_us(2);
    HAL_GPIO_WritePin(BH1750_Port,BH1750_SCL,GPIO_PIN_SET);
    delay_us(2);
    HAL_GPIO_WritePin(BH1750_Port,BH1750_SCL,GPIO_PIN_RESET);
}
//不产生ACK应答
void BH1750_IIC_NAck(void )
{
    HAL_GPIO_WritePin(BH1750_Port,BH1750_SCL,GPIO_PIN_RESET);
    SDA_OUT();
    HAL_GPIO_WritePin(BH1750_Port,BH1750_SDA,GPIO_PIN_SET);
    delay_us(2);
    HAL_GPIO_WritePin(BH1750_Port,BH1750_SCL,GPIO_PIN_SET);
    delay_us(2);
    HAL_GPIO_WritePin(BH1750_Port,BH1750_SCL,GPIO_PIN_RESET);
}
//IIC发送一个字节
void BH1750_IIC_Send_Byte(uint8_t txd)
{
    uint8_t t;
    SDA_OUT();
    HAL_GPIO_WritePin(BH1750_Port,BH1750_SCL,GPIO_PIN_RESET);
    for (t=0;t<8;t++)
    {
        if ((txd&&0x80)>>7)
            HAL_GPIO_WritePin(BH1750_Port,BH1750_SDA,GPIO_PIN_SET);
        else
            HAL_GPIO_WritePin(BH1750_Port,BH1750_SDA,GPIO_PIN_RESET);
        txd<<=1;
        delay_us(2);
        HAL_GPIO_WritePin(BH1750_Port,BH1750_SCL,GPIO_PIN_SET);
        delay_us(2);
        HAL_GPIO_WritePin(BH1750_Port,BH1750_SCL,GPIO_PIN_RESET);
        delay_us(2);
    }
}
//读一个字节
uint8_t BH1750_IIC_Read_Byte(unsigned char ack)
{
    unsigned char i,receive=0;
    SDA_IN();
    for (i=0;i<8;i++)
    {
        HAL_GPIO_WritePin(BH1750_Port,BH1750_SCL,GPIO_PIN_RESET);
        delay_us(2);
        HAL_GPIO_WritePin(BH1750_Port,BH1750_SCL,GPIO_PIN_SET);
        receive<<=1;
        if (HAL_GPIO_ReadPin(BH1750_Port,BH1750_SDA))
            receive++;
        delay_us(1);
    }
    if (!ack)
        BH1750_IIC_NAck();
    else
        BH1750_IIC_Ack();
    return receive;
}
void BH1750_Send_Data(uint8_t command){
    do {
        BH1750_IIC_Start();
        BH1750_IIC_Send_Byte(BH1750WriteAddress);
    } while (BH1750_Wait_Ack());
    BH1750_IIC_Send_Byte(command);
    BH1750_Wait_Ack();
    BH1750_IIC_Stop();
}
uint16_t BH1750_Read_Data(void ){
    uint16_t buf;
    BH1750_IIC_Start();
    BH1750_IIC_Send_Byte(BH1750ReadAddress);
    BH1750_Wait_Ack();
    buf= BH1750_IIC_Read_Byte(1);
    buf=buf<<8;
    buf+=0x00ff& BH1750_IIC_Read_Byte(0);
    BH1750_IIC_Stop();
    return buf;
}