assembler: assembler.o read_files.o compile.o pre_processing.o parse.o translate.o validate.o write_files.o

assembler.o: assembler.c  read_files.h
	gcc -c -Wall -ansi -pedantic assembler.c -o assembler.o

read_files.o: read_files.c compile.h
	gcc -c -Wall -ansi -pedantic -g read_files.c -o read_files.o

compile.o: compile.c pre_processing.h parse.h validate.h translate.h write_files.h objects.h
	gcc -c -Wall -ansi -pedantic -g compile.c -o compile.o

pre_processing.o: pre_processing.c lang.h
	gcc -c -Wall -ansi -pedantic -g pre_processing.c -o pre_processing.o

parse.o: parse.c objects.h commands.h lang.h
	gcc -c -Wall -ansi -pedantic -g parse.c -o parse.o

validate.o: validate.c  objects.h
	gcc -c -Wall -ansi -pedantic -g validate.c -o validate.o

translate.o: translate.c objects.h  
	gcc -c -Wall -ansi -pedantic -g translate.c -o translate.o

write_files.o: write_files.c objects.h 
	gcc -c -Wall -ansi -pedantic -g write_files.c -o write_files.o