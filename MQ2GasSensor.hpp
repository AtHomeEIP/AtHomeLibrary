//
// Created by Alexis Lina on 19/04/2018.
//

#ifndef FRAMEWORK_MQ2GASSENSOR_HPP
#define FRAMEWORK_MQ2GASSENSOR_HPP

#include "ISensor.hpp"


#define RL_value (5)            //define the load resistance on the board, in kilo ohms
#define R0_CLEAN_AIR_FACTOR (9.83) //RO_CLEAR_AIR_FACTOR=(Sensor resistance in clean air)/RO

/***********************Software Related Macros************************************/
#define         CALIBARAION_SAMPLE_TIMES     (50)    //define how many samples you are going to take in the calibration phase
#define         CALIBRATION_SAMPLE_INTERVAL  (500)   //define the time interal(in milisecond) between each samples in the
//cablibration phase
#define         READ_SAMPLE_INTERVAL         (50)    //define how many samples you are going to take in normal operation
#define         READ_SAMPLE_TIMES            (5)     //define the time interal(in milisecond) between each samples in
//normal operation

/**********************Application Related Macros**********************************/
#define         GAS_LPG                      (0)
#define         GAS_CO                       (1)
#define         GAS_SMOKE                    (2)

#ifdef ARDUINO

namespace athome{
    namespace sensor {
        class MQ2GasSensor : public ISensor{
        public:
            explicit MQ2GasSensor(int pin);
            MQ2GasSensor(const MQ2GasSensor &) = delete;
            MQ2GasSensor &operator=(const MQ2GasSensor &) = delete;
            ~MQ2GasSensor();
            int   MQGetPercentage(float rs_ro_ratio, float *pcurve);
            void        setpin(int pin);
            int         getpin() const;
            int         getLPG() const;
            int         getSMOKE() const;
            int         getCO() const;
            void        *getValue();

            ISensorScale getEstimate();

        private:
            int _pin;
            int array_value[3];
            bool _sampleValue;
            const float _LPGCurve[3];
            const float _COCurve[3];
            const float _SmokeCurve[3];
            const float _R0;
            float MQResistanceCalculation(int raw_adc);
            float MQCalibration(int mqpin);
            float MQRead(int mq_pin);
            int   MQGetGasPercentage(float rs_ro_ratio, int gas_id);
        };
    }
}

#endif /* ARDUINO */
#endif //FRAMEWORK_MQ2GASSENSOR_HPP
