#
# Shop Sys : a shopping system
#


cleanfiles=*.out *.o ShopSys
options=-Wall -Wextra -pedantic -std=c11 -g

all: ShopSys

data.o: data.c data.h
	gcc $(options) -c data.c
input.o: input.c input.h
	gcc $(options) -c input.c

services.o: services.c global.h
	gcc $(options) -c services.c

owner.o: owner.c global.h
	gcc $(options) -c owner.c
customer.o: customer.c global.h
	gcc $(options) -c customer.c

ShopSys.o: ShopSys.c global.h
	gcc $(options) -c ShopSys.c

ShopSys: data.o input.o services.o owner.o customer.o ShopSys.o
	gcc $(options) -o ShopSys \
			data.o input.o services.o owner.o customer.o ShopSys.o

clean:
	rm -f $(cleanfiles)
