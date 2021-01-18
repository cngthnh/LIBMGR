#ifndef _PROJ_LIB_MNMT_BOOKS_
#define _PROJ_LIB_MNMT_BOOKS_
#include "..\Misc\Library.h"
#define BOOK_FILE "Data/books.csv"
#define BOOKS_MENU_MAX_OPTION 6
#define BOOKS_MENU_MAX_SPECIALIST_OPTION 2
#define NOT_FOUND 0
#define UPDATE_SUCCESS 1
#define NOT_OPEN_FILE -1
#define ESC_RETURN -2
typedef Node* (findString)(Node*, char*);
void ReadBookFile(List& list);
void ReadBookList(List list);
bool InputABook(BookInfo& b);
void AddABook(List& list);
void OutputABook(BookInfo b);
void BookManage(User user, List listBook);
Node* findBookName(Node* book, char bookName[]);
Node* findISBN(Node* book, char isbn[]);
void findBookBy(findString f, const const wchar_t fieldName[], List list, int size, const char* accepted);
void OutputAllABook(BookInfo b);
int updateBook(List& list);
int RemoveBook(List& list);
bool rewriteReceiptISBN(char* oldISBN, char* newISBN);
#endif
