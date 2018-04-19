//
// Created by Alexis Lina on 19/04/2018.
//

#include <unistd.h>
#include <math.h>
#include "MQ2GasSensor.hpp"


athome::sensor::MQ2GasSensor::MQ2GasSensor(int pin) : _pin(pin),
                                                      _sampleValue(false),
                                                      _LPGCurve({2.3, 0.21, static_cast<float>(-0.47)}),
                                                      _COCurve({2.3, 0.72, static_cast<float>(-0.34)}),
                                                      _SmokeCurve({2.3, 0.53, static_cast<float>(-0.44)}),
                                                      _R0(10)
{
}

athome::sensor::MQ2GasSensor::~MQ2GasSensor()
{
}

void athome::sensor::MQ2GasSensor::setpin(int pin) {
    _pin = pin;
}

int athome::sensor::MQ2GasSensor::getpin() const {
    return _pin;
}

/****************** MQResistanceCalculation ****************************************
Input:   raw_adc - raw value read from adc, which represents the voltage
Output:  the calculated sensor resistance
Remarks: The sensor and the load resistor forms a voltage divider. Given the voltage
         across the load resistor and its resistance, the resistance of the sensor
         could be derived.
************************************************************************************/
float athome::sensor::MQ2GasSensor::MQResistanceCalculation(int raw_adc) {
    return ((static_cast<float>RL_value*(1023-raw_adc)/raw_adc));
}

/***************************** MQCalibration ****************************************
Input:   mq_pin - analog channel
Output:  Ro of the sensor
Remarks: This function assumes that the sensor is in clean air. It use
         MQResistanceCalculation to calculates the sensor resistance in clean air
         and then divides it with RO_CLEAN_AIR_FACTOR. RO_CLEAN_AIR_FACTOR is about
         10, which differs slightly between different sensors.
************************************************************************************/
float athome::sensor::MQ2GasSensor::MQCalibration(int mqpin) {
    float val = 0;

    for (int j = 0; j < CALIBARAION_SAMPLE_TIMES; j++) {
        val += MQResistanceCalculation(digitalRead(_pin));
        usleep(CALIBRATION_SAMPLE_INTERVAL);
    }
    val = val / CALIBARAION_SAMPLE_TIMES;
    val = static_cast<float>(val / R0_CLEAN_AIR_FACTOR);
    return val;
}

/*****************************  MQRead *********************************************
Input:   mq_pin - analog channel
Output:  Rs of the sensor
Remarks: This function use MQResistanceCalculation to caculate the sensor resistenc (Rs).
         The Rs changes as the sensor is in the different consentration of the target
         gas. The sample times and the time interval between samples could be configured
         by changing the definition of the macros.
************************************************************************************/
float athome::sensor::MQ2GasSensor::MQRead(int mq_pin) {
    float   rs = 0;

    for (int j = 0; j < READ_SAMPLE_TIMES; ++j) {
        rs += MQResistanceCalculation(digitalRead(_pin));
        usleep(READ_SAMPLE_INTERVAL);
    }
    rs = rs / READ_SAMPLE_TIMES;
    return rs;
}

/*****************************  MQGetGasPercentage **********************************
Input:   rs_ro_ratio - Rs divided by Ro
         gas_id      - target gas type
Output:  ppm of the target gas
Remarks: This function passes different curves to the MQGetPercentage function which
         calculates the ppm (parts per million) of the target gas.
************************************************************************************/
int athome::sensor::MQ2GasSensor::MQGetGasPercentage(float rs_ro_ratio, int gas_id) {
    if (gas_id == GAS_LPG){
        return MQGetPercentage(rs_ro_ratio, const_cast<float *>(_LPGCurve));
    }
    else if (gas_id == GAS_CO){
        return MQGetPercentage(rs_ro_ratio, const_cast<float *>(_COCurve));
    }
    else if (gas_id == GAS_SMOKE){
        return MQGetPercentage(rs_ro_ratio, const_cast<float *>(_SmokeCurve));
    }
    return 0;
}

/*****************************  MQGetPercentage **********************************
Input:   rs_ro_ratio - Rs divided by Ro
         pcurve      - pointer to the curve of the target gas
Output:  ppm of the target gas
Remarks: By using the slope and a point of the line. The x(logarithmic value of ppm)
         of the line could be derived if y(rs_ro_ratio) is provided. As it is a
         logarithmic coordinate, power of 10 is used to convert the result to non-logarithmic
         value.
************************************************************************************/
int athome::sensor::MQ2GasSensor::MQGetPercentage(float rs_ro_ratio, float *pcurve) {
    return static_cast<int>(pow(10, ((log(rs_ro_ratio) - pcurve[1]) / pcurve[2]) + pcurve[0]));
}