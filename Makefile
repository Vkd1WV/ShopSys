#
# Shop Sys : a shopping system
#


cleanfiles=*.out *.o ShopSys
options=-Wall -Wextra -pedantic -std=c11 -O2

all: ShopSys

data.o: data.c data.h
	gcc $(options) -c data.c
input.o: input.c input.h
	gcc $(options) -c input.c

formatting.o: formatting.c global.h
	gcc $(options) -c formatting.c

customer.o: customer.c global.h
	gcc $(options) -c customer.c
owner.o: owner.c global.h
	gcc $(options) -c owner.c

file_access.o: file_access.c global.h
	gcc $(options) -c file_access.c

ShopSys.o: ShopSys.c global.h
	gcc $(options) -c ShopSys.c

ShopSys: data.o input.o customer.o owner.o file_access.o formatting.o ShopSys.o
	gcc $(options) -o ShopSys \
			data.o input.o customer.o owner.o file_access.o formatting.o ShopSys.o

clean:
	rm -f $(cleanfiles)
