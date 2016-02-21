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
#include "data.h"
#include<stdlib.h>
#include<stdio.h>
#include<time.h>
void sort_menu();
void updateTransactionList(DS, DS, float);
char * userName;

bool customer_login(){
	char* temp;

	//get username and address
	printf("username:");
	userName=grabline(stdin);

	//Currently allowing every user to login

	printf("password:");
	temp=grabword(stdin);
	free(temp);

	return true;
}

/*struct cart_{
	char * name;
	char * ID;
	float price;
	int num_unit;
	struct cart_ * next;
};

typedef struct cart_ * Cart;
DS initializeCart(){
	DS cartDS = new_DS('l');
}*/

DS cart;
cart=new_DS('l');

Cart createCartItem(char * ID, int num_unit, DS productList){
	Prod product;
	Cart item = (Cart)malloc(sizeof(struct cart_));
	int itemFound = 0;
	(void) pview(productList, 0); // set the view pointer to NULL
	while((product=view_next(productList))!=NULL){
		if(strcmp(product->ID,ID)==0){
			if(product->num_unit >= num_unit ){
				item->ID = ID;
				item->num_unit = num_unit;
				item->price = product->price;
				item->next = NULL;
				item->name = product->name;
				itemFound = 1;
				break;
			}else{
				printf("\nOnly %d items of type %s is left",product->num_unit, product->ID);
				free(item);
				return NULL;
			}
		}
	}

	if(itemFound == 0){
		printf("Sorry, We didn't find your item. Try again with correct ID");

		return NULL;

	}
	return item;
}

void freeCartItems(DS cart){
	while(pop(cart)!=NULL){
		//Popping out elems
	}
}
void updateProductList(DS productList, DS cartItems){
	(void) pview(productList, 0); // set the view pointer to NULL
	Prod product;
	while((product=view_next(productList))!=NULL){
		while(product!=NULL){
			if(strcmp(product->ID, item->ID)==0){
				product->num_unit -= item->num_unit;
				break;
			}
			list = list->right;
		}
	}

}

void printCart(DS cart, DS productList, DS transactionList){
	struct _node * item = cart->head;
	fprintf(stdout,
			"    ID               NAME             NUM_ITEMS  UNIT PRICE\n"
	);
	float price = 0;
	while((item)!=NULL){
		Cart item_cart = item->data;
		fprintf(stdout,
			"%8s   %24s   %7d    $ %6.2f\n",
			item_cart->ID,
			item_cart->name,
			item_cart->num_unit,
			item_cart->price*item_cart->num_unit
		);
		price += item_cart->price*item_cart->num_unit;
		item = item->right;
	}
	puts("Are you sure you want to buy the Products in the cart (y/n)?");
	if(prompt()!='y'){
		puts("Cancelling....");
	}else{

		updateProductList(productList, cart);
		updateTransactionList(cart, transactionList, price);
		freeCartItems(cart);
	}
}

void updateTransactionList(DS cart, DS transList,float price){
	struct _node * item = cart->head;

	Trans trans = (Trans)malloc(sizeof(struct Transaction));

	printf("First Name");
	trans->f_name = grabword(stdin);
	printf("Last Name");
	trans->l_name = grabword(stdin);
	printf("Address");
	trans->address = grabword(stdin);
	trans->pay = price;
	time_t time_raw_format;
    struct tm * ptr_time;
    char buffer[50];

    time ( &time_raw_format );
    ptr_time = localtime ( &time_raw_format );
    strftime(buffer,50,"%d",ptr_time);
	trans->dd = atoi(buffer);
	buffer[0] = '\0';
	strftime(buffer,50,"%m",ptr_time);
	trans->mm = atoi(buffer);
	buffer[0] = '\0';
	strftime(buffer,50,"%Y",ptr_time);
	trans->yy = atoi(buffer);
	push(transList, trans);

}

DS addItemToCart(DS cart, DS productList){
	Cart item;
	puts("\nEnter ID of the product");
	char * ID = (char *)grabword(stdin);
	(void) pview(cart, 0); // set the view pointer to NULL
	puts("\nEnter Number of Units");
	int num_unit = atoi((char *)grabword(stdin));
	item = createCartItem(ID, num_unit, productList);
	if(item != NULL){
		push(cart, item);
	}
	return cart;
}

/************************************************************/
/*						The Customer Menu					*/
/************************************************************/
//USES: ShopSys.c:	prompt()

void printUnSortedList(DS productList){
	/*
		1-> Unsorted
		2->sortByQuantity
		3->sortByPrice
	*/
	Prod product;
	DS list;
	DS sortedList = new_DS('l');

	//printf("Printing");
	print_prod_heading(stdout);
	sortedList->head = productList->head;
	(void) pview(productList, 0); // set the view pointer to NULL

	while((product=view_next(productList))!=NULL){
		print_product(stdout, product);
	}

}

void printProdList(Prod sortedList){
	print_prod_heading(stdout);
	Prod list = sortedList;
	while(list!=NULL){
		print_product(stdout, list);
		list = list->next;
	}
}

Prod copyProd(Prod a){
	Prod b = (Prod)malloc(sizeof(struct Product));
	b->ID = a->ID;
	b->name = a->name;
	b->price = a->price;
	b->num_unit = a->num_unit;
	b->next = NULL;
	//printf("New Node  %s %f %f\n", b->name, b->price, a->price);
	return b;
}

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

void searchByName(DS prodList){
	puts("Enter Name of the desired product");
	char * name = grabword(stdin);
	print_prod_heading(stdout);
	if(prodList->head == NULL){
		puts("Sorry, No Product Found");
		return;
	}
	struct _node * list = prodList->head;
	int found = 0;
	while(list!=NULL){
		Prod elem = (Prod)list->data;
		//printf("%s and %s", elem->name, name);
		if(strcmp(name, elem->name) == 0){
			found = 1;
			print_product(stdout, list->data);
		}
		list = list->right;
	}
	if(found == 0){
		puts("Sorry, No Product Found");
		return;
	}

}


void customer_menu(DS prod_list, DS trans_list){
	int menu_option=0;
	DS cart = initializeCart();

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
			sort_menu(prod_list);
			break;
		case 2: // Search for Product by Name
			searchByName(prod_list);
			break;
		case 3: // Add Item to Cart
			cart = addItemToCart(cart, prod_list);
			break;
		case 4: // Checkout
			printCart(cart, prod_list, trans_list);

		}
	} while (menu_option != 5);
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
			printUnSortedList(prod_list);
			break;
		case 2: // Sort by Quantity on Hand
			printByUnits(prod_list);
			break;
		case 3: // Sort by Price
			printByPrice(prod_list);
			break;

		}
	} while (menu_option != 4);
}


