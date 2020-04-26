TAR = spider

all:
	g++ spider.cpp http.cpp http.h -o $(TAR)

.PHONY:
clear:
	rm $(TAR)
