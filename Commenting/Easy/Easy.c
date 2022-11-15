#include <math.h>
#include "mpu6050.h"

#define RAD2DEG 57.295779513082320876798154814105 // Radian to Degree Multiplier Constant
#define WHOAMI 0x75 // WHOAMI Register
#define PWR_MGMT 0x6B // PWR_MGMT Register
#define SMPLRT_DIV 0x19 // SMPLRT_DIV Register
#define ACCEL_CONFIG 0x1C
#define ACCEL_XOUT_H 0x3B // ACCEL_XOUT_H Register to fetch sensor Raw accel data
#define TEMP_OUT_H 0x41 // TEMP_OUT_H Register to fetch sensor temp data
#define GYRO_CONFIG 0x1B // GYRO_CONFIG Register
#define GYRO_XOUT_H 0x43 // GYRO_XOUT_H Register to fetch sensor Raw gyro data
#define MPU6050_ADDR 0xD0 // MPU6050 I2C Device address
const uint16_t I2C_TIMEOUT = 100; // Timeout value constant
const double Acc_Z_corrector = 14418.0; // Accelerometer Z axis correction
uint32_t timer; // timer counter

Filter_t FilterX = { // Filter Type Defined for the X axis
    .Q_ANGLE = 0.001f,  // noise variance for the accel X
    .Q_BIAS = 0.003f,   // noise variance for the gyro Y
    .R_MEASURE = 0.03f}; // measurement noise variance  

Filter_t FilterY = { // Filter Type Defined for the Y axis
    .Q_ANGLE = 0.001f,  // noise variance for the accel Y
    .Q_BIAS = 0.003f,   // noise variance for the gyro Y
    .R_MEASURE = 0.03f,  // measurement noise variance  
};

uint8_t MPU_Init(I2C_HandleTypeDef *I2Cx) // Initialization of the MPU6050 Module
{
    uint8_t check;  // Hold WHOAMI register value
    uint8_t Data;   // Holds data to be written in different I2C registers

    HAL_I2C_Mem_Read(I2Cx, MPU6050_ADDR, WHOAMI, 1, &check, 1, I2C_TIMEOUT); // Read 1 Byte from WHOAMI Register 

    if (check == 104) // Make sure WHOAMI register is correct
    {
        Data = 0;
        HAL_I2C_Mem_Write(I2Cx, MPU6050_ADDR, PWR_MGMT, 1, &Data, 1, I2C_TIMEOUT); // Write 1 Byte to PWR_MGMT Register

        Data = 0x07;
        HAL_I2C_Mem_Write(I2Cx, MPU6050_ADDR, SMPLRT_DIV, 1, &Data, 1, I2C_TIMEOUT); // Write 1 Byte to SMPLRT_DIV Register

        Data = 0x00;
        HAL_I2C_Mem_Write(I2Cx, MPU6050_ADDR, ACCEL_CONFIG, 1, &Data, 1, I2C_TIMEOUT); // Write 1 Byte to ACCEL_CONFIG Register

        Data = 0x00;
        HAL_I2C_Mem_Write(I2Cx, MPU6050_ADDR, GYRO_CONFIG, 1, &Data, 1, I2C_TIMEOUT); // Write 1 Byte to GYRO_CONFIG Register
        return 0; // Return ZERO on success 
    }
    return 1; // Return NON-ZERO on failure
}

void Read_Accel(I2C_HandleTypeDef *I2Cx, MPU6050_t *DataStruct) // Read Accel values 
{
    uint8_t Rec_Data[6]; // Buffer for holding received data

    HAL_I2C_Mem_Read(I2Cx, MPU6050_ADDR, ACCEL_XOUT_H, 1, Rec_Data, 6, I2C_TIMEOUT); // Read 6 Bytes from ACCEL_XOUT_H Register

    DataStruct->Accel_X_RAW = (int16_t)(Rec_Data[0] << 8 | Rec_Data[1]); // Reconstruct and Save to Accel_X_RAW struct member the received raw value of Accel_X
    DataStruct->Accel_Y_RAW = (int16_t)(Rec_Data[2] << 8 | Rec_Data[3]); // Reconstruct and Save to Accel_Y_RAW struct member the received raw value of Accel_Y
    DataStruct->Accel_Z_RAW = (int16_t)(Rec_Data[4] << 8 | Rec_Data[5]); // Reconstruct and Save to Accel_Z_RAW struct member the received raw value of Accel_Z

    DataStruct->Ax = DataStruct->Accel_X_RAW / 16384.0; // Save to Ax the Ax value after correction
    DataStruct->Ay = DataStruct->Accel_Y_RAW / 16384.0; // Save to Ay the Ay value after correction 
    DataStruct->Az = DataStruct->Accel_Z_RAW / Acc_Z_corrector; // Save to Az the Az value after correction
}
 
