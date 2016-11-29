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

void print_file            (const char*);
void add_product           (DS         );
void delete_product        (DS         );
void edit_product          (DS         );
void print_transaction_list(DS         );
void clear_xactions        (DS         );

/************************************************************/
/*						The Owner Menu						*/
/************************************************************/
//USES:	ShopSys.c:	prompt()

void owner_menu(DS prod_list, DS xaction_list){
	int menu_option=0;
	
	do {
		puts(""                               );
		puts("\t         OWNER MENU"          );
		puts("\t=============================");
		puts("\t1. List Products for Sale"    );
		puts("\t2. Add a New Product"         );
		puts("\t3. Delete a Product"          );
		puts("\t4. Edit an Existing Product"  );
		puts("\t5. View Recent Transactions"  );
		puts("\t6. Delete Recent Transactions");
		puts("\t7. View All Transactions"     );
		puts("\t8. Delete All Transactions"   );
		puts("\t9. Exit to Main Menu"         );
		
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
		case 5: // View Recent Transactions
			print_transaction_list(xaction_list);
			break;
		case 6: // Delete All Recent Transactions
			puts("Confirm Delete All Recent Transactions (y/n)?");
			if (prompt() == 'y')
				clear_xactions(xaction_list);
			else puts("Canceling...");
			break;
		case 7: // View All Transactions
			print_transaction_list(xaction_list);
			print_file(DEFAULT_XACTION_FILE);
			break;
		case 8: // Delete All Transactions
			puts("Confirm Delete All Transactions (y/n)?");
			if (prompt() == 'y'){
				clear_xactions(xaction_list);
				fclose(fopen(DEFAULT_XACTION_FILE, "w"));
			}else puts("Canceling...");
		default: break;
		}
	} while (menu_option != 9);
}

/************************************************************/
/*               Print the Contents of a File               */
/************************************************************/

void print_file(const char* filename){
	char c;
	FILE* fd;
	
	fd=fopen(filename, "r");
	
	c= (char) fgetc(fd);
	while (!feof(fd)){
		putchar(c);
		c= (char) fgetc(fd);
	}
	fclose(fd);
}

/************************************************************/
/*						Add a New Product					*/
/************************************************************/
//USES: input.h:		grabword()
//						grabline()
//		data.h:			sort()
//						DS_find()
//		formatting.c:	print_prod_heading()
//						print_product()

void add_product(DS prod_list){
	struct Product new_product;
	char *         temp;
	Prod           product;
	
	printf("\nEnter new Product ID:");
	new_product.ID=grabword(stdin);
	
	if (( product = (Prod)DS_find(prod_list, new_product.ID) )){
	// make sure the product ID is unique
		puts("That Product ID is already in Use:");
		print_prod_heading(stdout);
		print_product(stdout, product);
		puts("");
		free(new_product.ID);
		return;
	}
	
	printf("Enter new Product Name:");
	new_product.name=grabline(stdin);
	printf("Enter new Product price:");
	temp = grabword(stdin);
	new_product.price = (float)atof(temp);
	free(temp);
	printf("Enter new Product Quantity on hand:");
	temp=grabword(stdin);
	new_product.num_unit = atoi(temp);
	free(temp);
	
	if( DS_insert(prod_list, &new_product) ){
		print_prod_heading(stdout);
		print_product(stdout, &new_product);
	}
	else puts ("something went wrong");
}

/************************************************************/
/*					Delete a Product						*/
/************************************************************/
//USES: input.h:		grabword()
//		data.h:			DS_find()
//						DS_remove()
//		formatting.c	print_prod_heading()
//						print_product()
//		ShopSys.c		prompt()


void delete_product(DS prod_list){
	char * prod_id;
	Prod prod_rec;
	
	printf("Enter the ID of the Product to Delete: ");
	prod_id=grabword(stdin);
	
	prod_rec=(Prod)DS_find(prod_list, prod_id);
	
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
	
	// delete the node
	DS_remove(prod_list);
	return;
}

/************************************************************/
/*						Edit a Product						*/
/************************************************************/
//USES: input.h:		grabword()
//		data.h:			DS_find()
//		formatting.c	print_prod_heading()
//						print_product()
//		ShopSys.c		prompt()

void edit_product(DS prod_list){
	int menu_option=0;
	char* input;
	Prod prod_rec;
	
	printf("Enter the ID of the Product to Edit: ");
	input=grabword(stdin);
	
	prod_rec=(Prod)DS_find(prod_list, input);
	
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
		puts("\t2. Change the Price"         );
		puts("\t3. Back"                     );
		
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
			prod_rec->price=(float)atof(input);
			free(input);
		default: break;
		}
	} while (menu_option != 3);
}

/************************************************************/
/*				Print the Whole Transaction List			*/
/************************************************************/
//USES:	data.h:			DS_isempty()
//						DS_first()
//						DS_next()
//		formatting.c:	print_xaction_heading()
//						print_xaction()

void print_transaction_list(DS xaction_list){
	Trans xaction;
	
	if (DS_isempty(xaction_list))
		puts("No Recent Transactions.");
	else {
		xaction = (Trans)DS_first(xaction_list);
		do {
			print_xaction(stdout, xaction);
		} while (( xaction = (Trans)DS_next(xaction_list) ));
	}
}

/************************************************************/
/*				Clear All Transactions						*/
/************************************************************/
//USES: data.h:			DS_isempty()
//						DS_pop()
//		formatting.c:	print_transaction_list()
//		ShopSys.c		prompt()


void clear_xactions(DS xaction_list){
	//Trans temp;
	
	if(DS_isempty(xaction_list)){
		puts("There are no Outstanding Transactions");
		return;
	}
	
	print_transaction_list(xaction_list);
	
	puts("Are you sure you want to delete all outstanding transactions (y/n)?");
	if (prompt() != 'y'){
		puts("Canceling...");
		return;
	}
	
	while(( DS_pop(xaction_list) ));
	return;
}


