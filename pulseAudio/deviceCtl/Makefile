target=test
$(target): test.cpp audiomanager.cpp audiomanager.h audiocore.cpp audiocore.h \
	sink.cpp sink.h sinkinput.cpp sinkinput.h source.cpp source.h \
	sourceoutput.cpp sourceoutput.h device.cpp device.h 
	g++ -g test.cpp audiomanager.cpp audiocore.cpp sink.cpp sinkinput.cpp source.cpp sourceoutput.cpp device.cpp -o $(target) -Wall -lpthread -lpulse -std=c++11 
run:$(target)
	./test
clean:
	rm -rf *.out *.o test
