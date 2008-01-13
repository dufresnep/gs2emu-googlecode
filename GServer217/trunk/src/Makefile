# GServer Makefile
#
CPPS = CAccount.cpp CBaddy.cpp CBuffer.cpp CDatabase.cpp CIni.cpp CLevel.cpp CLinkedList.cpp CList.cpp CListServer.cpp CMap.cpp CNpc.cpp COther.cpp CPacket.cpp CPlayer.cpp CServerBot.cpp CSocket.cpp CStringList.cpp CWeapon.cpp CWordFilter.cpp codec.cpp main.cpp md5.cpp
OBJS = CAccount.o CBaddy.o CBuffer.o CDatabase.o CIni.o CLevel.o CLinkedList.o CList.o CListServer.o CMap.o CNpc.o COther.o CPacket.o CPlayer.o CServerBot.o CSocket.o CStringList.o CWeapon.o CWordFilter.cpp codec.o main.o md5.o

LD = g++
SLFLAGS = -lsqlite3 -lz -g

all: $(CPPS) gserver

%.o: %.cpp
	$(LD) -c $<

gserver: $(OBJS)
	$(LD) -o ../bin/GServer217 $(OBJS) $(CPPOFDB) $(SLFLAGS)

help:
	@echo "GServer Makefile"
	@echo
	@echo "make"
	@echo "  Compiles GServer for 217"
	@echo
	@echo "make clean_o"
	@echo "  Only clean objects"
	@echo
	@echo "make clean"
	@echo "  Clean everything up"

clean_o:
	rm -rf *.o

clean:
	rm -rf *.o ../bin/GServer217
