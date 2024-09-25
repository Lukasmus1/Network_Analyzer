TARGET = isa-top

SRCS = MainClass.cpp ArgParser.cpp PacketCapturing.cpp PacketParser.cpp

make: 
	g++ -g $(SRCS) -o $(TARGET) -lpcap

clean:
	rm -f $(TARGET)

run:
	sudo ./$(TARGET) $(ARGS)