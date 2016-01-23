all: 
	cd SP; make all
	$(MAKE) SPCalculator
SPCalculator: main.o Tree.o Funcs.o SPHash.o SPList.o SPListElement.o
	gcc -std=c99 -Wall -Werror -pedantic-errors main.o Tree.o Funcs.o SPHash.o SPList.o SPListElement.o -o SPCalculator -lm

SPList.o: SPList.c SPList.h SPListElement.h
	gcc -std=c99 -Wall -Werror -pedantic-errors -c SPList.c

SPListElement.o: SPListElement.c SPListElement.h
# no -Werror since school-provided SPListElement.c doesn't end with newline 
	gcc -std=c99 -Wall -pedantic-errors -c SPListElement.c 
	
SPHash.o: SPHash.c SPHash.h SPList.h SPListElement.h
	gcc -std=c99 -Wall -Werror -pedantic-errors -c SPHash.c

Funcs.o: Funcs.c API.h Funcs.h Tree.h SPHash.h
	gcc -std=c99 -Wall -Werror -pedantic-errors -c Funcs.c

Tree.o: Tree.c API.h Funcs.h Tree.h SPHash.h
	gcc -std=c99 -Wall -Werror -pedantic-errors -c Tree.c

main.o: main.c API.h Funcs.h Tree.h SPHash.h
	gcc -std=c99 -Wall -Werror -pedantic-errors -c main.c

clean:
	cd SP; make clean
	rm -fr *.o
	rm -f SPCalculator
