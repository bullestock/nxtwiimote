
OBJECTS = adc_8591.o bluetooth.o brick.o color.o compass.o DistNx.o error.o filesystem.o i2c.o io_8574.o motor.o sensor.o sonar.o tilt.o usbnxt.o

HEADERS = ../include/adc_8591.h ../include/bluetooth.h ../include/brick.h ../include/color.h ../include/compass.h ../include/DistNx.h ../include/error.h ../include/filesystem.h ../include/i2c.h ../include/io_8574.h ../include/motor.h ../include/sensor.h ../include/sonar.h ../include/tilt.h ../include/usbnxt.h

CPPFLAGS := -I../include -Wall

libnxtbt.a:	$(OBJECTS)
	$(AR) rcs libnxtbt.a $(OBJECTS)

$(OBJECTS):	$(HEADERS)

clean:
	-rm -f $(OBJECTS) libnxtbt.a