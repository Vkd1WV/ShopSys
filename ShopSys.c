/******************************************************************************/
//  Author: Ammon Dodson
//          Daryan Hanshew
//  CES202
//  Winter 2016
//
//  This program should be built with make. A makefile has been provided.
//
/******************************* COMPONENTS ***********************************/
//
// Makefile     contains build instructions
//
//  data.c  data.h      stand-alone data structure library
// input.c input.h      stand-alone input library
//
// global.h     contains type definitions, prototypes, and includes used
//              throughout
//
// ShopSys.c    contains the main menu and file activities
//   int  main                   (                                      )
//   bool owner_login            (                                      )
//   Prod read_product           (FILE* product_file_discriptor         )
//   DS   read_product_file      (const char* file_name                 )
//   int  write_product_file     (const char* file_name, DS product_data)
//   int  append_transaction_file(const char* file_name, DS xaction_data)
//
// owner.c      Contains the owner's menu and functions
//   void owner_menu            (DS product_data    , DS transaction_data)
//   void add_product           (DS product_data                         )
//   void delete_product        (DS product_data                         )
//   void edit_product          (DS product_data                         )
//   void print_transaction_list(DS transaction_data                     )
//   void clear_xactions        (DS transaction_data                     )
//   void print_file            (const char* filename                    )
//
// customer.c   Contains the customer's menu and functions
//   void customer_menu(DS prod_list, DS trans_list               )
//   int cart_menu     (Trans cart  , DS prod_list , DS trans_list)
//   void update_cart  (Trans cart                                )
//   void add_to_cart  (Trans cart  , DS prod_list                )
//   void searchByName (DS prod_list                              )
//   void edit_item    (Trans cart  , DS prod_list                )
//   int checkout      (Trans cart  , DS prod_list , DS trans_list)
//
// services.c   Cotains functions used in multiple places
//   int  prompt               (                              )
//   void print_product_list   (DS product_data               )
//   void print_prod_heading   (FILE* file_discriptor         )
//   void print_product        (FILE* file_discriptor, Prod p )
//   void print_xaction_heading(FILE* file_discriptor         )
//   void print_xaction        (FILE* file_discriptor, Trans t)
//
/******************************************************************************/

#include "global.h"

// LOCAL FUNCTIONS
DS   read_product_file      (const char*    );
Prod read_product           (FILE*          );
int  write_product_file     (const char*, DS);
int  append_transaction_file(const char*, DS);
bool owner_login            (               );

/************************************************************/
/*							MAIN							*/
/************************************************************/
//USES:	services.c:	prompt()
//					owner_login()
//		data.h:		new_DS()
//		owner.c:	owner_menu()
//		customer.c:	customer_menu()

int main (int argc, const char **argv){
	const char* product_file=DEFAULT_PRODUCT_FILE;
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
		puts("\t         MAIN MENU"         );
		puts("\t===========================");
		puts("\t1. Owner Login"             );
		puts("\t2. Customer Login"          );
		puts("\t3. Exit, Saving All Changes");
		
		menu_option=prompt();
		
		switch (menu_option){
		case 1:
			if (owner_login())
				owner_menu(product_list, transaction_list);
			else puts("Tried to break in did ye?");
			break;
		case 2:
			customer_menu(product_list, transaction_list);
			break;
		}
		
	} while (menu_option != 3);
	
	// Saving Data on Exit
	puts("Exiting...");
	
	write_product_file(product_file, product_list);
	append_transaction_file(DEFAULT_XACTION_FILE, transaction_list);
	
	return EXIT_SUCCESS;
}

/************************************************************/
/*						The Owner Login						*/
/************************************************************/
//USES:	input.h:	grabword()

bool owner_login(){
	char* temp;
	
	printf("username:");
	temp=grabword(stdin);
	if (strcmp(temp, USERNAME))
		return false;
	free(temp);
	
	printf("password:");
	temp=grabword(stdin);
	if (strcmp(temp, PASSWORD))
		return false;
	free(temp);
	
	return true;
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
	
	while ((temp=pop(xaction_list)) != NULL){
		print_xaction(fd, temp);
		free(temp);
	}
	
	fclose(fd);
	return EXIT_SUCCESS;
}
