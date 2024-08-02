#include "include.h"

void Get_Car_Angle(void)           //获取陀螺仪角度（完成）
{
    static uint8 IsAttitudeinit = 0;
    BMX055_DataRead(&BMX055_data, 0);
    GYRO_Speed_Z=(BMX055_data.GYROZdata - GyroOffset.Zdata);
    BMX055_data.GYROXdata = (BMX055_data.GYROXdata - GyroOffset.Xdata) * 0.030517578;   //下面这六段就是单位换算
    BMX055_data.GYROYdata = (BMX055_data.GYROYdata - GyroOffset.Ydata) * 0.030517578;
    BMX055_data.GYROZdata = (BMX055_data.GYROZdata - GyroOffset.Zdata) * 0.030517578;
  ///////1000 / 32768     //////BMX055本身零飘几乎可以忽略不计，但是安全起见还是矫正一下
    BMX055_data.ACCXdata *= 0.001953125;    ///////4 / 2048
    BMX055_data.ACCYdata *= 0.001953125;
    BMX055_data.ACCZdata *= 0.001953125;
   
    Acc.Xdata = BMX055_data.ACCXdata;
    Acc.Ydata = BMX055_data.ACCYdata;
    Acc.Zdata = BMX055_data.ACCZdata;
    Gyro.Xdata = BMX055_data.GYROXdata;
    Gyro.Ydata = BMX055_data.GYROYdata;
    Gyro.Zdata = BMX055_data.GYROZdata;  
    if(IsAttitudeinit == 0)
    {
      Quaternion_init();                    ////姿态解算初始化        
      IsAttitudeinit = 1;
    }
    else
    {
      Attitude_UpdateGyro();                /////快速更新
      Attitude_UpdateAcc();                 /////深度融合更新
      SystemAttitude.Pitch = -EulerAngle.Roll / PI * 180;         ////转为角度
      SystemAttitude.Roll = EulerAngle.Pitch / PI * 180;
      SystemAttitude.Yaw = EulerAngle.Yaw / PI * 180;
      SystemAttitudeRate.Pitch = -EulerAngleRate.Roll/ PI * 180;
      SystemAttitudeRate.Roll = EulerAngleRate.Pitch / PI * 180;
      SystemAttitudeRate.Yaw = EulerAngleRate.Yaw / PI * 180;//正负号需要根据芯片摆放修改   
    }
      Negative_treatment();//把角度转成0~360（原来是-180 ~ 180）
    
}
