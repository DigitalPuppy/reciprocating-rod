
#include <Seeed_vl53l0x.h>


// Work in progress for direct arduino rod control. Will require USB shield.


//https://github.com/alexgorbatchev/crc/blob/master/src/calculators/crc16modbus.ts
uint16_t table[] = {
  0x0000, 0xc0c1, 0xc181, 0x0140, 0xc301, 0x03c0, 0x0280, 0xc241, 0xc601, 0x06c0, 0x0780, 0xc741,
  0x0500, 0xc5c1, 0xc481, 0x0440, 0xcc01, 0x0cc0, 0x0d80, 0xcd41, 0x0f00, 0xcfc1, 0xce81, 0x0e40,
  0x0a00, 0xcac1, 0xcb81, 0x0b40, 0xc901, 0x09c0, 0x0880, 0xc841, 0xd801, 0x18c0, 0x1980, 0xd941,
  0x1b00, 0xdbc1, 0xda81, 0x1a40, 0x1e00, 0xdec1, 0xdf81, 0x1f40, 0xdd01, 0x1dc0, 0x1c80, 0xdc41,
  0x1400, 0xd4c1, 0xd581, 0x1540, 0xd701, 0x17c0, 0x1680, 0xd641, 0xd201, 0x12c0, 0x1380, 0xd341,
  0x1100, 0xd1c1, 0xd081, 0x1040, 0xf001, 0x30c0, 0x3180, 0xf141, 0x3300, 0xf3c1, 0xf281, 0x3240,
  0x3600, 0xf6c1, 0xf781, 0x3740, 0xf501, 0x35c0, 0x3480, 0xf441, 0x3c00, 0xfcc1, 0xfd81, 0x3d40,
  0xff01, 0x3fc0, 0x3e80, 0xfe41, 0xfa01, 0x3ac0, 0x3b80, 0xfb41, 0x3900, 0xf9c1, 0xf881, 0x3840,
  0x2800, 0xe8c1, 0xe981, 0x2940, 0xeb01, 0x2bc0, 0x2a80, 0xea41, 0xee01, 0x2ec0, 0x2f80, 0xef41,
  0x2d00, 0xedc1, 0xec81, 0x2c40, 0xe401, 0x24c0, 0x2580, 0xe541, 0x2700, 0xe7c1, 0xe681, 0x2640,
  0x2200, 0xe2c1, 0xe381, 0x2340, 0xe101, 0x21c0, 0x2080, 0xe041, 0xa001, 0x60c0, 0x6180, 0xa141,
  0x6300, 0xa3c1, 0xa281, 0x6240, 0x6600, 0xa6c1, 0xa781, 0x6740, 0xa501, 0x65c0, 0x6480, 0xa441,
  0x6c00, 0xacc1, 0xad81, 0x6d40, 0xaf01, 0x6fc0, 0x6e80, 0xae41, 0xaa01, 0x6ac0, 0x6b80, 0xab41,
  0x6900, 0xa9c1, 0xa881, 0x6840, 0x7800, 0xb8c1, 0xb981, 0x7940, 0xbb01, 0x7bc0, 0x7a80, 0xba41,
  0xbe01, 0x7ec0, 0x7f80, 0xbf41, 0x7d00, 0xbdc1, 0xbc81, 0x7c40, 0xb401, 0x74c0, 0x7580, 0xb541,
  0x7700, 0xb7c1, 0xb681, 0x7640, 0x7200, 0xb2c1, 0xb381, 0x7340, 0xb101, 0x71c0, 0x7080, 0xb041,
  0x5000, 0x90c1, 0x9181, 0x5140, 0x9301, 0x53c0, 0x5280, 0x9241, 0x9601, 0x56c0, 0x5780, 0x9741,
  0x5500, 0x95c1, 0x9481, 0x5440, 0x9c01, 0x5cc0, 0x5d80, 0x9d41, 0x5f00, 0x9fc1, 0x9e81, 0x5e40,
  0x5a00, 0x9ac1, 0x9b81, 0x5b40, 0x9901, 0x59c0, 0x5880, 0x9841, 0x8801, 0x48c0, 0x4980, 0x8941,
  0x4b00, 0x8bc1, 0x8a81, 0x4a40, 0x4e00, 0x8ec1, 0x8f81, 0x4f40, 0x8d01, 0x4dc0, 0x4c80, 0x8c41,
  0x4400, 0x84c1, 0x8581, 0x4540, 0x8701, 0x47c0, 0x4680, 0x8641, 0x8201, 0x42c0, 0x4380, 0x8341,
  0x4100, 0x81c1, 0x8081, 0x4040,
};


