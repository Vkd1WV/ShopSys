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
//	Makefile	contains build instructions
//
//	data.c data.h		stand-alone data structure library
//	input.c input.h		stand-alone input library
//
//	global.h	contains type definitions, prototypes, and includes used
//				throughout
//
//	ShopSys.c	contains the main menu and file activities
//		: int  main						(										);
//		: Prod read_product				(FILE* product_file_discriptor			);
//		: DS   read_product_file		(const char* file_name					);
//		: int  write_product_file		(const char* file_name, DS product_data	);
//		: int  append_transaction_file	(const char* file_name, DS xaction_data	);
//
//	owner.c		Contains the owner's menu and functions
//		: void owner_menu				(DS product_data, DS transaction_data);
//		: void add_product				(DS product_data		);
//		: void delete_product			(DS product_data		);
//		: void edit_product				(DS product_data		);
//		: void print_transaction_list	(DS transaction_data	);
//		: void clear_xactions			(DS transaction_data	);
//
//	customer.c	Contains the customer's menu and functions
//		: void sort_menu		(				);
//		: void update_cart		( Trans			);
//		: Cart createCartItem	(char*, int, DS	);
//		: void freeCartItems	(DS				);
//		: void searchByName		(DS				);
//		: void printByUnits		(DS				);
//		: void printByPrice		(DS				);
//		: Prod copyProd			(Prod			);
//		: void updateProductList(DS, DS			);
//
//	services.c	Cotains functions used in multiple places
//		; int  prompt				(								);
//		: bool owner_login			(								);
//		: void print_product_list	(DS product_data				);
//		: void print_prod_heading	(FILE* file_discriptor			);
//		: void print_product		(FILE* file_discriptor, Prod p	);
//		: void print_xaction_heading(FILE* file_discriptor			);
//		: void print_xaction		(FILE* file_discriptor, Trans t	);
//
/******************************************************************************/

#include "global.h"

#include <time.h>
#include <string.h>

//LOCAL FUNCTIONS
void sort_menu		(		);
void update_cart	( Trans	);
void add_to_cart	(DS, DS	);
void searchByName	(DS		);

void printByUnits	(DS);
void printByPrice	(DS);

Prod copyProd(Prod);
void updateProductList(DS, DS);


/************************************************************/
/*						The Customer Menu					*/
/************************************************************/
//USES: ShopSys.c:	prompt()

void customer_menu(DS prod_list, DS trans_list){
	int menu_option=0;
	Trans cart;
	Prod temp;
	
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
		puts("\t============================================");
		puts("\t1. Display Products");
		puts("\t2. Search for Product by Name");
		puts("\t3. Add Item to Cart");
		puts("\t4. View Cart");
		puts("\t5. Checkout");
		puts("\t6. Exit to Main Menu");
		puts("\t(Items remaining in cart will be removed)");
		
		menu_option=prompt();
		
		switch (menu_option){
		case 1: // Display Products
			sort_menu(prod_list);
			break;
		case 2: // Search for Product by Name
			searchByName(prod_list);
			break;
		case 3: // Add Item to Cart
			//add_to_cart(cart, prod_list);
			break;
		case 4: // view cart
		
			update_cart(cart);
			print_xaction(stdout, cart);
		
		break;
		case 5: // Checkout
			print_xaction(stdout, cart);
			
			//checkout(cart, prod_list, trans_list);
		}
	} while (menu_option != 6);
	
	// Free memory
	puts("Emptying Cart");
	while((temp=pop(cart->items)) != NULL)
		free(temp);
	free(cart->items);
	free(cart);
}

/************************************************************/
/*				Sort Products for the Customer				*/
/************************************************************/
//USES: ShopSys.c:	prompt()

void sort_menu(DS prod_list){
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
			print_product_list(prod_list);
			break;
		case 2: // Sort by Quantity on Hand
			//printByUnits(prod_list);
			break;
		case 3: // Sort by Price
			//printByPrice(prod_list);
			break;

		}
	} while (menu_option != 4);
}

/************************************************************/
/*				Update the Cart's date and Total			*/
/************************************************************/
//USES: data.h:	pview()
//				view_next()

