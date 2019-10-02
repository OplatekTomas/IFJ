CFLAGS=

make: ./src/main.c
	cc $^ -o ./target/IFJC $(CFLAGS)

run:
	./target/IFJC

clear:
	rm -rf ./target/*
# ¯\_(ツ)_/¯ heyo snaha byla
#SOURCES := $(wildcard $(SRC)/*.c)
#make: target/%.o
#	cc $(CFLAGS) -o
#target/%.o: src/%.c
#	cc $(CFLAGS) -c $< -o $@