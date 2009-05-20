
OBJECTS = adc_8591.o bluetooth.o brick.o color.o compass.o DistNx.o error.o filesystem.o i2c.o io_8574.o motor.o sensor.o sonar.o tilt.o usbnxt.o

CPPFLAGS := -I../include -Wall

libnxtbt.a:	$(OBJECTS)
	$(AR) rcs libnxtbt.a $(OBJECTS)

clean:
	-rm -f $(OBJECTS) libnxtbt.a