void Read_Gyro(I2C_HandleTypeDef *I2Cx, MPU6050_t *DataStruct) // Read Gyro values 
{
    uint8_t Rec_Data[6]; // Buffer for holding received data

    HAL_I2C_Mem_Read(I2Cx, MPU6050_ADDR, GYRO_XOUT_H, 1, Rec_Data, 6, I2C_TIMEOUT); // Read 6 Bytes from GYRO_XOUT_H Register

    DataStruct->Gyro_X_RAW = (int16_t)(Rec_Data[0] << 8 | Rec_Data[1]); // Reconstruct and Save to Gyro_X_RAW struct member the received raw value of Gyro_X
    DataStruct->Gyro_Y_RAW = (int16_t)(Rec_Data[2] << 8 | Rec_Data[3]); // Reconstruct and Save to Gyro_Y_RAW struct member the received raw value of Gyro_Y
    DataStruct->Gyro_Z_RAW = (int16_t)(Rec_Data[4] << 8 | Rec_Data[5]); // Reconstruct and Save to Gyro_Z_RAW struct member the received raw value of Gyro_Z

    DataStruct->Gx = DataStruct->Gyro_X_RAW / 131.0;  // Save to Gx the Gx value after correction
    DataStruct->Gy = DataStruct->Gyro_Y_RAW / 131.0;  // Save to Gy the Gy value after correction
    DataStruct->Gz = DataStruct->Gyro_Z_RAW / 131.0;  // Save to Gz the Gz value after correction
}

void Read_Temp(I2C_HandleTypeDef *I2Cx, MPU6050_t *DataStruct) // Read Temp 
{
    uint8_t Rec_Data[2]; //  Buffer for holding received data
    int16_t temp; // buffer to create temp from raw data

    HAL_I2C_Mem_Read(I2Cx, MPU6050_ADDR, TEMP_OUT_H, 1, Rec_Data, 2, I2C_TIMEOUT); // Read 2 Bytes from TEMP_OUT_H Register

    temp = (int16_t)(Rec_Data[0] << 8 | Rec_Data[1]); // Reconstruct and Save to temp the received temp data
    DataStruct->Temperature = (float)((int16_t)temp / (float)340.0 + (float)36.53); // Save the corrected temp to Temperature struct member
}

