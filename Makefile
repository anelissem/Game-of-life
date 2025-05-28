# to run other inputs modify data16.in with any other input file

DIR = output
build:
	gcc master.c stivelib.c -o program
execute:
	./program data16.in output1.out
clean:
	rm -f program
clean_files:
	rm -rf $(DIR)/*

