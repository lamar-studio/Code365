target=test.out
$(target): HWConfig.cpp HWConfig.h SoundTest.cpp SoundTest.h config_test.cpp
	g++ -g -lasound -lpthread HWConfig.cpp SoundTest.cpp config_test.cpp -o $(target)
run:$(target)
	./test.out
clean:
	rm -rf *.out *.o
