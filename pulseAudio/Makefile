target=test.out
$(target): audiomanager.cc audiomanager.h audiocore.cc audiocore.h card.cc card.h \
	sink.cc sink.h sinkinput.cc sinkinput.h source.cc source.h \
	sourceoutput.cc sourceoutput.h device.cc device.h 
	g++ -g audiomanager.cc audiocore.cc card.cc sink.cc sinkinput.cc source.cc sourceoutput.cc device.cc -o $(target) -lasound -lpthread -lpulse -std=c++11
run:$(target)
	./test.out
clean:
	rm -rf *.out *.o
