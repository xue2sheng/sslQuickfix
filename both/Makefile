SHELL := /bin/bash

all: bothcpp bothgo

bothcpp: src/C++/config.h src/C++/getopt-repl.h src/C++/Application.h src/C++/Application.cpp src/C++/bothcpp.cpp src/C++/XMLSettings.h src/C++/XMLSettings.cpp
	g++ -o bothcpp src/C++/XMLSettings.cpp src/C++/Application.cpp src/C++/bothcpp.cpp -Isrc -I/opt/quickfix/include -L/opt/quickfix/lib -lquickfix -lssl -lcrypto -lpthread -Wno-deprecated

bothgo: src/bothgo/main.go src/bothgo/xmlsettings.go
	go build -v -o ./bothgo ./src/bothgo/main.go ./src/bothgo/xmlsettings.go

clean:
	rm -f bothcpp
	rm -f bothgo

test: lint vet bothgo

lint:
	golint ./...

vet:
	go vet ./...

# Commands for docker images.
# ----------------------------
build-linux:
	GOOS=linux GOARCH=amd64 go build -v -o ./bothgo ./src/bothgo

build-docker: clean build-linux
	docker build -t quickfixgo/bothgo:latest -f ./src/bothgo/Dockerfile .
