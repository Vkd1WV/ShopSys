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
//				throughout
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

// LOCAL FUNCTIONS
void print_product_list(DS);
void add_product(DS);
void delete_product(DS);
void edit_product(DS);
void print_transaction_list(DS);


void owner_menu(DS prod_list, DS xaction_list){
	int menu_option=0;
	
	do {
		puts("\t            OWNER MENU");
		puts("\t            ==========");
		puts("\t1. List Products for Sale");
		puts("\t2. Add a New Product");
		puts("\t3. Delete a Product");
		puts("\t4. Edit an Existing Product");
		puts("\t5. View Outstanding Transactions");
		puts("\t6. Delete All Outstanding Transactions");
		puts("\t7. Exit to Main Menu");
		
		menu_option=prompt();
		
		switch (menu_option){
		case 1: // LIST PRODUCTS FOR SALE
			print_product_list(prod_list);
			break;
		case 2: // ADD A NEW PRODUCT
			add_product(prod_list);
			break;
		case 3: // DELETE A PRODUCT
			delete_product(prod_list);
			break;
		case 4: // EDIT AN EXISTING PRODUCT
			printf("Enter ID of the Product to edit");
			
			//owner_sub_menu();
			break;
		case 5: // View Outstanding Transactions
			break;
		case 6: // Delete All Outstanding Transactions
			printf("Are you sure you want to Delete all outstanding transactions?\nThis action cannot be undone (y/n):");
			prompt();
		}
	} while (menu_option != 7);
}

bool owner_login(){
	printf("username:");
	grabword(stdin);
	printf("password:");
	grabword(stdin);
	return true;
}

void print_product_list(DS prod_list){
	Prod product;
	
	(void) pview(prod_list, 0); // set the view pointer to the head
	
	print_prod_heading(stdout);
	
	while ((product=view_next(prod_list)) != NULL){
		print_product(stdout, product);
	}
}


void add_product(DS prod_list){
	Prod new_product;
	
	new_product=malloc(sizeof(struct Product));
	if (new_product == NULL) {
		puts("malloc(): returned NULL");
		return;
	}
	
	printf("\nEnter new Product ID:");
	new_product->ID=grabword(stdin);
	
	// make sure the product ID is unique
	if (sort(prod_list, new_product, new_product->ID)) {
		puts("That Product ID is already in Use:");
		print_prod_heading(stdout);
		print_product(
			stdout,
			iview(prod_list, new_product->ID)
		);
		puts("");
		
		free(new_product);
		return;
	}
	
	
	printf("Enter new Product Name:");
	new_product->name=grabword(stdin);
	printf("Enter new Product price:");
	scanf("%f", &(new_product->price));
	printf("Enter new Product Quantity on hand:");
	scanf("%d", &(new_product->num_unit));
}

void delete_product(DS prod_list){
	return;
}

void edit_product(DS prod_list){
	int menu_option=0;
	
	do {
		puts("\t          EDIT");
		puts("\t          ====");
		puts("\t1. Edit the Quantity on Hand");
		puts("\t2. Change the Price");
		puts("\t3. Back");
		
		menu_option=prompt();
		
		switch (menu_option){
		case 1:
		case 2:
			puts("Not Implemented");
		}
	} while (menu_option != 3);
}