void Read_All(I2C_HandleTypeDef *I2Cx, MPU6050_t *DataStruct) // Read Accel, Gyro and Temp at once from Sensor
{
    uint8_t Rec_Data[14]; //  Buffer for holding received data
    int16_t temp; // buffer to create temp from raw data

    HAL_I2C_Mem_Read(I2Cx, MPU6050_ADDR, ACCEL_XOUT_H, 1, Rec_Data, 14, I2C_TIMEOUT);  // Read 14 Bytes from sensor starting from ACCEL_XOUT_H register
    /* reconstruct the received data and save it */
    DataStruct->Accel_X_RAW = (int16_t)(Rec_Data[0] << 8 | Rec_Data[1]);
    DataStruct->Accel_Y_RAW = (int16_t)(Rec_Data[2] << 8 | Rec_Data[3]);
    DataStruct->Accel_Z_RAW = (int16_t)(Rec_Data[4] << 8 | Rec_Data[5]);
    temp = (int16_t)(Rec_Data[6] << 8 | Rec_Data[7]);
    DataStruct->Gyro_X_RAW = (int16_t)(Rec_Data[8] << 8 | Rec_Data[9]);
    DataStruct->Gyro_Y_RAW = (int16_t)(Rec_Data[10] << 8 | Rec_Data[11]);
    DataStruct->Gyro_Z_RAW = (int16_t)(Rec_Data[12] << 8 | Rec_Data[13]);
    /* Apply correction on the saved data and save it */
    DataStruct->Ax = DataStruct->Accel_X_RAW / 16384.0;
    DataStruct->Ay = DataStruct->Accel_Y_RAW / 16384.0;
    DataStruct->Az = DataStruct->Accel_Z_RAW / Acc_Z_corrector;
    DataStruct->Temperature = (float)((int16_t)temp / (float)340.0 + (float)36.53);
    DataStruct->Gx = DataStruct->Gyro_X_RAW / 131.0;
    DataStruct->Gy = DataStruct->Gyro_Y_RAW / 131.0;
    DataStruct->Gz = DataStruct->Gyro_Z_RAW / 131.0;

    double dt = (double)(HAL_GetTick() - timer) / 1000; // calculate delta t
    timer = HAL_GetTick(); // update timer
    double roll;
    double roll_sqrt = sqrt( 
    /**
    * calculate: sqrt roll =  Accel_X_RAW^2 + Accel_Z_RAW^2
    **/ 
        DataStruct->Accel_X_RAW * DataStruct->Accel_X_RAW + DataStruct->Accel_Z_RAW * DataStruct->Accel_Z_RAW);
    if (roll_sqrt != 0.0) 
    {
    /** 
    * if sqrt roll != 0, calculate: roll = atan( Accel_Y_RAW / sqrt roll ) and convert it to degrees 
    **/
        roll = atan(DataStruct->Accel_Y_RAW / roll_sqrt) * RAD2DEG;
    }
    else
    {
      /** 
      * sqrt roll = 0, calculate: roll = 0 (prevent div by zero) 
      **/
        roll = 0.0;
    }
    /** 
    * calculate: pitch = atan2( Accel_X_RAW, Accel_Z_RAW ) and convert it to degrees 
    **/
    double pitch = atan2(-DataStruct->Accel_X_RAW, DataStruct->Accel_Z_RAW) * RAD2DEG;
    if ((pitch < -90 && DataStruct->FilterAngleY > 90) || (pitch > 90 && DataStruct->FilterAngleY < -90))
    {
    /** 
    * in case `pitch < -90 degree AND  FilterAngleY > 90` OR `pitch > 90 AND FilterAngleY < -90`
    * fixes the transition problem when the accelerometer angle jumps between -180 and 180 degrees
    **/
        FilterY.angle = pitch; // Update the FilterY's angle
        DataStruct->FilterAngleY = pitch; // Update the FilterAngleY
    }
    else
    {
    /** 
    * Otherwise, we should rely on the Filter 
    * Calculate the angle using a thr Filter
    **/
        DataStruct->FilterAngleY = Filter_getAngle(&FilterY, pitch, DataStruct->Gy, dt); // get an approximate angle from the Filter 
    }
    if (fabs(DataStruct->FilterAngleY) > 90)
        DataStruct->Gx = -DataStruct->Gx; // Invert rate, so it fits the restriced accelerometer reading
    DataStruct->FilterAngleX = Filter_getAngle(&FilterX, roll, DataStruct->Gx, dt);
}

double Filter_getAngle(Filter_t *Filter, double newAngle, double newRate, double dt)
{
    /**
    * 
    * Kallman Filter 
    * See WIKI https://en.wikipedia.org/wiki/Kalman_filter
    **/

    // Discrete Kalman filter time update equations - Time Update ("Predict")

    // Update xhat - Project the state ahead
    /* Step 1 */ 
    double rate = newRate - Filter->bias;
    Filter->angle += dt * rate;
    // Update estimation error covariance - Project the error covariance ahead
    /* Step 2 */
    Filter->P[0][0] += dt * (dt * Filter->P[1][1] - Filter->P[0][1] - Filter->P[1][0] + Filter->Q_ANGLE);
    Filter->P[0][1] -= dt * Filter->P[1][1];
    Filter->P[1][0] -= dt * Filter->P[1][1];
    Filter->P[1][1] += Filter->Q_BIAS * dt;
  // Discrete Kalman filter measurement update equations - Measurement Update ("Correct")
  // Calculate Kalman gain - Compute the Kalman gain
  /* Step 4 */

    double S = Filter->P[0][0] + Filter->R_MEASURE;
      /* Step 5 */
    double K[2];
    K[0] = Filter->P[0][0] / S;
    K[1] = Filter->P[1][0] / S;
  // Calculate angle and bias - Update estimate with measurement zk (newAngle)
  /* Step 3 */


    double y = newAngle - Filter->angle;
      /* Step 6 */

    Filter->angle += K[0] * y;
    Filter->bias += K[1] * y;
  // Calculate estimation error covariance - Update the error covariance
  /* Step 7 */

    double P00_temp = Filter->P[0][0];
    double P01_temp = Filter->P[0][1];

    Filter->P[0][0] -= K[0] * P00_temp;
    Filter->P[0][1] -= K[0] * P01_temp;
    Filter->P[1][0] -= K[1] * P00_temp;
    Filter->P[1][1] -= K[1] * P01_temp;

    return Filter->angle;
};