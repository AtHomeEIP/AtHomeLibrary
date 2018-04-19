//
// Created by Alexis Lina on 19/04/2018.
//

#include <unistd.h>
#include <cmath>
#include "MQ2GasSensor.hpp"


athome::sensor::MQ2GasSensor::MQ2GasSensor(int pin) : _pin(pin),
                                                      _sampleValue(false)
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

float athome::sensor::MQ2GasSensor::MQResistanceCalculation(int raw_adc) {
    return ((static_cast<float>RL_value*(1023-raw_adc)/raw_adc));
}

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

float athome::sensor::MQ2GasSensor::MQRead(int mq_pin) {
    float   rs = 0;

    for (int j = 0; j < READ_SAMPLE_TIMES; ++j) {
        rs += MQResistanceCalculation(digitalRead(_pin));
        usleep(READ_SAMPLE_INTERVAL);
    }
    rs = rs / READ_SAMPLE_TIMES;
    return rs;
}

int athome::sensor::MQ2GasSensor::MQGetGasPercentage(float rs_ro_ratio, int gas_id) {
    if (gas_id == GAS_LPG){
        return MQGetPercentage(rs_ro_ratio, _LPGCurve);
    }
    else if (gas_id == GAS_CO){
        return MQGetPercentage(rs_ro_ratio, _COCurve);
    }
    else if (gas_id == GAS_SMOKE){
        return MQGetPercentage(rs_ro_ratio, _SmokeCurve);
    }
    return 0;
}

int athome::sensor::MQ2GasSensor::MQGetPercentage(float rs_ro_ratio, float *pcurve) {
    return static_cast<int>(std::pow(10, ((std::log(rs_ro_ratio) - pcurve[1]) / pcurve[2]) + pcurve[0]));
}


