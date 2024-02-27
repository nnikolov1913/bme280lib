#include "IDrukSensor.h"

namespace druksensor {

class fakeSensor : public IDrukSensor {

    public:
        fakeSensor();
        virtual ~fakeSensor() override;

        virtual bool open() override;
        virtual void close() override;
        virtual bool getTemperature(double &t) override;
        virtual void setParameter(ParameterType paramtype, int param) override;

    private:
        bool mOpened;
        //Temparature given by this fake sensor will be between max and min
        int mMax;
        int mMin;
        //Last given temperature. Every time getTemperature() is called mTemperature
        //will be increased or decreased according to mInc
        int mTemperature;
        //true - increase, false - decreasse
        bool mInc;

};

}
