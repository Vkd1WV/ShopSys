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

void sort_menu();

/************************************************************/
/*						The Customer Menu					*/
/************************************************************/
//USES: ShopSys.c:	prompt()

void customer_menu(){
	int menu_option=0;
	
	do {
		puts("");
		puts("\t        CUSTOMER MENU");
		puts("\t=================================");
		puts("\t1. Display Products");
		puts("\t2. Search for Product by Name");
		puts("\t3. Add Item to Cart");
		puts("\t4. Checkout");
		puts("\t5. Logout and Return to Main Menu");
		
		menu_option=prompt();
		
		switch (menu_option){
		case 1: // Display Products
			sort_menu();
			break;
		case 2: // Search for Product by Name
		case 3: // Add Item to Cart
		case 4: // Checkout
		puts("Not Implemented");
		}
	} while (menu_option != 5);
}

/************************************************************/
/*				Sort Products for the Customer				*/
/************************************************************/
//USES: ShopSys.c:	prompt()

void sort_menu(){
	int menu_option=0;
	
	do {
		puts("");
		puts("\t       SORT OPTIONS");
		puts("\t===========================");
		puts("\t1. Unsorted");
		puts("\t2. Sort by Quantity on Hand");
		puts("\t3. Sort by Price");
		puts("\t4. Back");
		
		menu_option=prompt();
		
		switch (menu_option){
		case 1: // Unsorted
		case 2: // Sort by Quantity on Hand
		case 3: // Sort by Price
		puts("Not Implemented");
		}
	} while (menu_option != 4);
}