uint16_t crc16modbus(uint8_t* data, int len)
{
  uint16_t crc = 0xFFFF;
  for (int index=0;index<len;index++)
  {
    crc = (table[(crc ^ data[index]) & 0xFF] ^ (crc >> 8)) & 0xffff;
  }
  return crc;
}

void rodSetPosition(uint32_t targetPosition)
{
  uint8_t buffer[13] = { 1, 0x10, 0x99, 0, 0, 2, 4};
  buffer[7] = targetPosition >> 24;
  buffer[8] = (targetPosition >> 16) & 0xFF;
  buffer[9] = (targetPosition >> 8) & 0xFF;
  buffer[10] = (targetPosition) & 0xFF;
  auto crc = crc16modbus(buffer, 11);
  buffer[12] = crc >> 8;
  buffer[11] = crc & 0xFF;
  Serial.write(buffer, 13);
}

/**
 * Positioning Data Direct Writing (Queries Using Code 10))
 * Produces a reponse of size 8 bytes;
 * VCMD Speed specification register (2 byte in 0.01 mm/sec) Writing 3 registers, each 2 bytes
 * ACMD Acceleration/deceleration specification register (1 byte in 0.01 G)
 * @param targetPosition target position in mm/100
 * @param targetPositionBand in mm/100 (default is 0.1 mm)
 * @param velocity in mm/100 (good value is 10 000)
 * @param acceleration in g/100 (valid is [1, 300], good value is 30)
 * @param pushCurrentLimitingValue Set the current limit during push-motion operation in PPOW. Range: [51-178] (equiv of 20% to 70% of 255). 0 means no limit.
 * @param controlFlags
 */
void rodSet(uint32_t targetPosition, uint32_t velocity, uint16_t acceleration)
{
  uint8_t buffer[23] = { 1, 0x10, 0x99, 0, 0, 7, 0x0E};
  buffer[7] = targetPosition >> 24;
  buffer[8] = (targetPosition >> 16) & 0xFF;
  buffer[9] = (targetPosition >> 8) & 0xFF;
  buffer[10] = (targetPosition) & 0xFF;
  buffer[11] = buffer[12] = buffer[13] = 0;
  buffer[14] = 10;
  buffer[15] = velocity >> 24;
  buffer[16] = (velocity >> 16) & 0xFF;
  buffer[17] = (velocity >> 8) & 0xFF;
  buffer[18] = (velocity) & 0xFF;
  buffer[19] = acceleration >> 8;
  buffer[20] = acceleration & 0xFF;
  auto crc = crc16modbus(buffer, 21);
  buffer[22] = crc >> 8; // crc has reverse endianess!!
  buffer[21] = crc & 0xFF;
  Serial.write(buffer, 23);
}

void forceSingleCoil(uint16_t address, uint16_t data)
{
  uint8_t buffer[8] = {1, 5};
  buffer[2] = (address  >> 8) & 0xFF;
  buffer[3] = address & 0xFF;
  buffer[4] = (data >> 8) & 0xFF;
  buffer[5]  = data & 0xFF;
  auto crc = crc16modbus(buffer, 6);
  buffer[7] = crc >> 8;
  buffer[6] = crc & 0xFF;
  Serial.write(buffer, 8);
}
void resetAlarm()
{
  char buf[8];
  forceSingleCoil(0x0407, 0xFF00);
  Serial.readBytes(buf, 8);
  forceSingleCoil(0x0407, 0);
  Serial.readBytes(buf, 8);
}
void pioModbusOn()
{
  forceSingleCoil(0x0427, 0xFF00);
  char buf[8];
  Serial.readBytes(buf, 8);
}
void setServo(bool on)
{
  forceSingleCoil(0x0403, on ? 0xFF00 : 0x0000);
  char buf[8];
  Serial.readBytes(buf, 8);
}
void queryStatusRegister()
{
  //queryHoldingRegisters(0x9000, 10);
  uint16_t address = 0x9000;
  uint16_t data = 10;
  uint8_t buffer[8] = {1, 3};
  buffer[2] = (address  >> 8) & 0xFF;
  buffer[3] = address & 0xFF;
  buffer[4] = (data >> 8) & 0xFF;
  buffer[5]  = data & 0xFF;
  auto crc = crc16modbus(buffer, 6);
  buffer[7] = crc >> 8;
  buffer[6] = crc & 0xFF;
  Serial.write(buffer, 8);
  char repl[25];
  Serial.readBytes(repl, 25);
}
void rodInit()
{
  resetAlarm(); 
  pioModbusOn();
  setServo(true);
  //queryStatusRegister();
}

