#define _CRT_SECURE_NO_WARNINGS
#include "ItemList.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define N 5
const int ITEMS = 1;
const int CURRS =  2;

const char* INSTRUCTIONS = "Instructions.txt";
const char* INVENTORY = "Inventory.txt";
const char* CURRENCIES = "Currencies.txt";
const char* RESULTS = "Results.txt";
char DELIM[] = "###";
char START[] = "start";
char PURCHASE[] = "purchase";
char RECALL[] = "recall";
char PRICE[] = "price";
char END[] = "end";



/*a function to read the line/string and save the different instructions in an array
receives a pointer to the line and returns a pointer to the arry of the instructions*/
void parseLine(char*, char* []);

ItemList* start();//making new list and filling it with items , returns a  pointer to this list

/*a function to buy the item' it receives the item list and the item we want to buy
and the quantity.*/
void purchase(ItemList*, char[], int);

/*a function to recall an item*/
void recall(ItemList*, char[], int);

ItemList* readListFromFile(const char*, int);

/*a function to change the currency, receives the list of items and the list of prices, and the name
of the item and the currency. returns the price in new currency*/
double price(ItemList*, ItemList*, char[], char[]);

/*writing the details of the list into the file
receives a pointer to the list and the name of the file*/
void writeToFile(ItemList*, const char*);

int main() {
	FILE* inst = fopen(INSTRUCTIONS, "r");//opening the file and having a pointer to it
	char line[256];//an array to hold the line
	char* command[N];
	ItemList* items = NULL;//a pointer to the list containing the items
	ItemList* currencies = NULL;
	


	while (fgets(line, sizeof(line), inst)) {//reading a whole line from the folder and inserting it into the array
		if (line[strlen(line) - 1] == '\n') 
			line[strlen(line) - 1] = '\0';//a strip to cancel the enters
		
		parseLine(line, command);//the function will fill the command array with the relevant strings/instructions
		

		/*if the command in the beginning of the sentence is "START" we are starting a list
		of items, we are making a list in which every element contains the details of one item*/
		if (!strcmp(command[0], START)) {
			printf("Initializing the items\n");
			//items = start();//reading the content in the list	
			/*read the file inventory, and take the and put in the linked lidt items
			the items from the file*/
			items = readListFromFile(INVENTORY, ITEMS);
			currencies = readListFromFile(CURRENCIES, CURRS);

		}
		/*if the command is "PURCHASE" we are buying the item*/
		else if (!strcmp(command[0], PURCHASE)) {
			printf("purchasing %s\n", command[1]);
			purchase(items, command[1], atoi(command[2]));
		}
		else if (!strcmp(command[0], RECALL)) {
			printf("Recalling %s\n", command[1]);
			recall(items, command[1], atoi(command[2]));
		}
		else if (!strcmp(command[0], PRICE)) {
			double p = price(items, currencies, command[1], command[2]);

			if (p != -1) {
				printf("price of %s in %s is %lf\n", command[1], command[2], p);
			}
		}
		else if (!strcmp(command[0], END)) {
			printf("wrting data to file\n");
			writeToFile(items, RESULTS);
			deleteList(items);
			deleteList(currencies);
		}
	}
	
}

void parseLine(char* line, char* command[]) {
	int i = 0;
	char* sub = strtok(line, DELIM);

	while (sub != NULL) {
		command[i] = sub;
		sub = strtok(NULL, DELIM);
		i++;
	}
}

ItemList* start() {
	FILE* inv = fopen(INVENTORY, "r");
	char line[256];
	//an array of pointers to line parts, first element is the name next element is the price and e
	//last element is the quantity
	char* item[N];
	ItemList* list = createList();
	char* c;
	
	while (fgets(line, sizeof(line), inv)) {//reading a whole line from the folder and inserting it into the array
		line[strlen(line) - 1] = '\0';//a strip to cancel the enters
		parseLine(line, item);//the function will fill the items array with the relevant strings
		//printf("%s\n", item[0]);

		addToList(list, item[0], strtod(item[1], &c),atoi(item[2]));

	}
	fclose(inv);
	return list;
}

void purchase(ItemList* list, char name[], int quantity) {
	ItemNode* element = findItem(list, name);

	if (element == NULL) {
		return;
	}
	if (quantity < element->quantity) {
		element->quantity -= quantity;//redicung the wanted quantity from the exsisting quantity
	}
	else {
		removeItem(list, name);//if there is more than we need to but we are removing it from the list
	}
}

void recall(ItemList* list, char name[], int quantity) {
	ItemNode* element = findItem(list, name);

	if (element != NULL) {
		element->quantity += quantity;
	}
}

ItemList* readListFromFile(const char* fileName, int type) {
	FILE* f = fopen(fileName, "r");
	char line[256];
	//an array of pointers to line parts, first element is the name next element is the price and e
	//last element is the quantity
	char* element[N];
	ItemList* list = createList();
	char* c;

	while (fgets(line, sizeof(line), f)) {//reading a whole line from the folder and inserting it into the array
		line[strlen(line) - 1] = '\0';//a strip to cancel the enters
		
		parseLine(line, element);//the function will fill the items array with the relevant strings
		

		if (type == ITEMS) {
			addToList(list, element[0], strtod(element[1], &c), atoi(element[2]));
		}
		else if (type == CURRS) {
			addToList(list, element[0], strtod(element[1], &c), 0);
		}
	}
	fclose(f);
	return list;

}

double price(ItemList* items, ItemList* currencies, char name[], char coin[]) {
	ItemNode* element = findItem(items, name);//finding the item in the list
	ItemNode* curr = findItem(currencies, coin);//finding the currency we need to convert to

	if (element == NULL || curr == NULL) {
		return -1;
	}
	double p = element->price * curr->price;
	return p;

}
void writeToFile(ItemList* list, const char* fileName) {
	FILE* f = fopen(fileName, "w");
	ItemNode* p = NULL;

	for (p = list->head; p != NULL; p = p->next) {
		fprintf(f, "%s", p->name);
		fprintf(f, "%s", DELIM);
		fprintf(f, "%lf", p->price);
		fprintf(f, "%s", DELIM);
		fprintf(f, "%d", p->quantity);
		fprintf(f,"\n");


	}


}