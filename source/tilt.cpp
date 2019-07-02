#include "tilt.h"

using namespace std;

Tilt::Tilt()
{
    _mpuInterrupt = false;
    _slidingWindowSize = 20;
    _tiltValues = NULL;
}

Tilt::~Tilt()
{
}

void Tilt::DmpDataReady()
{
    _mpuInterrupt = true;
}

void Tilt::Setup()
{
    // join I2C bus (I2Cdev library doesn't do this automatically)
    Wire.begin();
    Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties

    // initialize device
    Serial.println(F("Initializing I2C devices..."));
    _mpu.initialize();

    // verify connection
    Serial.println(F("Testing device connections..."));
    Serial.println(_mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));

    // load and configure the DMP
    Serial.println(F("Initializing DMP..."));
    _devStatus = _mpu.dmpInitialize();

    // supply your own gyro offsets here, scaled for min sensitivity
    _mpu.setXGyroOffset(220);
    _mpu.setYGyroOffset(76);
    _mpu.setZGyroOffset(-85);
    _mpu.setZAccelOffset(1788); // 1688 factory default for my test chip

    // make sure it worked (returns 0 if so)
    if (_devStatus == 0)
    {
        // turn on the DMP, now that it's ready
        Serial.println(F("Enabling DMP..."));
        _mpu.setDMPEnabled(true);

        // enable Arduino interrupt detection
        Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));

        _mpuIntStatus = _mpu.getIntStatus();

        // set our DMP Ready flag so the main loop() function knows it's okay to use it
        Serial.println(F("DMP ready! Waiting for first interrupt..."));
        _dmpReady = true;

        // get expected DMP packet size for later comparison
        _packetSize = _mpu.dmpGetFIFOPacketSize();
    }
    else
    {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        Serial.print(F("DMP Initialization failed (code "));
        Serial.print(_devStatus);
        Serial.println(F(")"));
    }
}

void Tilt::Read()
{
    // if programming failed, don't try to do anything
    if (!_dmpReady)
        return;

    // wait for MPU interrupt or extra packet(s) available
    if (!_mpuInterrupt && _fifoCount < _packetSize)
        return;

    // reset interrupt flag and get INT_STATUS byte
    _mpuInterrupt = false;
    _mpuIntStatus = _mpu.getIntStatus();

    // get current FIFO count
    _fifoCount = _mpu.getFIFOCount();
    
    // check for overflow (this should never happen unless our code is too inefficient)
    if ((_mpuIntStatus & 0x10) || _fifoCount == 1024)
    {
        // reset so we can continue cleanly
        _mpu.resetFIFO();
        Serial.println(F("FIFO overflow!"));

        // otherwise, check for DMP data ready interrupt (this should happen frequently)
    }
    else if (_mpuIntStatus & 0x02)
    {
        // wait for correct available data length, should be a VERY short wait
        while (_fifoCount < _packetSize)
            _fifoCount = _mpu.getFIFOCount();

        // read a packet from FIFO
        _mpu.getFIFOBytes(_fifoBuffer, _packetSize);

        // track FIFO count here in case there is > 1 packet available
        // (this lets us immediately read more without waiting for an interrupt)
        _fifoCount -= _packetSize;

        _mpu.dmpGetQuaternion(&_q, _fifoBuffer);
        _mpu.dmpGetGravity(&_gravity, &_q);
        _mpu.dmpGetYawPitchRoll(_ypr, &_q, &_gravity);

        float pitch = _ypr[1] * 180/M_PI;
        float roll = _ypr[2] * 180/M_PI;

        _tilt = sqrt(pitch * pitch + roll * roll);        

        TiltValue *v = new TiltValue();
        v->value = _tilt; 

        if(_tiltValues == NULL)
        {
            _tiltValues = v;
            return;
        }

        //Add to end
        TiltValue *n = _tiltValues;
        int count = 1;

        while(n->next != NULL)
        {
            n = n->next;
            count++;
        }

        n->next = v;

        //remove first item if above sliding window size
        if(count > _slidingWindowSize)
        {
            TiltValue *itemToRemove = _tiltValues;
            _tiltValues = _tiltValues->next;
            delete itemToRemove;        
        }
    }
}

float Tilt::GetTilt()
{
    if(_tiltValues == NULL)
    {
        return 0;
    }

    TiltValue *n = _tiltValues;

    float sum = n->value;
    int count = 1;

    while(n->next != NULL)
    {
        n = n->next;
        sum += n->value;
        count++;
    }

    return sum / count;
}

void Tilt::Print()
{
    Serial.print("tilt: ");
    Serial.println(GetTilt());
}
