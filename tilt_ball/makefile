CC=g++
CFLAGS=-c -Wall
LDFLAGS= -lmraa
SOURCES=spi/spi_port_edison.cpp spi/spi_device_edison.cpp SFE_LSM9DS0.cpp oled/Edison_OLED.cpp gpio/gpio_edison.cpp tilt_ball.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=tilt_ball

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o $(EXECUTABLE)
