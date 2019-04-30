/*
版权声明：
深圳市小二极客科技（小R科技）旗下：WIFI机器人网·机器人创意工作室版权所有www.wifi-robots.com
您可以任意修改本程序，并应用于自行研发的智能小车机器人及其他电子产品上，但是禁止用于商业牟利。
小R科技保留付诸法律起诉侵权的权利！2016-4-1
BY WIFI机器人网·机器人创意工作室
*/
#ifndef __SERVO_H
#define __SERVO_H

#include "stm32f10x.h"
#include "timer.h"
void SetServoAngle(u16 servoNum,u8 Angle);
void Init_Steer(void);
void Cruising(void);

#endif /* __SERVO_H */

