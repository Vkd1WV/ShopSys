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
// services.c   Cotains functions used in multiple places
//   int  prompt               (                              )
//   void print_product_list   (DS product_data               )
//   void print_prod_heading   (FILE* file_discriptor         )
//   void print_product        (FILE* file_discriptor, Prod p )
//   void print_xaction_heading(FILE* file_discriptor         )
//   void print_xaction        (FILE* file_discriptor, Trans t)
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
//   int  cart_menu   (Trans cart , DS prod_list, DS trans_list  )
//   void update_cart (Trans cart                                )
//   void add_to_cart (Trans cart , DS prod_list                 )
//   void searchByName(DS prodList                               )
//   void edit_item   (Trans cart , DS prod_list                 )
//   int  checkout    (Trans cart , DS prod_list, DS xaction_list)
//
//   void print_sorted(int (*) (const void*, const void*) , DS)
//
//   int  by_qty      (const void* first, const void* second)
//   int  by_price    (const void* first, const void* second)
//
/******************************************************************************/

#include "global.h"
#include <time.h>

//LOCAL FUNCTIONS
int  cart_menu   (Trans cart , DS prod_list, DS trans_list  );
void update_cart (Trans cart                                );
void add_to_cart (Trans cart , DS prod_list                 );
void searchByName(DS prodList                               );
void edit_item   (Trans cart , DS prod_list                 );
int  checkout    (Trans cart , DS prod_list, DS xaction_list);

void print_sorted (int (*) (const void*, const void*) , DS);

int by_qty  (const void* first, const void* second);
int by_price(const void* first, const void* second);


/************************************************************/
/*                   The Customer Menu                      */
/************************************************************/
//USES: ShopSys.c:	prompt()

void customer_menu(DS prod_list, DS trans_list){
	int                menu_option=0;
	struct Transaction cart;

	cart.items=DS_new(
		DS_bst,
		sizeof(struct Product),
		false,
		&cmp_product,
		&cmp_prod_key
	);

	// get customer information
	printf("First Name(s): ");
	cart.f_name=grabline(stdin);
	printf("Last Name(s): ");
	cart.l_name=grabline(stdin);
	printf("Address: ");
	cart.address=grabline(stdin);

	do {
		puts(""                                           );
		puts("\t              CUSTOMER MENU"              );
		puts("\t=========================================");
		puts("\t1. View Products Sorted by ID"            );
		puts("\t2. View Products Sorted by Quantity"      );
		puts("\t3. View Products Sorted by Price"         );
		puts("\t4. Search for Products by Name"           );
		puts("\t5. Add Item to Cart"                      );
		puts("\t6. View Cart (Go to Checkout)"            );
		puts("\t7. Exit to Main Menu (Clearing Cart)"     );
		puts("\t(Items remaining in cart will be removed)");
		
		menu_option=prompt();

		switch (menu_option){
		case 1: // View Products Sorted by ID
			print_product_list(prod_list);
			break;
		
		case 2: // View Products Sorted by Quantity on Hand
			print_sorted(&by_qty, prod_list);
			break;
		
		case 3: // View Products Sorted by Price
			print_sorted(&by_price, prod_list);
			break;
		
		case 4: // Search for Products by Name
			searchByName(prod_list);
			break;
		
		case 5: // Add Item to Cart
			add_to_cart(&cart, prod_list);
			break;
		
		case 6: // view cart
			if(cart_menu(&cart, prod_list, trans_list) == EXIT_SUCCESS)
				return;
			break;
		
		case 7:
			if(!DS_isempty(cart.items)){
				puts("There are still items in your cart. Are you sure you want to leave (y/n)?");
				if (prompt() != 'y') menu_option=0;
			}
		}
	} while (menu_option != 7);
	
	// Free memory
	puts("Emptying Cart...");
	DS_delete(cart.items);
}


/************************************************************/
/*                       Cart Menu                          */
/************************************************************/
//USES: services.c:	prompt()

int cart_menu(Trans cart, DS prod_list, DS trans_list){
	int menu_option=0;
	
	do {
		update_cart(cart);
		print_xaction(stdout, cart);
		
		puts(""                       );
		puts("\t        CART"         );
		puts("\t=====================");
		puts("\t1. Edit Item Quantity");
		puts("\t2. Checkout"          );
		puts("\t3. Back"              );

		menu_option=prompt();

		switch (menu_option){
		case 1: // Edit Item Quantity
			edit_item(cart, prod_list);
			break;
		case 2: // Checkout
			if(checkout(cart, prod_list, trans_list) == EXIT_SUCCESS)
				return EXIT_SUCCESS; // Sale :D
		}
	} while (menu_option != 3);
	return EXIT_FAILURE; // No Sale :(
}


/************************************************************/
/*             Update the Cart's date and Total             */
/************************************************************/
//USES: data.h:	DS_first()
//				DS_next()

void update_cart(Trans cart){
	struct tm* tm_struct;
	Prod       product;
	time_t     seconds;
	
	// set date
	seconds=time(NULL);
	tm_struct=localtime(&seconds);
	cart->dd=tm_struct->tm_mday;
	cart->mm=tm_struct->tm_mon+1;
	cart->yy=tm_struct->tm_year+1900;
	
	restart:
	// update the total
	cart->pay=0;
	
	if(DS_isempty(cart->items)) return;
	
	product=DS_first(cart->items);
	do{
		// if there are 0 units we remove the item
		if (product->num_unit <1){
			DS_remove(cart->items);
			goto restart;
			// traversals must be restarted after a removal
		}
		else cart->pay += product->price * product->num_unit;
		
	} while (( product = DS_next(cart->items) ));
}

