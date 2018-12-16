target=test.out
$(target): HWConfig.cpp HWConfig.h SoundTest.cpp SoundTest.h test.cpp
	g++ -g HWConfig.cpp SoundTest.cpp test.cpp -o $(target) -lpulse-simple -lpulse -lpthread
run:$(target)
	./test.out
clean:
	rm -rf *.out *.o
