CC= g++
LIBS= /opt/picoscope/lib/ -lwiringPi -lusbdrdaq -lpthread
INCLUDES =  /opt/picoscope/include/

all:
	sudo $(CC) *.cpp -o build/Regelung -L $(LIBS) -I $(INCLUDES) -Wall -Wextra -pedantic 