void update_cart(Trans cart){
	struct tm* time_pt;
	Prod temp;
	time_t seconds;
	
	// set date
	seconds=time(NULL);
	time_pt=localtime(&seconds);
	cart->dd=time_pt->tm_mday;
	cart->mm=time_pt->tm_mon+1;
	cart->yy=time_pt->tm_year+1900;
	
	// update the total
	cart->pay=0;
	pview(cart->items, 0);
	while((temp=view_next(cart->items)) != NULL)
		cart->pay += temp->price;
}

/************************************************************/
/*			Search for a product by it's names				*/
/************************************************************/
//USES:	input.h:	grabword()
//		services.c:	print_prod_heading()
//					print_product()
//		data.h:		isempty()
//					pview()
//					view_next()

void searchByName(DS prodList){
	char* search_term;
	Prod temp;
	
	printf("Enter Name of the desired product: ");
	search_term=grabword(stdin);
	
	puts("\nResults:");
	print_prod_heading(stdout);
	
	pview(prodList, 0);
	while((temp=view_next(prodList)) != NULL)
		if(strstr(temp->name, search_term) != NULL)
			print_product(stdout, temp);
	
	free(search_term);
}

/*void add_to_cart(DS cart, DS prod_list){*/
/*	char* prod_id;*/
/*	char* input;*/
/*	Prod prod_rec;*/
/*	int num_unit;*/
/*	*/
/*	printf("\nEnter ID of the product: ");*/
/*	prod_id = grabword(stdin);*/
/*	*/
/*	prod_rec=iview(prod_list, prod_id);*/
/*	*/
/*	if (prod_rec == NULL){*/
/*		printf("\nThat Product Does not Exist\n");*/
/*		return;*/
/*	}*/
/*	*/
/*	printf("\nEnter Number of Units: ");*/
/*	input=grabword(stdin);*/
/*	num_unit = atoi(input);*/
/*	free(input);*/
/*	Prod product;*/
/*	*/
/*	*/
/*	Cart item = (Cart)malloc(sizeof(struct cart_));*/
/*	int itemFound = 0;*/
/*	(void) pview(productList, 0); // set the view pointer to NULL*/
/*	while((product=view_next(productList))!=NULL){*/
/*		if(strcmp(product->ID,ID)==0){*/
/*			if(product->num_unit >= num_unit ){*/
/*				item->ID = ID;*/
/*				item->num_unit = num_unit;*/
/*				item->price = product->price;*/
/*				item->next = NULL;*/
/*				item->name = product->name;*/
/*				itemFound = 1;*/
/*				break;*/
/*			}else{*/
/*				printf("\nOnly %d items of type %s is left",product->num_unit, product->ID);*/
/*				free(item);*/
/*				return NULL;*/
/*			}*/
/*		}*/
/*	}*/

/*	if(itemFound == 0){*/
/*		printf("Sorry, We didn't find your item. Try again with correct ID");*/

/*		return NULL;*/

/*	}*/
/*	return item;*/
/*	if(item != NULL){*/
/*		push(cart, item);*/
/*	}*/
/*	return cart;*/
/*}*/

/*void checkout(DS cart, DS productList, DS transactionList){*/
/*	struct _node * item = cart->head;*/
/*	*/
/*	float price = 0;*/
/*	while((item)!=NULL){*/
/*		Cart item_cart = item->data;*/
/*		fprintf(stdout,*/
/*			"%8s   %24s   %7d    $ %6.2f\n",*/
/*			item_cart->ID,*/
/*			item_cart->name,*/
/*			item_cart->num_unit,*/
/*			item_cart->price*item_cart->num_unit*/
/*		);*/
/*		price += item_cart->price*item_cart->num_unit;*/
/*		item = item->right;*/
/*	}*/
/*	puts("Are you sure you want to buy the Products in the cart (y/n)?");*/
/*	if(prompt()!='y'){*/
/*		puts("Cancelling....");*/
/*	}else{*/

/*		updateProductList(productList, cart);*/
/*		updateTransactionList(cart, transactionList, price);*/
/*		freeCartItems(cart);*/
/*	}*/
/*}*/

