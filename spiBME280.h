#include <string>
#include "IDrukSensor.h"

namespace druksensor {

class spiBME280 : public IDrukSensor {

    public:
        spiBME280();
        virtual ~spiBME280() override;

        virtual bool open() override;
        virtual void close() override;
        virtual bool getTemperature(double &t) override;
        virtual void setParameter(ParameterType paramtype, int param) override;

    private:
        bool mOpened;
        std::string mInterface;
        void *mCtx;
};

}
