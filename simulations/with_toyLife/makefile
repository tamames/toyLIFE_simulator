main: main.o classes.o toylife/helper_functions.o toylife/toy_plugin.o functions/func.o functions/globals.o
	g++ main.o classes.o toylife/helper_functions.o toylife/toy_plugin.o functions/func.o functions/globals.o -o main

classes.o: classes.cpp
	g++ -c classes.cpp

toylife/helper_functions.o: toylife/helper_functions.cpp
	g++ -c toylife/helper_functions.cpp -o toylife/helper_functions.o

toylife/toy_plugin.o: toylife/toy_plugin.cpp
	g++ -c toylife/toy_plugin.cpp -o toylife/toy_plugin.o

functions/func.o: functions/func.cpp
	g++ -c functions/func.cpp -o functions/func.o

functions/globals.o: functions/globals.cpp
	g++ -c functions/globals.cpp -o functions/globals.o

clean:
	rm -f *.o
	rm -f toylife/*.o
	rm -f functions/*.o
	rm -f main






