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
//		: Prod	read_product			(FILE* product_file_descriptor			);
//		: DS	read_product_file		(const char* file_name					);
//		: int	write_product_file		(const char* file_name, DS product_data	);
//		: int	append_transaction_file	(const char* file_name, DS xaction_data	);
//
//	formatting.c	Prints out data to the screen or files
//		: void print_prod_heading		(FILE* file_descriptor			);
//		: void print_product			(FILE* file_descriptor, Prod p	);
//		: void print_xaction_heading	(FILE* file_descriptor			);
//		: void print_xaction			(FILE* file_descriptor, Trans t	);
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

/**	
*/

/************************************************************/
/*		Read a single product record from a file			*/
/************************************************************/
//USES:	input.h:	grabword()
//					grabfield()

Prod read_product(FILE* file){
	Prod prod_rec;
	char* temp;
	
	// Allocate memory
	prod_rec=malloc(sizeof(struct Product));
	if ( prod_rec==NULL ) {
		puts("read_product(): ERROR: malloc() returned NULL");
		return NULL;
	}
	
	// PRODUCT ID
	prod_rec->ID=grabfield(file);
	if (prod_rec->ID == NULL ){
		free(prod_rec);
		return NULL;
	}
	// PRODUCT NAME
	prod_rec->name=grabfield(file);
	// QUANTITY
	temp=grabfield(file);
	prod_rec->num_unit=atoi(temp);
	free(temp);
	// PRICE
	temp=grabword(file);
	
	// ignore the dollar sign if present
	if (*temp == '$'){
		free(temp);
		temp=grabword(file);
	}
	
	prod_rec->price=atof(temp);
	free(temp);

	return prod_rec;
}

/************************************************************/
/*			Read all product record from a file				*/
/************************************************************/
//USES:	input.h:	grabline()
//		data.h:		new_DS()
//					sort()


/**	Read the contents of products.txt and store that data in a linked list
 *	returns a pointer to the linked list
 *	returns NULL on failure
*/
DS read_product_file(const char* file_name){
	FILE* product_fd;
	Prod new_prod_rec;
	DS product_list; // the data structure type is provided by data.h
	//char* word;
	//int valid=1;
	
	
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
/*	word=grabword(product_fd);*/
/*	if(strcmp(word, "Product_ID")) {*/
/*		valid=0;*/
/*		goto skip_validation;*/
/*	}*/
/*	free(word);*/
/*	word=grabword(product_fd);*/
/*	if(strcmp(word, "Product_name")) {*/
/*		valid=0;*/
/*		goto skip_validation;*/
/*	}*/
/*	free(word);*/
/*	word=grabword(product_fd);*/
/*	if(strcmp(word, "Number_of_units")) {*/
/*		valid=0;*/
/*		goto skip_validation;*/
/*	}*/
/*	free(word);*/
/*	word=grabword(product_fd);*/
/*	if(strcmp(word, "Price_of_unit")) {*/
/*		valid=0;*/
/*		goto skip_validation;*/
/*	}*/
/*	free(word);*/
/*	*/
/*	skip_validation:*/
/*	if(!valid) {*/
/*	fclose(product_fd);*/
/*	printf("read_product_file(): ERROR: file '%s' does not have \n\*/
/*correct file format\n", \*/
/*			file_name);*/
/*	return NULL;*/
/*	}*/
	
	//eat the first line
	free(grabline(product_fd));
	
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


/************************************************************/
/*		Write the product data to the product.txt file		*/
/************************************************************/
//USES:	data.h:			pop()
//		formatting.c	print_prod_heading()
//						print_product()


/**	returns EXIT_FAILURE or EXIT_SUCCESS
 *	This function must only be accessed when the program closes because it
 *	clears the product data structure
*/
int write_product_file(const char* file_name, DS product_list){
	FILE* fd;
	Prod temp;
	
	// open the file
	fd=fopen(file_name, "w");
	if (fd == NULL){
		printf("There was a error opening %s for writting", file_name);
		return EXIT_FAILURE;
	}
	
	print_prod_heading(fd);
	
	while ((temp=pop(product_list)) != NULL){
		print_product(fd, temp);
		free(temp);
	}
	
	fclose(fd);
	return EXIT_SUCCESS;
}


/************************************************************/
/*	append new transactions to the end of transactions.txt	*/
/************************************************************/
//USES:	data.h:			pop()
//		formatting.c	print_xaction_heading()
//						print_xaction()

/**	returns EXIT_FAILURE or EXIT_SUCCESS
 *	This function must only be accessed when the program closes
*/
int append_transaction_file(const char* file_name, DS xaction_list){
	FILE* fd;
	Trans temp;
	
	// open the file
	fd=fopen(file_name, "a");
	if (fd == NULL){
		printf("append_transaction_file(): There was a error opening %s for append", file_name);
		return EXIT_FAILURE;
	}
	
	print_xaction_heading(fd);
	
	while ((temp=pop(xaction_list)) != NULL){
		print_xaction(fd, temp);
		free(temp);
	}
	
	fclose(fd);
	return EXIT_SUCCESS;
}


