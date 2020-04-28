TAR = spider

all:
	g++ -g -std=c++11 spider.cpp http.cpp http.h -o $(TAR)

.PHONY:
clear:
	rm $(TAR)
