DIR = output
build:
	gcc master.c -o program
execute:
	./program data1.in output1.out
clean:
	rm -f program
clean_files:
	rm -rf $(DIR)/*