Seeed_vl53l0x VL53L0X;

#ifdef ARDUINO_SAMD_VARIANT_COMPLIANCE
    #define SERIAL SerialUSB
#else
    #define SERIAL Serial
#endif



void setup() {
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    SERIAL.begin(115200);
    Status = VL53L0X.VL53L0X_common_init();
    if (VL53L0X_ERROR_NONE != Status) {
        SERIAL.println("start vl53l0x mesurement failed!");
        VL53L0X.print_pal_error(Status);
        while (1);
    }
    VL53L0X.VL53L0X_continuous_ranging_init();
    if (VL53L0X_ERROR_NONE != Status) {
        SERIAL.println("start vl53l0x mesurement failed!");
        VL53L0X.print_pal_error(Status);
        while (1);
    }
}

#define AC 3 // number of measurements to average
int32_t valSum = 0;
int acPos = 0;
void loop() {
    VL53L0X_RangingMeasurementData_t RangingMeasurementData;
    VL53L0X.PerformContinuousRangingMeasurement(&RangingMeasurementData);
    
    if (RangingMeasurementData.RangeMilliMeter >= 2000) {
        SERIAL.print('@');
        SERIAL.println(-1);
    } else {
        //SERIAL.print("distance::");
        valSum += RangingMeasurementData.RangeMilliMeter;
        if (++acPos == AC)
        {
          SERIAL.print('@');
          SERIAL.println(valSum / AC);
          acPos = 0;
          valSum = 0;
        }
    }
    delay(10);
}

/* Version that sends raw data: too noisy
void loopRaw() {
    VL53L0X_RangingMeasurementData_t RangingMeasurementData;
    VL53L0X.PerformContinuousRangingMeasurement(&RangingMeasurementData);
    SERIAL.print('@');
    if (RangingMeasurementData.RangeMilliMeter >= 2000) {
        SERIAL.println(-1);
    } else {
        //SERIAL.print("distance::");
        SERIAL.println(RangingMeasurementData.RangeMilliMeter);
    }
    delay(10);
}*/

/* version with precision measurement (too slow)
void setupPREC() {
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    SERIAL.begin(115200);
    Status = VL53L0X.VL53L0X_common_init();
    if (VL53L0X_ERROR_NONE != Status) {
        SERIAL.println("start vl53l0x mesurement failed!");
        VL53L0X.print_pal_error(Status);
        while (1);
    }

    VL53L0X.VL53L0X_high_accuracy_ranging_init();

    if (VL53L0X_ERROR_NONE != Status) {
        SERIAL.println("start vl53l0x mesurement failed!");
        VL53L0X.print_pal_error(Status);
        while (1);
    }
}


void loopPREC() {
    VL53L0X_RangingMeasurementData_t RangingMeasurementData;
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;

    memset(&RangingMeasurementData, 0, sizeof(VL53L0X_RangingMeasurementData_t));
    Status = VL53L0X.PerformSingleRangingMeasurement(&RangingMeasurementData);
    if (VL53L0X_ERROR_NONE == Status) {
      SERIAL.print('@');
        if (RangingMeasurementData.RangeMilliMeter >= 2000) {
            SERIAL.println("-1");
        } else {
            //SERIAL.print("Measured distance:");
            SERIAL.println(RangingMeasurementData.RangeMilliMeter);
            //SERIAL.println(" mm");
        }
    } else {
        //SERIAL.print("mesurement failed !! Status code =");
        //SERIAL.println(Status);
    }

    //delay(300);
}*/
