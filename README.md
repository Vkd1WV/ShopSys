# ShopSys

################################################################################
# Ammon Dodson
#	CES202
#	Winter 2016
#
#	This program should be build with make. A makefile has been provided.
#
################################ COMPONENTS ####################################
#
#	data.c data.h		stand-alone data structure library
#	input.c input.h		stand-alone input library
#
#	Makefile	contains build instructions
#
#	ShopSys.c	: main()	including main menu
#				; prompt()	prompt user for menu inputs
#	global.h	contains type definitions, prototypes, and includes used
#				throughout
#
#	file_access.c	This includes functions for reading and writing to files
#					: read_product_file()
#					: read_product()		called for each line of product.txt
#					: write_product_file()
#					: append_transaction_file()
#
#	owner.c		Contains the owner's menu and functions
#				; owner_login()
#				; owner_menu()
#				; print_product_list()
#				; print_product()
#				; add_product()
#				; delete_product()
#				; edit_product()
#				; print_transaction_list()
#
################################################################################
