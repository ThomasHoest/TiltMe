#ifndef tilt_h
#define tilt_h

#include <Wire.h>
#include <MPU6050.h>

class Tilt
{
    public:
    
    Tilt();
    ~Tilt();    

    void Setup();
    void Read();
    void Print();
    void DmpDataReady();
    
    private:
    
    volatile bool _mpuInterrupt;     // indicates whether MPU interrupt pin has gone high

    MPU6050 _mpu;
    bool _dmpReady = false;  // set true if DMP init was successful
    uint8_t _mpuIntStatus;   // holds actual interrupt status byte from MPU
    uint8_t _devStatus;      // return status after each device operation (0 = success, !0 = error)
    uint16_t _packetSize;    // expected DMP packet size (default is 42 bytes)
    uint16_t _fifoCount;     // count of all bytes currently in FIFO
    uint8_t _fifoBuffer[64]; // FIFO storage buffer

    Quaternion _q;           // [w, x, y, z]         quaternion container
    VectorInt16 _aa;         // [x, y, z]            accel sensor measurements
    VectorInt16 _aaReal;     // [x, y, z]            gravity-free accel sensor measurements
    VectorInt16 _aaWorld;    // [x, y, z]            world-frame accel sensor measurements
    VectorFloat _gravity;    // [x, y, z]            gravity vector

};

#endif