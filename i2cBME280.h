#include "IDrukSensor.h"

namespace druksensor {

class i2cBME280 : public IDrukSensor {

    public:
        i2cBME280();
        virtual ~i2cBME280() override;

        virtual bool open() override;
        virtual void close() override;
        virtual bool getTemperature(double &t) override;

    private:
        bool mOpened;
};

}
