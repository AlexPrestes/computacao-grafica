COMPUTACAO_GRAFICA_ROOT := $(PWD)/trabalho-pratico-01
CC := g++
CFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -O2 -lglfw -lGLEW -lX11 -lGLU -lGL

all := main

main:
	$(CC) $(CFLAGS) $(COMPUTACAO_GRAFICA_ROOT)/trabalho-pratico-01.cpp -o $(COMPUTACAO_GRAFICA_ROOT)/trabalho-pratico-01.bin

run:
	$(COMPUTACAO_GRAFICA_ROOT)/trabalho-pratico-01.bin

clean:
	rm $(COMPUTACAO_GRAFICA_ROOT)/trabalho-pratico-01.bin
