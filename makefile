CC = gcc
CFLAGS = -g -Wall
TARGET = FD

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC)	$(CFLAGS)	-o	hw3	$(TARGET).c	-lglut	-lGLU -lGL -lm


clean:
	$(RM) LorenzAttractor
