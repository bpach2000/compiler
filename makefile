compile: driver.c parser.c scanner.c symtbl.c ast.c ast-print.c gen-code.c
	gcc -Wall -Werror -std=c11 -o compile driver.c parser.c scanner.c symtbl.c ast-print.c ast.c gen-code.c

clean:
	rm -f compile
  
