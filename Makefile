CC=g++ -Wall -g -O2
CFLAGS=
LDFLAGS=-lpthread -lgsl -lgslcblas -lm

SERVER=myheap.o mytimer.o dataserver.o faketran.o globalfunction.o servermain.o myserver.o \
	   dbuffer.o dbufferlru.o dbufferdw.o dbufferlfru.o dbufferlfu.o dbufferlrfu.o dbufferfifo.o dbufferpr.o \
	   dbufferdws.o dbufferlfus.o dbufferlrus.o
CLIENT=myheap.o mytimer.o dbuffer.o faketran.o globalfunction.o modelassemble.o myclient.o myclientmanage.o clientmain.o \
	   dbufferlru.o dbufferdw.o dbufferlfru.o dbufferlfu.o dbufferlrfu.o dbufferfifo.o dbufferpr.o
OPT=optMain.o globalfunction.o
LISTGENERATOR=tools/src/globalfunction.o tools/src/modelassemble.o tools/src/myclient.o tools/src/myclientmanage.o tools/src/clientmain.o

all:server_exe client_exe listgenerator optResult

.PHONY:all

server_exe:$(SERVER)
	$(CC) $^ -o $@ $(CFLAGS) $(LDFLAGS)

client_exe:$(CLIENT)
	$(CC) $^ -o $@ $(CFLAGAS) $(LDFLAGS)
optResult:$(OPT)
	$(CC) $^ -o $@ $(CFLAGAS) $(LDFLAGS)

listgenerator:$(LISTGENERATOR)
	$(CC) $^ -o $@ $(CFLAGAS) $(LDFLAGS)

%.o:src/%.cpp
	$(CC) -c $^

clean:
	rm -rf *.o
	rm -rf client_exe server_exe listgenerator
