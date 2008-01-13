# GServer Makefile
#
CPPS = CAccount.cpp CBaddy.cpp CBuffer.cpp CDatabase.cpp CIni.cpp CLevel.cpp CLinkedList.cpp CList.cpp CMap.cpp CNpc.cpp COther.cpp CPacket.cpp CPlayer.cpp CServerBot.cpp CSocket.cpp CStringList.cpp CWeapon.cpp codec.cpp main.cpp
OBJS = CAccount.o CBaddy.o CBuffer.o CDatabase.o CIni.o CLevel.o CLinkedList.o CList.o CMap.o CNpc.o COther.o CPacket.o CPlayer.o CServerBot.o CSocket.o CStringList.o CWeapon.o codec.o main.o

LD = g++
SLFLAGS = -lsqlite3 -lz

all: $(CPPS) gserver

%.o: %.cpp
	$(LD) -c $<

gserver: $(OBJS)
	$(LD) -o ../bin/GServer217 $(OBJS) $(CPPOFDB) $(SLFLAGS)

help:
	@echo "GServer Makefile"
	@echo
	@echo "make"
	@echo "  Compiles Gserver for 217"
	@echo
	@echo "make clean_o"
	@echo "  Only clean objects"
	@echo
	@echo "make clean"
	@echo "  Clean everything up"

clean_o:
	rm -rf *.o

clean:
	rm -rf *.o ../bin/Gserver217