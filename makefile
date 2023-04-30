
ifeq ($(OS),Windows_NT)
	config := "MinGW Makefiles"
	rm	   := del -fR
else
	config := "Unix Makefiles"
	rm	   := rm -rf
endif

.PHONY: setup build run docs clean

all: setup build docs
 
setup:
	cmake -S . -B build/ -G $(config) -Wno-dev

build: setup
	cd "./build" && $(MAKE) 

run: build
	cd "./bin" && "./ghostless-pacman"

clean:
	$(rm) "./build"
	$(rm) "./external"
	$(rm) "./bin"

docs:
	cd "./docs" && pdflatex -output-directory="../bin/docs" documentation.tex

