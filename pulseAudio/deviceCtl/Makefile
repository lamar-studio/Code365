target=test
$(target): test.cc audiomanager.cc audiomanager.h audiocore.cc audiocore.h \
	sink.cc sink.h sinkinput.cc sinkinput.h source.cc source.h \
	sourceoutput.cc sourceoutput.h device.cc device.h 
	g++ -g test.cc audiomanager.cc audiocore.cc sink.cc sinkinput.cc source.cc sourceoutput.cc device.cc -o $(target) -Wall -lpthread -lpulse -std=c++11 
run:$(target)
	./test
clean:
	rm -rf *.out *.o test
