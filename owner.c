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

#include <string.h>
#include "global.h"

// LOCAL FUNCTIONS
void print_product_list(DS);
void add_product(DS);
void delete_product(DS);
void edit_product(DS);
void print_transaction_list(DS);
void clear_xactions(DS);


void owner_menu(DS prod_list, DS xaction_list){
	int menu_option=0;
	
	do {
		puts("");
		puts("\t             OWNER MENU");
		puts("\t======================================");
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
			edit_product(prod_list);
			break;
		case 5: // View Outstanding Transactions
			print_transaction_list(xaction_list);
			break;
		case 6: // Delete All Outstanding Transactions
			clear_xactions(xaction_list);
		}
	} while (menu_option != 7);
}

bool owner_login(){
	char* temp;
	
	printf("username:");
	temp=grabword(stdin);
	if (strcmp(temp,"owner"))
		return false;
	free(temp);
	
	printf("password:");
	temp=grabword(stdin);
	if (strcmp(temp,"password"))
		return false;
	free(temp);
	
	return true;
}

void print_product_list(DS prod_list){
	Prod product;
	
	(void) pview(prod_list, 0); // set the view pointer to NULL
	
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
	char* prod_id;
	Prod prod_rec;
	
	printf("Enter the ID of the Product to Delete: ");
	prod_id=grabword(stdin);
	
	prod_rec=iview(prod_list, prod_id);
	
	if (prod_rec == NULL){
		printf("\nThat Product Does not Exist\n");
		return;
	}
	
	print_prod_heading(stdout);
	print_product(stdout, prod_rec);
	
	puts("Are you sure you want to delete this Product Record (y/n)?");
	if (prompt() != 'y'){
		puts("Canceling...");
		return;
	}
	// delete the data
	free(prod_rec);
	// delete the node
	iremove(prod_list, prod_id);
	return;
}

void edit_product(DS prod_list){
	int menu_option=0;
	char* prod_id;
	char* input;
	Prod prod_rec;
	
	printf("Enter the ID of the Product to Edit: ");
	prod_id=grabword(stdin);
	
	prod_rec=iview(prod_list, prod_id);
	
	if (prod_rec == NULL){
		printf("\nThat Product Does not Exist\n");
		return;
	}
	
	do {
		puts("");
		printf("\tEditing: %s\n", prod_rec->name);
		print_prod_heading(stdout);
		print_product(stdout, prod_rec);
		
		puts("\t============================");
		puts("\t1. Edit the Quantity on Hand");
		puts("\t2. Change the Price");
		puts("\t3. Back");
		
		menu_option=prompt();
		
		switch (menu_option){
		case 1:
			printf("Enter the new Quantity:");
			
			input=grabword(stdin);
			prod_rec->num_unit=atoi(input);
			free(input);
			break;
		case 2:
			printf("Enter the new Price:");
			
			input=grabword(stdin);
			prod_rec->price=atof(input);
			free(input);
		}
	} while (menu_option != 3);
}

void print_transaction_list(DS xaction_list){
	Trans xaction;
	
	(void) pview(xaction_list, 0); // set the view pointer to NULL
	
	print_xaction_heading(stdout);
	
	while ((xaction=view_next(xaction_list)) != NULL){
		print_xaction(stdout, xaction);
	}
}

void clear_xactions(DS xaction_list){
	Trans temp;
	
	if(isempty(xaction_list)){
		puts("There are no Outstanding Transactions");
		return;
	}
	
	print_transaction_list(xaction_list);
	
	puts("Are you sure you want to delete all outstanding transactions (y/n)?");
	if (prompt() != 'y'){
		puts("Canceling...");
		return;
	}
	
	while((temp=pop(xaction_list)) != NULL)
		free(temp);
	return;
}


