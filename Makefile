CC=g++
LIBS=-liniparser -ljson-c -lasound -pthread
CFLAGS=-std=c++11 -c -Wall -O3 -fomit-frame-pointer

all: main.o file_to.o set_asound.o set_battery.o set_hwmon.o set_icon.o set_nvidia.o set_pause.o set_time.o set_wlan.o get_input.o
	$(CC) $(LIBS) main.o file_to.o set_asound.o set_battery.o set_hwmon.o set_icon.o set_nvidia.o set_pause.o set_time.o set_wlan.o get_input.o -o i3barout

main.o:
	$(CC) $(CFLAGS) main.cpp

file_to.o:
	$(CC) $(CFLAGS) file_to.cpp

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

get_input.o:
	$(CC) $(CFLAGS) get_input.cpp

clean:
	rm *.o i3barout