/*void updateProductList(DS productList, DS cartItems){*/
/*	(void) pview(productList, 0); // set the view pointer to NULL*/
/*	Prod product;*/
/*	while((product=view_next(productList))!=NULL){*/
/*		while(product!=NULL){*/
/*			if(strcmp(product->ID, item->ID)==0){*/
/*				product->num_unit -= item->num_unit;*/
/*				break;*/
/*			}*/
/*			list = list->right;*/
/*		}*/
/*	}*/

/*}*/

/*Prod copyProd(Prod a){*/
/*	Prod b = (Prod)malloc(sizeof(struct Product));*/
/*	b->ID = a->ID;*/
/*	b->name = a->name;*/
/*	b->price = a->price;*/
/*	b->num_unit = a->num_unit;*/
/*	b->next = NULL;*/
/*	//printf("New Node  %s %f %f\n", b->name, b->price, a->price);*/
/*	return b;*/
/*}*/

/*Trans copyTrans(Trans a){
	Trans b = (Trans)malloc(sizeof(struct Transaction));
	b->f_name = a->f_name;
	b->l_name = a->l_name;
	b->address = a->address;
	b->pay = a->pay;
	b->dd = a->dd;
	b->mm = a->mm;
	b->yy = a->yy;
	b->p = a->p;
	b->next = NULL;
	return b;
}*/

/*void printByPrice(DS productList){*/
/*	Prod product;*/
/*	Prod sortedList;*/
/*	if(productList->head == NULL){*/
/*		puts("No Products Available");*/
/*		return;*/
/*	}*/
/*	(void) pview(productList, 0); // set the view pointer to NULL*/
/*	sortedList = copyProd((Prod)productList->head->data);*/
/*	product =view_next(productList);*/
/*	while((product=(Prod)view_next(productList))!=NULL){*/
/*		Prod list = sortedList;*/
/*		Prod copy = copyProd(product);*/
/*		if(list->price>=product->price){*/
/*			copy->next = sortedList;*/
/*			sortedList = copy;*/
/*		}else{*/
/*			int inserted = 0;*/
/*			while(list->next!=NULL){*/
/*				//printf("\nNo  => %f: %f\n",product->price, list->next->price);*/
/*				if((list->next->price>product->price)){*/
/*					//printf("\nYes%f: %f\n",product->price, list->next->price);*/
/*					copy->next = list->next;*/
/*					list->next = copy;*/
/*					list = list->next;*/
/*					inserted = 1;*/
/*					break;*/
/*				}*/
/*				list = list->next;*/
/*			}*/
/*			if(inserted == 0){*/
/*				list->next = copy;*/
/*			}*/
/*		}*/

/*	}*/
/*	printProdList(sortedList);*/

/*}*/

/*void printByUnits(DS productList){*/
/*	Prod product;*/
/*	Prod sortedList;*/
/*	if(productList->head == NULL){*/
/*		puts("No Products Available");*/
/*		return;*/
/*	}*/
/*	(void) pview(productList, 0); // set the view pointer to NULL*/
/*	sortedList = copyProd((Prod)productList->head->data);*/
/*	product =view_next(productList);*/
/*	while((product=(Prod)view_next(productList))!=NULL){*/
/*		Prod list = sortedList;*/
/*		Prod copy = copyProd(product);*/
/*		if(list->num_unit>=product->num_unit){*/
/*			copy->next = sortedList;*/
/*			sortedList = copy;*/
/*		}else{*/
/*			int inserted = 0;*/
/*			while(list->next!=NULL){*/
/*				//printf("\nNo  => %f: %f\n",product->price, list->next->price);*/
/*				if((list->next->num_unit>product->num_unit)){*/
/*					//printf("\nYes%f: %f\n",product->price, list->next->price);*/
/*					copy->next = list->next;*/
/*					list->next = copy;*/
/*					list = list->next;*/
/*					inserted = 1;*/
/*					break;*/
/*				}*/
/*				list = list->next;*/
/*			}*/
/*			if(inserted == 0){*/
/*				list->next = copy;*/
/*			}*/
/*		}*/

/*	}*/
/*	printProdList(sortedList);*/

/*}*/



