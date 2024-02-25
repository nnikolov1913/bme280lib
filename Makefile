
TARGETBIN=readsensors
TARGETSO=libnicksensors.so

MYCFLAGS:=-Wall -O

SRCSOFILES=bme280.c linux_userspace.c
SRCSOCPPFILES=i2cBME280.cpp fake_sensor.cpp nicksenors.cpp
SRCFILES=main.cpp

SOOBJS=$(SRCSOFILES:.c=.o)
OBJS=$(SRCFILES:.cpp=.o)
SRCSOCPPOBJS=$(SRCSOCPPFILES:.cpp=.o)
HEADILES=bme280_defs.h bme280.h i2cBME280.h IDrukSensor.h linux_userspace.h nicksenors.h fake_sensor.h

LDFLAGS=-li2c -pthread -L.
CPP=g++
CC=gcc

all: $(TARGETSO) $(TARGETBIN)

%.o: %.cpp $(HEADILES)
	$(CPP) $(MYCFLAGS) -fPIC -o $@ -c $<

%.o: %.c $(HEADILES)
	$(CC) $(MYCFLAGS) -fPIC -o $@ -c $<

$(TARGETSO): $(SOOBJS) $(SRCSOCPPOBJS) $(HEADILES)
	$(CPP) -o $(TARGETSO) -shared $(SOOBJS) $(SRCSOCPPOBJS)

$(TARGETBIN): $(OBJS) $(HEADILES)
	$(CPP) -o $(TARGETBIN) $(OBJS) -lnicksensors  $(LDFLAGS)

clean:
	rm -f *.o $(TARGETBIN) $(TARGETSO)

