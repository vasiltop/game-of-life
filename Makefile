$(shell mkdir -p bin)

C := cc
SRC := src/*.c
TARGET := bin/game-of-life
BINDIR := /usr/local/bin

all: $(TARGET)

$(TARGET): $(SRC)
	$(C) $(SRC) -o $(TARGET) -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

run: $(TARGET)
	./$(TARGET)

install: $(TARGET)
	install -d $(BINDIR)
	install -m 755 $(TARGET) $(BINDIR)

clean:
	rm bin/*
