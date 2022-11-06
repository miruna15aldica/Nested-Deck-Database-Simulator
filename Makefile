.phony: build clean

build:
	gcc  -Wall -Wextra -std=c99 -g tema1.c Parsare_info.h Card.c Card.h -o tema1 
	
clean:
	rm -f ./tema1

