CC=g++
LIBS=-liniparser -ljson-c -lasound
CFLAGS=-std=c++11 -c -Wall -O3 -fomit-frame-pointer

all: main.o set_asound.o set_battery.o set_hwmon.o set_icon.o set_nvidia.o set_pause.o set_time.o set_wlan.o
	$(CC) $(LIBS) main.o set_asound.o set_battery.o set_hwmon.o set_icon.o set_nvidia.o set_pause.o set_time.o set_wlan.o -o i3barout

main.o:
	$(CC) $(CFLAGS) main.cpp

set_asound.o:
	$(CC) $(CFLAGS) set_asound.cpp

set_battery.o:
	$(CC) $(CFLAGS) set_battery.cpp

set_hwmon.o:
	$(CC) $(CFLAGS) set_hwmon.cpp

set_icon.o:
	$(CC) $(CFLAGS) set_icon.cpp

set_nvidia.o:
	$(CC) $(CFLAGS) set_nvidia.cpp

set_pause.o:
	$(CC) $(CFLAGS) set_pause.cpp

set_time.o:
	$(CC) $(CFLAGS) set_time.cpp

set_wlan.o:
	$(CC) $(CFLAGS) set_wlan.cpp

clean:
	rm *.o i3barout
