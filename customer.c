/******************************************************************************/
//  Author: Ammon Dodson
//  		Daryan Hanshew
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
//
// customer.c   Contains the customer's menu and functions
//   void customer_menu    (DS prod_list, DS trans_list)
//   void sort_menu        (DS prod_list               )
//   void update_cart      (Trans cart                 )
//   void add_to_cart      (Trans cart  , DS prod_list )
//   void searchByName     (DS prod_list               )
//   void printByUnits     (DS prod_list               )
//   void printByPrice     (DS prod_list               )
//   Prod copyProd         (Prod                       )
//   void updateProductList(DS          , DS           )
//
// services.c   Cotains functions used in multiple places
//   int  prompt               (                              )
//   bool owner_login          (                              )
//   void print_product_list   (DS product_data               )
//   void print_prod_heading   (FILE* file_discriptor         )
//   void print_product        (FILE* file_discriptor, Prod p )
//   void print_xaction_heading(FILE* file_discriptor         )
//   void print_xaction        (FILE* file_discriptor, Trans t)
//
/******************************************************************************/

#include "global.h"

#include <time.h>
#include <string.h>

//LOCAL FUNCTIONS
int cart_menu    (Trans, DS , DS);
void update_cart (Trans         );
void add_to_cart (Trans, DS     );
void searchByName(DS            );
void edit_item   (Trans, DS     );
int checkout     (Trans, DS , DS);

void sortStack(DS prod_list, int type);
void sort_product_list(DS prod_list, int type);

/*void printByUnits     (DS       );*/
/*void printByPrice     (DS       );*/
/*Prod copyProd         (Prod     );*/
/*void updateProductList(DS   , DS);*/


/************************************************************/
/*                   The Customer Menu                      */
/************************************************************/
//USES: ShopSys.c:	prompt()

void customer_menu(DS prod_list, DS trans_list){
	int menu_option=0;
	Trans cart;
	Prod product;

	// allocate space
	cart=malloc(sizeof(struct Transaction));
	if (cart == NULL) {
		puts("malloc(): returned NULL");
		return;
	}
	cart->items=new_DS('l');

	// get customer information
	printf("First Name(s): ");
	cart->f_name=grabline(stdin);
	printf("Last Name(s): ");
	cart->l_name=grabline(stdin);
	printf("Address: ");
	cart->address=grabline(stdin);

	do {
		puts("");
		puts("\t              CUSTOMER MENU");
		puts("\t===========================================");
		puts("\t1. View Products Sorted by ID");
		puts("\t2. View Products Sorted by Quantity on Hand");
		puts("\t3. View Products Sorted by Price");
		puts("\t4. Search for Products by Name");
		puts("\t5. Add Item to Cart");
		puts("\t6. View Cart (Go to Checkout)");
		puts("\t7. Exit to Main Menu (Clearing Cart)");
		puts("\t(Items remaining in cart will be removed)");

		menu_option=prompt();

		switch (menu_option){
		case 1: // View Products Sorted by ID
			print_product_list(prod_list);
			break;
		case 2: // View Products Sorted by Quantity on Hand
			sort_product_list(prod_list, 1);
			break;
		case 3: // View Products Sorted by Price
			sort_product_list(prod_list, 0);

			break;
		case 4: // Search for Products by Name
			searchByName(prod_list);
			break;
		case 5: // Add Item to Cart
			add_to_cart(cart, prod_list);
			break;
		case 6: // view cart
			if(cart_menu(cart, trans_list, prod_list) == EXIT_SUCCESS)
				return;
			break;
		case 7:
			if(!isempty(cart->items)){
				puts("There are still items in your cart. Are you sure you want to leave (y/n)?");
				if (prompt() != 'y') menu_option=0;
			}
		}
	} while (menu_option != 7);

	// Free memory
	puts("Emptying Cart");
	while((product=pop(cart->items)) != NULL)
		free(product);
	free(cart->items);
	free(cart);
}

/************************************************************/
/*                       Cart Menu                          */
/************************************************************/
//USES: services.c:	prompt()

