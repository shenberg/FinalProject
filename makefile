all: 
	cd SP; make all
	$(MAKE) SPCalculator
SPCalculator: SPHash.o SPList.o SPListElement.o #Extends dependency list
	#Put your commands here

SPList.o: SPList.c SPList.h SPListElement.h
	gcc -std=c99 -Wall -Werror -pedantic-errors -c SPList.c

SPListElement.o: SPListElement.c SPListElement.h
	gcc -std=c99 -Wall -Werror -pedantic-errors -c SPListElement.c 
	
SPHash.o: SPHash.c SPHash.h SPList.h SPListElement.h
	gcc -std=c99 -Wall -Werror -pedantic-errors -c SPHash.c

clean:
	cd SP; make clean
	rm -fr SPHash.o SPList.o SPListElement.o #Extend to remove other object files you created.
	