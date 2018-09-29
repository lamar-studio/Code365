target=test.out
$(target): pavucontrol.cc pavucontrol.h mainwindow.cc mainwindow.h cardwidget.cc cardwidget.h \
	sinkwidget.cc sinkwidget.h sinkinputwidget.cc sinkinputwidget.h sourcewidget.cc sourcewidget.h \
	sourceoutputwidget.cc sourceoutputwidget.h devicewidget.cc devicewidget.h 
	g++ -g pavucontrol.cc mainwindow.cc cardwidget.cc sinkwidget.cc sinkinputwidget.cc sourcewidget.cc sourceoutputwidget.cc devicewidget.cc -o $(target) -lasound -lpthread -lpulse
run:$(target)
	./test.out
clean:
	rm -rf *.out *.o
