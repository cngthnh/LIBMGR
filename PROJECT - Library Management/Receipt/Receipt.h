#ifndef _PROJ_LIB_MNMT_RECEIPT_
#define _PROJ_LIB_MNMT_RECEIPT_
#define BRW_PERIOD 604800
#define FINE_PER_DAY 5
#include "..\Misc\Library.h"
void bookBorrow(List& bookList, MemberList memberList);
void bookReturn(List& bookList, MemberList memberList);
void updateBookQty(List& bookList);
#endif