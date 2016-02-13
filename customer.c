/* Ammon Dodson
	CES202
	Winter 2016
	The owner file
*/

#include "global.h"

void customer_menu(){
	int menu_option=0;
	
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
			sort_menu();
			break;
		case 2: // Search for Product by Name
		case 3: // Add Item to Cart
		case 4: // Checkout
		puts("Not Implemented");
		}
	} while (menu_option != 5);
}

void sort_menu(){
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
		case 2: // Sort by Quantity on Hand
		case 3: // Sort by Price
		puts("Not Implemented");
		}
	} while (menu_option != 4);
}
