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

#ifndef Global_LIST_H_INCLUDED
#define Global_LIST_H_INCLUDED

/********************************** INCLUDES **********************************/

#include <stdlib.h> // we use the EXIT_FAILURE macros everywhere
#include <stdio.h>
#include <string.h>

#include <data.h>   // data structure library
#include <input.h>  // input library

/**************************** TYPE DEFINITIONS ********************************/

struct Product{
	char*  ID;
	char* name;
	int num_unit;
	float price;
	struct Product * next;
};

struct Transaction{
	char* f_name;
	char* l_name;
	char* address;
	float pay;
	int dd, mm, yy;
	DS items;			// I CHANGED THIS
};

typedef struct Product* Prod;
typedef struct Transaction* Trans;


#define DEFAULT_XACTION_FILE "transaction.txt"
#define DEFAULT_PRODUCT_FILE "product.txt"
#define USERNAME             "owner"
#define PASSWORD             "password"


/******************************** PROTOTYPES **********************************/
// These are functions defined in one file and used in others so their
// prototypes have to be here

// In services.c
int          cmp_product(const void * left, const void * right);
const void * prod_key   (const void * product);

int  prompt            (void        );
void print_product_list(DS          );
void print_prod_heading(FILE*       );
void print_product     (FILE*, Prod );
void print_xaction     (FILE*, Trans);

// In owner.c
void owner_menu        (DS   , DS   );

// In customer.c
void customer_menu     (DS   , DS   );

#endif

