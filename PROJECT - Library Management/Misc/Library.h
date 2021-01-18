#ifndef _PROJ_LIB_MNMT_
#define _PROJ_LIB_MNMT_
#define _CRT_SECURE_NO_WARNINGS
#define MAX_USR_LEN 51
#define MAX_PWD_LEN 51
#define MAX_NAME_LEN 51
#define MAX_EMAIL_LEN 51
#define MAX_ADD_LEN 101
#define S_ACTIVATED 1
#define S_BLOCKED 0
#define MAX_IDCARD_LEN 13
#define ADMIN_ROLE 0
#define MANAGER_ROLE 1
#define SPECIALIST_ROLE 2
#define USER_CREDENTIALS_FILE "Data/userCredentials.bin"
#define MEMBER_DATABASE_FILE "Data/memberInfo.bin"
#define RECEIPT_FILE "Data/receiptTrack.bin"
#define RED L"\x1b[38;2;223;21;62m"
#define CYAN L"\x1b[38;2;28;171;249m"
#define YELLOW L"\x1b[38;2;230;115;0m"
#define GREEN L"\x1b[38;2;16;188;132m"
#define PURPLE L"\x1b[38;2;124;14;184m"
#define BLACK L"\x1b[38;2;0;0;0m"
#define MAIN_MENU_MAX_OPTION 6
#define MENU_MIN_OPTION 0
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define ESC_KEY 27
#define GENDER_MALE 1
#define GENDER_FEMALE 0
#define ISBN_LEN 14
#define ACCEPTED_CHARS_PWD "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ~! @#$%^&*_-+=`|\(){}[]:;'<>,.?/"
#define ACCEPTED_CHARS_HANDLE "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_-"
#define ACCEPTED_CHARS_NAME "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ "
#define ACCEPTED_CHARS_ADDRESS "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ/_-:()., "
#define ACCEPTED_CHARS_EMAIL "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ.@+"
#define ACCEPTED_CHARS_NUMERIC "0123456789"
#define ACCEPTED_CHARS_NUMERIC_DOT "0123456789."
#define ACCEPTED_CHARS_REGULAR "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_- /?.;:'()#$!@&<>+="
#define SPECIAL_CHARS_FILTER "_- /?.;:'()#$!@&<>+="
#define KEY_ENTER '\r'
#define accessCredNode(index) findDataAt(credentials.users, index)
#define accessCredData(index, dtype) ((User*)findDataAt(credentials.users, index)->data)->dtype
#define accessMemNode(index) findDataAt(memList.members, index)
#define BOOK_NAME_SIZE 61
#define _TIME_ZONE 7
#define DAY_IN_SECOND 86400
#include <iostream>
#include <ctype.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <errno.h>
#include <direct.h>
#include <fcntl.h>
#include <io.h>
#include <Windows.h>
#include <time.h>
struct ReceiptRecord {
	char idCard[MAX_IDCARD_LEN];
	char ISBN[ISBN_LEN];
	tm time;
	bool returned;
};
struct BookInfo {
	char ISBN[ISBN_LEN];
	char bookName[BOOK_NAME_SIZE];
	char authorName[BOOK_NAME_SIZE];
	char NXB[BOOK_NAME_SIZE];
	int namXB;
	char typeBook[BOOK_NAME_SIZE];
	int price; // don vi tinh la vnd
	int bookNum;
};
struct Node {
	void* data;
	Node* next;
};
struct List {
	Node* head;
	Node* tail;
};
struct Date {
	unsigned int day;
	unsigned int month;
	unsigned int year;
};
struct User {
	int id;
	char handle[MAX_USR_LEN];
	long long hash;
	char fullName[MAX_NAME_LEN];
	char address[MAX_ADD_LEN];
	Date birthday;
	int gender;
	int role;
	int status;
	char idCard[MAX_IDCARD_LEN];
};
struct AuthFile {
	int size;
	List users;
};
struct MemberList {
	int size;
	List members;
};
struct Member {
	int id;
	char fullName[MAX_NAME_LEN];
	char address[MAX_ADD_LEN];
	Date birthday;
	int gender;
	char idCard[MAX_IDCARD_LEN];
	char email[MAX_EMAIL_LEN];
	Date issued;
	Date expired;
};

void writeProgName();
bool deleteAtIndex(List& list, int index);
void listInit(List& list);
void deleteList(List& list);
Node* appendList(List& list, void* data);
Node* findDataAt(List& list, int index);
int nodeCount(List list);
bool secureInp(char field[], int max, const wchar_t fieldName[], const char accepted[]);
bool securePassword(char field[], int max, const wchar_t fieldName[]);
void caesarEncrypt(char field[]);
void caesarDecrypt(char field[]);
Node* userIdCardCheck(List list, char idCard[]);
Node* memberIdCardCheck(List list, char idCard[]);
Node* handleCheck(List list, char handle[]);
void setTextColor(const wchar_t* color);
void ShowConsoleCursor(bool showFlag);
void consoleInit(bool quickEdit);
void drawExitIcon(int choice);
void drawFirstLineIcon(int choice); 
void drawSecondLineIcon(int choice);
void gotoxy(int x, int y);
void drawFavicon();
void drawLeftLogo();
bool nameCmp(char* sub, char* base);
void dustIgnore();
bool confBox();
#endif