# Joe O'Regan
# K00203642
# Build JOR_Net Static and Shared Libraries

all: program1 program2 program3 program4 program5 program6 program7 program8

# Static Lib

program1: JOR_Net.c
	gcc -c JOR_Net.c -o bin/static/JOR_Net.o

program2: JOR_NetClient.c
	gcc -c JOR_NetClient.c -o bin/static/JOR_NetClient.o

program3: JOR_NetServer.c
	gcc -c JOR_NetServer.c -o bin/static/JOR_NetServer.o

program4: bin/static/JOR_Net.o bin/static/JOR_NetClient.o bin/static/JOR_NetServer.o
	ar rcs bin/static/libJOR_Net.a bin/static/JOR_Net.o bin/static/JOR_NetClient.o bin/static/JOR_NetServer.o

# Shared LIb

program5: JOR_Net.c
	gcc -c -fPIC JOR_Net.c -o bin/shared/JOR_Net.o

program6: JOR_NetClient.c
	gcc -c -fPIC JOR_NetClient.c -o bin/shared/JOR_NetClient.o

program7: JOR_NetServer.c
	gcc -c -fPIC JOR_NetServer.c -o bin/shared/JOR_NetServer.o

program8: bin/shared/JOR_Net.o bin/shared/JOR_NetClient.o bin/shared/JOR_NetServer.o
	gcc -shared bin/shared/JOR_Net.o bin/shared/JOR_NetClient.o bin/shared/JOR_NetServer.o -o bin/shared/libJOR_Net.so
