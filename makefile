CXX ?= g++
servertest:
	g++ -o servertest main.cpp Httpcon.h  printerror.h Response.h Request.h Threadpool.h lock.h -lpthread
debug:
	g++ -ggdb -o servertest main.cpp Httpcon.h  printerror.h Response.h Threadpool.h lock.h -lpthread
clear:
	rm -f servertest 