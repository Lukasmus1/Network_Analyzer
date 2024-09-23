TARGET = isa-top

SRCS = MainClass.cpp ArgParser.cpp PacketCapturing.cpp

make: 
	g++ -g $(SRCS) -o $(TARGET) -lpcap

clean:
	rm -f $(TARGET)

run:
	sudo ./$(TARGET) $(ARGS)