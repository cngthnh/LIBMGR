#ifndef _PROJ_LIB_MNMT_MEMBER_
#define _PROJ_LIB_MNMT_MEMBER_
#define MEMBER_MENU_MAX_SPECIALIST_OPTION 6
#define MEMBER_MENU_MAX_OPTION 7
#include "..\Misc\Library.h"
void viewMembers(MemberList& list);
void memberManage(MemberList& list, User user);
void getMemberList(MemberList& memList);
void addMember(MemberList& memList);
bool memberInfoForm(MemberList memList, Member* newMember, int id, bool isUpdate);
void printAMember(Member* member);
void searchMemberByName(MemberList memList);
void searchMemberByIDCard(MemberList memList);
void updateMember(MemberList memList);
void cardExt(MemberList memList);
bool rewriteReceiptMemID(char* oldId, char* newId);
#endif