/************************************************************/
/*           Search for a product by it's names             */
/************************************************************/
//USES: input.h:    grabword()
//      services.c: print_prod_heading()
//                  print_product()
//      data.h:     DS_isempty()
//                  DS_first()
//                  DS_next()

void searchByName(DS prodList){
	char* search_term;
	Prod  product;

	printf("Enter Name of the desired product: ");
	search_term=grabword(stdin);

	puts("\nResults:");
	print_prod_heading(stdout);

	product=DS_first(prodList);
	do {
		if(strstr(product->name, search_term) != NULL)
			print_product(stdout, product);
	} while (( product = DS_next(prodList) ));

	free(search_term);
}

/************************************************************/
/*                 Add an Item to the cart                  */
/************************************************************/
//USES: input.h:    grabword()
//      data.h:     DS_find()
//                  DS_first()
//                  DS_next()
//                  DS_sort()
//      services.c: print_prod_heading()
//                  print_product()


void add_to_cart(Trans cart, DS prod_list){
	char *         input;
	int            num_unit;
	Prod           prod_rec;
	struct Product new_item;

	printf("\nEnter ID of the product: ");
	input = grabword(stdin);

	// Find the Product
	prod_rec=DS_find(prod_list, input);
	if (prod_rec == NULL){
		printf("\nThat Product Does not Exist\n");
		return;
	}

	// check if the item is already in the cart
	if( !DS_isempty(cart->items) && DS_find(cart->items, input) ){
		printf("\nThat Product is already in your cart.\n");
		puts("You can change the quantity from the cart menu.");
		
		free(input);
		return;
	}
	
	if (!prod_rec->num_unit){
		printf("\nWe're sorry. That product is currently out of stock.\n");
		free(input);
		return;
	}
	
	print_prod_heading(stdout);
	print_product(stdout, prod_rec);

	do {
		free(input);
		printf("\nEnter Number of Units: ");
		input=grabword(stdin);
		num_unit = atoi(input);
	} while (num_unit > prod_rec->num_unit);

	free(input);

	new_item.ID       = prod_rec->ID;
	new_item.name     = prod_rec->name;
	new_item.price    = prod_rec->price;
	new_item.num_unit = num_unit;

	DS_sort(cart->items, &new_item);
}


/************************************************************/
/*                 Edit an Item in the cart                 */
/************************************************************/
//USES: input.h:    grabword()
//      data.h:     DS_find()
//      services.c: print_prod_heading()
//                  print_product()


void edit_item(Trans cart, DS prod_list){
	char * input;
	int num_unit;
	Prod prod_rec, item;

	printf("\nEnter ID of the product: ");
	input = grabword(stdin);

	// find the item
	item=DS_find(cart->items, input);
	if (item == NULL){
		printf("\nThat Product is not in your cart.\n");
		return;
	}

	// Find the Product
	prod_rec=DS_find(prod_list, input);

	print_prod_heading(stdout);
	print_product(stdout, item);

	do { // check that there are enough units in stock
		free(input);
		printf("\nEnter New Number of Units (enter 0 to remove item): ");
		input=grabword(stdin);
		num_unit = atoi(input);
	} while (num_unit > prod_rec->num_unit);

	free(input);

	item->num_unit = num_unit;
}


/************************************************************/
/*                  Checkout the Customer                   */
/************************************************************/
//USES: services.c: prompt()
//      data.h:     DS_first()
//                  DS_next()
//                  DS_find()
//                  DS_push()


int checkout(Trans cart, DS prod_list, DS xaction_list){
	Prod item, product;
	
	if (DS_isempty(cart->items)){
		puts("Your cart is empty.");
		return EXIT_FAILURE;
	}

	puts("Confirm Checkout (y/n)?");
	if(prompt() != 'y')
		return EXIT_FAILURE;

	// Update the product list
	item = DS_first(cart->items);
	do{
		product=DS_find(prod_list, item->ID); // find the corresponding product
		product->num_unit -= item->num_unit; // and change its quantity
	} while (( item = DS_next(cart->items) ));

	DS_push(xaction_list, cart);
	return EXIT_SUCCESS;
}


/************************************************************/
/*            Print a Resorted Product List                 */
/************************************************************/
//USES: data.h: DS_count()
//              DS_first()
//              DS_next()


void print_sorted(int (compare) (const void*, const void*), DS prod_list){
	Prod* index;
	unsigned int num_nodes, i;
	
	// get the size of the data set
	num_nodes=DS_count(prod_list);
	
	// create a new index
	index=calloc(sizeof(Prod), num_nodes);
	if (index == NULL) {
		puts("malloc(): returned NULL");
		return;
	}
	
	// set the index pointers to each product
	index[0] = (Prod) DS_first(prod_list);
	for (i=1; i<num_nodes; i++)
		index[i] = (Prod) DS_next(prod_list);
	
	// sort the index
	qsort(index, num_nodes, sizeof(Prod), compare);
	
	// print the results
	print_prod_heading(stdout);
	for (i=0; i<num_nodes; i++){
		print_product(stdout, index[i]);
	}
	
	free(index);
}

int by_qty (const void* first, const void* second){
	return (*(Prod*) second)->num_unit - (*(Prod*) first)->num_unit;
}

int by_price (const void* first, const void* second){
	return (*(Prod*) second)->price - (*(Prod*) first)->price;
}
