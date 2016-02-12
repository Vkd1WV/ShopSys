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

/**	product.txt has a header in the first line followed by one product record
 *	per line. Each product record has 4 tab-delimited fields.
 *	PRODUCT ID	PRODUCT NAME	QUANTITY ON HAND	PRICE
 */

/**	transaction.txt has a header in the first line followed by one transaction
 *	record per line.
 *	each transaction record has 5 tab-delimited fields:
 *	First Name	last Name	Amount of payment	date of payment	list of
 *	purchased products
 */

/**	Read a single product record from a file
*/
Prod read_product(FILE* file){
	Prod prod_rec;
	
	// Allocate memory
	prod_rec=malloc(sizeof(struct Product));
	if ( prod_rec==NULL ) {
		puts("read_product(): ERROR: malloc() returned NULL");
		return NULL;
	}
	
	// PRODUCT ID
	prod_rec->ID=grabword(file);
	if (prod_rec->ID == NULL){
		free(prod_rec);
		return NULL;
	}
	// PRODUCT NAME
	prod_rec->name=grabword(file);
	// QUANTITY
	fscanf(file, "%d", &(prod_rec->num_unit));
	// PRICE
	fscanf(file, "%f", &(prod_rec->price));
	
	return prod_rec;
}

/**	Read the contents of products.txt and store that data in a linked list
 *	returns a pointer to the linked list
 *	returns NULL on failure
*/
DS read_product_file(const char* file_name){
	FILE* product_fd;
	Prod new_prod_rec;
	DS product_list; // the data structure type is provided by data.h
	char* word;
	int valid=1;
	
	
	// open the product file
	product_fd=fopen(file_name, "r");
	// check that file opened
	if (product_fd == NULL) {
		printf("read_product_file(): file '%s' could not be opened.\n",
					file_name);
		return NULL;
	}
	
	// Check that the header is correct and move the file pointer to the
	// beginning of the data
	word=grabword(product_fd);
	if(strcmp(word, "Product_ID")) {
		valid=0;
		goto skip_validation;
	}
	free(word);
	word=grabword(product_fd);
	if(strcmp(word, "Product_name")) {
		valid=0;
		goto skip_validation;
	}
	free(word);
	word=grabword(product_fd);
	if(strcmp(word, "Number_of_units")) {
		valid=0;
		goto skip_validation;
	}
	free(word);
	word=grabword(product_fd);
	if(strcmp(word, "Price_of_unit")) {
		valid=0;
		goto skip_validation;
	}
	free(word);
	
	skip_validation:
	if(!valid) {
	fclose(product_fd);
	printf("read_product_file(): ERROR: file '%s' does not have \n\
correct file format\n", \
			file_name);
	return NULL;
	}
	
	// make a link list to contain product data.
	// Link list implementation provided by data.h
	
	product_list=new_DS('l');
	
	
	while ( !feof(product_fd) ) { // check for end of file
	
		new_prod_rec=read_product(product_fd); // read each product
		if (new_prod_rec == NULL)
			break;
		
		// add it to the DS
		if(sort(product_list, new_prod_rec, new_prod_rec->ID)){
			puts("read_product_file():ERROR in adding data to sorted DS");
			return NULL;
		}
	}
	
	fclose(product_fd);
	return product_list;
}


/**	Write the product data to the product.txt file.
 *	returns EXIT_FAILURE or EXIT_SUCCESS
 *	This function must only be accessed when the program closes
*/
int write_product_file(const char* file_name, DS product_list){
	file_name=file_name;
	product_list=product_list;
	return EXIT_SUCCESS;
}


/**	append new transactions to the end of transactions.txt
 *	returns EXIT_FAILURE or EXIT_SUCCESS
 *	This function must only be accessed when the program closes
*/
int append_transaction_file(const char* file_name, DS xaction_list){
	file_name=file_name;
	xaction_list=xaction_list;
	return EXIT_SUCCESS;
}


