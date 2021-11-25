#define _CRT_SECURE_NO_WARNINGS

#include "ItemList.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


ItemList* createList() {
	ItemList* list = (ItemList*)malloc(sizeof(ItemList));
	list->head = NULL;
	return list;
}

void addToList(ItemList* list, char name[], double price, int quantity) {
	ItemNode* x = (ItemNode*)malloc(sizeof(ItemNode));//creating the new item (node)
	strcpy(x->name, name);
	x->price = price;
	x->quantity = quantity;

	ItemNode* curr = list->head;
	ItemNode* prev  = NULL;

	while (curr != NULL && strcmp(name, curr->name) > 0) {
		prev = curr;
		curr = curr->next;

	}
	x->next = curr;
	if (prev == NULL) {
		list->head = x;
	}
	else
		prev->next = x;
}

void printItem(ItemNode* item) {
	printf("[%s,%lf,%d]", item->name, item->price, item->quantity);

}
void printList(ItemList* list) {
	printf("head-->");
	ItemNode* p;
	for (p = list->head; p != NULL; p = p->next) {
		printItem(p);
		printf("-->");
}

	printf("NULL");

}

void removeItem(ItemList* list, char name[]) {

	ItemNode* curr = list->head;
	ItemNode* prev = NULL;

	while (curr != NULL && strcmp(name, curr->name)) {
		prev = curr;
		curr = curr->next;
	}
	if (curr != NULL) {
		if (prev == NULL) {
			list->head = curr->next;
		}
		else
			prev->next = curr->next;
		free(curr);
	}

}

ItemNode* findItem(ItemList* list, char name[]) {
	ItemNode* curr;
	for (curr = list->head; curr != NULL; curr = curr->next) {
		if (!strcmp(name, curr->name)) {//if they are same it returns 0 which is "false"
			return curr;
		}
	}
	return NULL;

}

void deleteList(ItemList* list) {
	ItemNode* p = list->head;

	while (list->head != NULL) {
		list->head = list->head->next;
		free(p);
		p = list->head;

	}

}

