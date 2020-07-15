SHELL := /bin/bash

all: tradeclient executorgo

tradeclient: src/C++/config.h src/C++/getopt-repl.h src/C++/Application.h src/C++/Application.cpp src/C++/tradeclient.cpp src/C++/XMLSettings.h
	g++ -o tradeclient src/C++/XMLSettings.cpp src/C++/Application.cpp src/C++/tradeclient.cpp -Isrc -I/opt/quickfix/include -L/opt/quickfix/lib -lquickfix -lssl -lcrypto -lpthread -Wno-deprecated

executorgo: src/executorgo/main.go 
	go build -v -o ./executorgo ./src/executorgo/main.go

clean:
	rm -f tradeclient
	rm -f executorgo

test: lint vet executorgo 

lint:
	golint ./...

vet:
	go vet ./...

# Commands for docker images.
# ----------------------------
build-linux:
	GOOS=linux GOARCH=amd64 go build -v -o ./executorgo ./src/executorgo

build-docker: clean build-linux
	docker build -t quickfixgo/executorgo:latest -f ./src/executorgo/Dockerfile .
