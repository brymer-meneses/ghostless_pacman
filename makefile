ifeq ($(OS),Windows_NT)
	config := "MinGW Makefiles"
	rm	   := del -fR
else
	config := "Unix Makefiles"
	rm	   := rm -rf
endif

.PHONY: setup build run docs clean

all: setup build run docs
 
setup:
	cmake -S . -B build/ -G $(config) -Wno-dev

build:
	cd "./build" && $(MAKE) 

run: setup build
	cd "./bin" && "./ghostless-pacman"

clean:
	$(rm) "./build"
	$(rm) "./external"
	$(rm) "./bin"

docs:
	pandoc "./readme.md" -o "./readme.pdf"




	

