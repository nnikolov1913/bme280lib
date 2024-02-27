#include <string>
#include "IDrukSensor.h"

namespace druksensor {

class i2cBME280 : public IDrukSensor {

    public:
        i2cBME280();
        virtual ~i2cBME280() override;

        virtual bool open() override;
        virtual void close() override;
        virtual bool getTemperature(double &t) override;
        virtual void setParameter(ParameterType paramtype, int param) override;

    private:
        bool mOpened;
        std::string mInterface;
        int mAddress;
        void *mCtx;
};

}
