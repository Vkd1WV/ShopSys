/******************************************************************************/
//	Author:	Ammon Dodson
//			Daryan Hanshew
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
//	ShopSys.c	contains the main menu, and the menu prompt
//		: int main()
//		; int prompt()
//
//	global.h	contains type definitions, prototypes, and includes used
//				throughout
//
//	file_access.c	This includes functions for reading and writing to files
//		: Prod	read_product			(FILE* product_file_discriptor			);
//		: DS	read_product_file		(const char* file_name					);
//		: int	write_product_file		(const char* file_name, DS product_data	);
//		: int	append_transaction_file	(const char* file_name, DS xaction_data	);
//
//	formatting.c	Prints out data to the screen or files
//		: void print_prod_heading		(FILE* file_discriptor			);
//		: void print_product			(FILE* file_discriptor, Prod p	);
//		: void print_xaction_heading	(FILE* file_discriptor			);
//		: void print_xaction			(FILE* file_discriptor, Trans t	);
//
//	owner.c		Contains the owner's menu and functions
//		: bool owner_login				();
//		: void owner_menu				(DS product_data, DS transaction_data);
//		: void print_product_list		(DS product_data		);
//		: void add_product				(DS product_data		);
//		: void delete_product			(DS product_data		);
//		: void edit_product				(DS product_data		);
//		: void print_transaction_list	(DS transaction_data	);
//		: void clear_xactions			(DS transaction_data	);
//
/******************************************************************************/

#include "global.h"

/************************************************************/
/*						The Menu Prompt						*/
/************************************************************/
//USES:	input.h:	grabword()

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

/************************************************************/
/*							MAIN							*/
/************************************************************/
//USES:	file_access.c:	read_product_file()
//						write_product_file()
//						append_transaction_file()
//		data.h:			new_DS()
//		owner.c:		owner_login()
//						owner_menu()
//		customer.c:		customer_menu()

int main (int argc, const char **argv){
	const char* product_file="product.txt";
	const char* transaction_file="transaction.txt";
	int menu_option=0;
	DS product_list, transaction_list;
	// DS is a generic data structure root provided by data.o
	
	
	// Use different product files
	if (argc>1){
		product_file=argv[1];
	}else{
		puts("Using default files.");
	}
	
	// Read Product Data from File
	product_list=read_product_file(product_file);
	if(product_list == NULL)
		return EXIT_FAILURE;
	
	// Create an empty transaction list
	transaction_list=new_DS('l');
	
	// The Main Menu
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
	
	// Saving Data on Exit
	puts("Exiting...");
	
	write_product_file(product_file, product_list);
	append_transaction_file(transaction_file, transaction_list);
	//FIXME: do something with these return values
	
	return EXIT_SUCCESS;
}
