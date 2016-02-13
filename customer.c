/* Ammon Dodson
	CES202
	Winter 2016
	The owner file
*/

#include "global.h"

int customer_menu(){
	int i =-1;
	
	while (i<1 || i>3) {
		puts("\t        CUSTOMER MENU");
		puts("\t        =============");
		puts("\t1. Display Products");
		puts("\t2. Search for Product by Name");
		puts("\t3. Add Item to Cart");
		puts("\t4. Checkout");
		puts("\t5. Logout and Return to Main Menu");
		
		i=prompt();
	}
	return i;
}

int sort_menu(){
	int i =-1;
	
	while (i<1 || i>3) {
		puts("\t       SORT OPTIONS");
		puts("\t       ============");
		puts("\t1. Unsorted");
		puts("\t2. Sort by Quantity on Hand");
		puts("\t3. Sort by Price");
		puts("\t4. Back");
		
		i=prompt();
	}
	return i;
}
