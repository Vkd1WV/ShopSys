/******************************************************************************/
// Ammon Dodson
//	CES202
//	Winter 2016
//
//	This program should be build with make. A makefile has been provided.
//
/******************************* COMPONENTS ***********************************/
//
//	data.c data.h		stand-alone data structure library
//	input.c input.h		stand-alone input library
//
//	Makefile	contains build instructions
//
//	ShopSys.c	: main()	including main menu
//				; prompt()	prompt user for menu inputs
//	global.h	contains type definitions, prototypes, and includes used
//				throughout the program
//
//	file_access.c	This includes functions for reading and writing to files
//					: read_product_file()
//					: read_product()		called for each line of product.txt
//					: write_product_file()
//					: append_transaction_file()
//
//	owner.c		Contains the owner's menu and functions
//				; owner_login()
//				; owner_menu()
//				; print_product_list()
//				; print_product()
//				; add_product()
//				; delete_product()
//				; edit_product()
//				; print_transaction_list()
//
/******************************************************************************/

#include "global.h"

/**	Prompt the user for simple input
 */
int prompt(){
	char* temp;
	int result;
	
	printf("ShopSys:>");
	temp=grabword(stdin);
	
	if (!(result=atoi(temp)))	// If the input was not a number
		result=*temp;			// we assume it's a letter
	free(temp);
	return result;
}

int main (int argc, const char **argv){
	const char* product_file="product.txt";
	const char* transaction_file="transaction.txt";
	int menu_option=0;
	DS product_list, transaction_list;
	// DS is a generic data structure root provided by data.o
	
	/************************************************************/
	/*					Use different data files				*/
	/************************************************************/
	
	if (argc>1){
		product_file=argv[1];
		transaction_file=argv[2];
	}else{
		puts("Using default files.");
	}
	
	/************************************************************/
	/*				Read Product Data from File					*/
	/************************************************************/
	product_list=read_product_file(product_file);
	if(product_list == NULL)
		return EXIT_FAILURE;
	
	// Create an empty transaction list
	transaction_list=new_DS('l');
	
	/************************************************************/
	/*						The Main Menu						*/
	/************************************************************/
	
	do {
		puts("\t         MAIN MENU");
		puts("\t===========================");
		puts("\t1. Owner Login");
		puts("\t2. Customer Login");
		puts("\t3. Exit, Saving All Changes");
		
		menu_option=prompt();
		
		switch (menu_option){
		case 1:
			if (owner_login())
				owner_menu(product_list, transaction_list);
			else puts("Tried to break in did ye?");
			break;
		case 2:
			customer_menu();
			break;
		}
		
	} while (menu_option != 3);
	
	/************************************************************/
	/*					Saving data on Exit						*/
	/************************************************************/
	
	puts("Exiting...");
	
	write_product_file(product_file, product_list);
	append_transaction_file(transaction_file, transaction_list);
	//FIXME: do something with these return values
	
	return EXIT_SUCCESS;
}
