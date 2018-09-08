target=test.out
$(target): HWConfig.cpp HWConfig.h SoundTest.cpp SoundTest.h config_test.cpp
	g++ -g HWConfig.cpp SoundTest.cpp config_test.cpp -o $(target) -lasound -lpthread
run:$(target)
	./test.out
clean:
	rm -rf *.out *.o
