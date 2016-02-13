#include "global.h"

void print_prod_heading(FILE* file){
	fprintf(file,
			"    ID               NAME             ON-HAND  UNIT PRICE\n"
	);
}

/**	product.txt has a header in the first line followed by one product record
 *	per line. Each product record has 4 tab-delimited fields.
 *	PRODUCT ID	PRODUCT NAME	QUANTITY ON HAND	PRICE
*/
void print_product(FILE* file, Prod p){
	fprintf(file,
			"%8s   %24s   %7d    $ %6.2f\n",
			p->ID,
			p->name,
			p->num_unit,
			p->price
	);
}

void print_xaction_heading(FILE* file){
	fprintf(file,
			"FIRST NAME  LAST NAME AMOUNT  ADDRESS   \n"
	);
}


/**	transaction.txt has a header in the first line followed by one transaction
 *	record per line.
 *	each transaction record has 5 tab-delimited fields:
 *	First Name	last Name	Ammount of payment	date of payment	list of
 *	purchased products
*/
void print_xaction(FILE* file, Trans t){
	fprintf(file,
			"%10s %10s  $ %6.2f %s  %4d.%2d.%2d \n",
			t->f_name,
			t->l_name,
			t->pay,
			t->address,
			t->yy,
			t->mm,
			t->dd
	);
}