int cart_menu(Trans cart, DS trans_list, DS prod_list){
	int menu_option=0;
	do {
		update_cart(cart);
		print_xaction(stdout, cart);

		puts("");
		puts("\t        CART");
		puts("\t=====================");
		puts("\t1. Edit Item Quantity");
		puts("\t2. Checkout");
		puts("\t3. Back");

		menu_option=prompt();

		switch (menu_option){
		case 1: // Edit Item Quantity
			edit_item(cart, prod_list);
			break;
		case 2: // Checkout
			if(checkout(cart, prod_list, trans_list) == EXIT_SUCCESS)
				return EXIT_SUCCESS;
		}
	} while (menu_option != 3);
	return EXIT_FAILURE;
}

/************************************************************/
/*             Update the Cart's date and Total             */
/************************************************************/
//USES: data.h:	pview()
//				view_next()

void update_cart(Trans cart){
	struct tm* tm_struct;
	Prod product;
	time_t seconds;

	// set date
	seconds=time(NULL);
	tm_struct=localtime(&seconds);
	cart->dd=tm_struct->tm_mday;
	cart->mm=tm_struct->tm_mon+1;
	cart->yy=tm_struct->tm_year+1900;

	// update the total
	cart->pay=0;
	pview(cart->items, 0); // initialize the view_next pointer
	while((product=view_next(cart->items)) != NULL){
		if (product->num_unit <1){
			iremove(cart->items, product->ID);
			/* After removing a node the view pointer is reset so we have to */
			/* start the process over again */
			cart->pay=0;
		}else
			cart->pay += product->price * product->num_unit;
	}
}

/************************************************************/
/*           Search for a product by it's names             */
/************************************************************/
//USES: input.h:    grabword()
//      services.c: print_prod_heading()
//                  print_product()
//      data.h:     isempty()
//                  pview()
//                  view_next()

void searchByName(DS prodList){
	char* search_term;
	Prod product;

	printf("Enter Name of the desired product: ");
	search_term=grabword(stdin);

	puts("\nResults:");
	print_prod_heading(stdout);

	pview(prodList, 0); // start searching from the head of the linked list
	while((product=view_next(prodList)) != NULL)
		if(strstr(product->name, search_term) != NULL)
			print_product(stdout, product);

	free(search_term);
}

/************************************************************/
/*                 Add an Item to the cart                  */
/************************************************************/
//USES: input.h:    grabword()
//      data.h:     iview()
//                  pview()
//                  view_next()
//                  sort()
//      services.c: print_prod_heading()
//                  print_product()


