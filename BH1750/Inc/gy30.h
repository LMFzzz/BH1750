//
// Created by LMF on 2024/1/14.
//

#ifndef BH1750_GY30_H
#define BH1750_GY30_H
#include "delay.h"
//IO����
//#define SDA_IN() {GPIOB->CRL&=0x0FFFFFFF;GPIOB->CRL|=8<<28;}
//#define SDA_OUT() {GPIOB->CRL&=0x0FFFFFFF;GPIOB->CRL|=3<<28;}
#define BH1750_SCL GPIO_PIN_6//SCL
#define BH1750_SDA GPIO_PIN_7//SDA
#define BH1750_Port GPIOB
#define ADDR 0x23
#define BH1750WriteAddress 0x46//д��ַ
#define BH1750ReadAddress 0x47//����ַ
#define BH1750PowDown 0x00//�ر�ģ��
#define BH1750PowOn 0x01//��ģ��
#define BH1750ModeH1 0x10//�߷ֱ���
#define BH1750ModeH2 0x11//�߷ֱ���ģʽ2
#define BH1750ModeL 0x13//�ͷֱ���
#define BH1750SigModeH 0x20//һ�θ߷ֱ���
#define BH1750SigModeH2 0x21
#define BH1750SigModeL 0x23
void Single_Write_BH1750(unsigned char REGAddress);
void BH1750_Init(void );
void BH1750_Send_Data(uint8_t command);
uint16_t BH1750_Read_Data(void );
void BH1750_IIC_Init(void );//��ʼ��IIC��IO��
void BH1750_IIC_Start(void );//����IIC��ʼ�ź�
void BH1750_IIC_Stop(void );//IIC����һ���ֽ�
void BH1750_IIC_Send_Byte(uint8_t txd);//IIC����һ���ֽ�
uint8_t BH1750_IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
uint8_t BH1750_Wait_Ack(void );//IIC�ȴ�ACK�ź�
void BH1750_IIC_Ack(void );//IIC����ACK�ź�
void BH1750_IIC_NAck(void );//IIC������ACK�ź�
void IIC_Write_One_Byte(uint8_t daddr,uint8_t addr,uint8_t data);
uint8_t IIC_Read_One_Byte(uint8_t daddr,uint8_t addr);
#endif //BH1750_GY30_H
