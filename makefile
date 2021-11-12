CLASS_PATH = .

all: main

main: main.c 
	gcc -std=c99 main.c -o main

clean:
	rm main

run:
	./main