void add_to_cart(Trans cart, DS prod_list){
	char* input;
	int num_unit;
	Prod prod_rec, new_item;

	printf("\nEnter ID of the product: ");
	input = grabword(stdin);

	// Find the Product
	prod_rec=iview(prod_list, input);
	if (prod_rec == NULL){
		printf("\nThat Product Does not Exist\n");
		return;
	}

	// check if the item is already in the cart
	if( iview(cart->items, input) != NULL ){
		printf("\nThat Product is already in your cart.\n");
		puts("You can change the quantity from the cart menu.");
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

	// add a new item to the cart
	new_item=malloc(sizeof(struct Product));
	if (new_item == NULL) {
		puts("malloc(): returned NULL");
		return;
	}

	new_item->ID       = prod_rec->ID;
	new_item->name     = prod_rec->name;
	new_item->price    = prod_rec->price;
	new_item->num_unit = num_unit;

	sort(cart->items, new_item, new_item->ID);
}


/************************************************************/
/*                 Edit an Item in the cart                 */
/************************************************************/
//USES: input.h:    grabword()
//      data.h:     iview()
//      services.c: print_prod_heading()
//                  print_product()


void edit_item(Trans cart, DS prod_list){
	char* input;
	int num_unit;
	Prod prod_rec, item;

	printf("\nEnter ID of the product: ");
	input = grabword(stdin);

	// find the item
	item=iview(cart->items, input);
	if (item == NULL){
		printf("\nThat Product is not in your cart.\n");
		return;
	}

	// Find the Product
	prod_rec=iview(prod_list, input);

	print_prod_heading(stdout);
	print_product(stdout, item);

	do {
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
//input.h:    grabword()
//
//                  print_product()
//      data.h:     isempty()
//                  pview()
//                  view_next()


int checkout(Trans cart, DS prod_list, DS xaction_list){
	Prod item, product;

	puts("Confirm Checkout (y/n)?");
	if(prompt() != 'y')
		return EXIT_FAILURE;

	// Update the product list
	pview(cart->items, 0);
	while((item=view_next(cart->items)) != NULL){ // for each item in the cart
		product =  iview(prod_list, item->ID); // find the corresponding product
		product->num_unit -= item->num_unit;   // and change its quantity
	}

	push(xaction_list, cart);
	return EXIT_SUCCESS;
}



/************************************************************/
/*Print the whole product List to the Screen in SORTED ORDER*/
/************************************************************/
//USES:	data.h:			pview()
//						view_next()

//type = 0 => sort by price
//type = 1 => sort by quantity
void sort_product_list(DS prod_list, int type){

	sortStack(prod_list, type);
	Prod product;
	(void) pview(prod_list, 0); // set the view pointer to NULL

	print_prod_heading(stdout);

	while ((product=view_next(prod_list)) != NULL){
		print_product(stdout, product);
	}
}



// Recursive function to insert an item x in sorted way
void sortedInsert(DS s, Prod x, int type)
{
	(void) pview(s, 0); // set the view pointer to NULL
	Prod top_element = view_next(s);
    // Base case: Either stack is empty or newly inserted
    // item is greater than top (more than all existing)

    int condition_result = 0;
    if(!isempty(s)){
    	switch(type){
    		case 0:
    			condition_result = x->price - top_element->price;
    			break;
    		case 1:
    			condition_result = x->num_unit - top_element->num_unit;
    			break;
    	}
    }

    if (isempty(s) || condition_result > 0)
    {
        push(s, x);
        return;
    }

    // If top is greater, remove the top item and recur
    Prod temp = pop(s);
    sortedInsert(s, x, type);

    // Put back the top item removed earlier
    push(s, temp);
}

// Function to sort stack
void sortStack(DS s, int type)
{
    // If stack is not empty
    if (!isempty(s))
    {
        // Remove the top item
        Prod x = pop(s);

        // Sort remaining stack
        sortStack(s, type);

        // Push the top item back in sorted stack
        sortedInsert(s, x, type);
    }
}


void printByPrice(DS productList){
	Prod product;
	Prod sortedList;
	if(productList->head == NULL){
		puts("No Products Available");
		return;
	}
	(void) pview(productList, 0); // set the view pointer to NULL
	sortedList = copyProd((Prod)productList->head->data);
	product =view_next(productList);
	while((product=(Prod)view_next(productList))!=NULL){
		Prod list = sortedList;
		Prod copy = copyProd(product);
		if(list->price>=product->price){
			copy->next = sortedList;
			sortedList = copy;
		}else{
			int inserted = 0;
			while(list->next!=NULL){
				//printf("\nNo  => %f: %f\n",product->price, list->next->price);
				if((list->next->price>product->price)){
					//printf("\nYes%f: %f\n",product->price, list->next->price);
					copy->next = list->next;
					list->next = copy;
					list = list->next;
					inserted = 1;
					break;
				}
				list = list->next;
			}
			if(inserted == 0){
				list->next = copy;
			}
		}

	}
	printProdList(sortedList);

}

void printByUnits(DS productList){
	Prod product;
	Prod sortedList;
	if(productList->head == NULL){
		puts("No Products Available");
		return;
	}
	(void) pview(productList, 0); // set the view pointer to NULL
	sortedList = copyProd((Prod)productList->head->data);
	product =view_next(productList);
	while((product=(Prod)view_next(productList))!=NULL){
		Prod list = sortedList;
		Prod copy = copyProd(product);
		if(list->num_unit>=product->num_unit){
			copy->next = sortedList;
			sortedList = copy;
		}else{
			int inserted = 0;
			while(list->next!=NULL){
				//printf("\nNo  => %f: %f\n",product->price, list->next->price);
				if((list->next->num_unit>product->num_unit)){
					//printf("\nYes%f: %f\n",product->price, list->next->price);
					copy->next = list->next;
					list->next = copy;
					list = list->next;
					inserted = 1;
					break;
				}
				list = list->next;
			}
			if(inserted == 0){
				list->next = copy;
			}
		}

	}
	printProdList(sortedList);

}
