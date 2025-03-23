$(shell mkdir -p bin)

C := cc
SRC := src/*.c
TARGET := bin/out

run:
	$(C) $(SRC) -o $(TARGET) -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
	./$(TARGET)

clean:
	rm bin/*
