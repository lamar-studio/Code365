target=volume
$(target): alsa.cpp alsa.h alsa_card.cpp alsa_card.h alsa_mixer.cpp alsa_mixer.h log.cpp log.h main.cpp 
	g++ -g alsa.cpp alsa_card.cpp alsa_mixer.cpp log.cpp main.cpp -o $(target) -lasound -lpthread
run:$(target)
	./volume
clean:
	rm -rf *.out *.o $(taget)
