# Description: File to compile, run or remove the project
# Author: Lukáš Píšek (xpisek02)
# File: Makefile

TARGET = isa-top

SRCS = MainClass.cpp ArgParser.cpp PacketCapturing.cpp PacketParser.cpp Output.cpp

LIBS = -lpcap -lncurses

.PHONY: clean run

make: 
	g++ -g -Wall -Wextra $(SRCS) -o $(TARGET) $(LIBS)

clean:
	rm -f $(TARGET)

run: make
	sudo ./$(TARGET) $(ARGS)