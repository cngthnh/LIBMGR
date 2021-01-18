#include "Library.h"
void listInit(List& list)
{
	list.head = NULL;
	list.tail = NULL;
}
int nodeCount(List list)
{
	int count = 0;
	for (Node* p = list.head; p; p = p->next)
	{
		count++;
	}
	return count;
}
Node* appendList(List& list, void* data)
{
	Node* newNode = new Node;
	if (newNode == NULL) return newNode;
	newNode->data = data;
	newNode->next = NULL;
	if (list.head == NULL)
	{
		list.head = newNode;
		list.tail = newNode;
		return newNode;
	}
	list.tail->next = newNode;
	list.tail = newNode;
	return newNode;
}
bool deleteAtIndex(List& list, int index)
{
	int count = 0;
	if (list.head == NULL) return false;
	if (index == 0 && list.head != NULL)
	{
		Node* tmp = list.head;
		list.head = list.head->next;
		delete tmp;
		return true;
	}
	for (Node* p = list.head; p; p = p->next)
	{
		if (count + 1 == index && p->next != NULL)
		{
			Node* tmp = p->next;
			p->next = p->next->next;
			delete tmp;
			return true;
		}
		if (count + 1 == index && p->next == NULL)
		{
			delete p->next;
			p->next = NULL;
			return true;
		}
		count++;
	}
	//Khong tim thay index thi khong xoa
	return false;
}
void deleteList(List& list)
{
	while (list.head != NULL)
	{
		deleteAtIndex(list, 0);
	}
}
Node* findDataAt(List& list, int index)
{
	int count = 0;
	for (Node* p = list.head; p; p = p->next)
	{
		if (index == count) return p;
		count++;
	}
	return NULL;
}
bool nameCmp(char* sub, char* base)
{
	//tolower
	char* str = new char[strlen(base) + 1];
	strcpy(str, base);
	for (int i = 0; i < strlen(str); i++) str[i] = tolower(str[i]);
	//split substring
	char* keyword = strtok(sub, SPECIAL_CHARS_FILTER);
	while (keyword != NULL)
	{
		// if 1 of all keywords cant be found => FALSE
		if (strstr(str, keyword) == NULL)
		{
			return 0;
			delete[] str;
		}
		keyword = strtok(NULL, SPECIAL_CHARS_FILTER);
	}
	delete[]str;
	return 1;
}