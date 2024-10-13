TARGET = isa-top

SRCS = MainClass.cpp ArgParser.cpp PacketCapturing.cpp PacketParser.cpp Output.cpp

LIBS = -lpcap -lncurses

make: 
	g++ -g -Wall -Wextra $(SRCS) -o $(TARGET) $(LIBS)

clean:
	rm -f $(TARGET)

run:
	sudo ./$(TARGET) $(ARGS)