COMPUTACAO_GRAFICA_ROOT := $(PWD)/exercicio-pratico-03
CC := g++
CFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -O2 -lglfw -lGLEW -lX11 -lGLU -lGL

all := main

main:
	$(CC) $(CFLAGS) $(COMPUTACAO_GRAFICA_ROOT)/exercicio-pratico-03.cpp -o $(COMPUTACAO_GRAFICA_ROOT)/exercicio-pratico-03.bin

run:
	$(COMPUTACAO_GRAFICA_ROOT)/exercicio-pratico-03.bin

clean:
	rm $(COMPUTACAO_GRAFICA_ROOT)/exercicio-pratico-03.bin
