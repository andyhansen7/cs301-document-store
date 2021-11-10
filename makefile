CLASS_PATH = .

all: main

main: main.c table_utils.c entry_utils.c field_utils.c
	gcc -std=c99 main.c -o main

clean:
	rm main

run:
	./main