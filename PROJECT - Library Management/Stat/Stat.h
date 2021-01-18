#ifndef _PROJ_LIB_MNMT_STAT_
#define _PROJ_LIB_MNMT_STAT_
#define STAT_MENU_MAX_OPTION 6
#define STAT_MENU_MAX_SPECIALIST_OPTION 2
#include "..\Misc\Library.h"
#include "..\Receipt\Receipt.h"
#include "..\Member\Member.h"
struct typeBook {
	int num;
	char nameType[BOOK_NAME_SIZE];
};

void StatBook(List listBook);
void StatReader(MemberList list);
void StatReaderType(MemberList list);
int StatBookNotReturned();
bool isExistType(List listBook, char* typeName);
void Classify(char* type, List& listType);
void countBookByType(List listBook, List& listType);
void makeListType(List listBook, List& listType);
void StatBasic(User user, List listBook, MemberList listMem);
void ShowBookNotReturned();
void ShowLateReader(MemberList listMem);
void writeDownTypes(List listType);
#endif
