#include "Auth\Auth.h"
#include "Member\Member.h"
#include "Books\Books.h"
#include "Receipt\Receipt.h"
#include "Stat\Stat.h"
void main()
{
	consoleInit(0);
	User thisSessionUser;
	thisSessionUser.id = -1;
	AuthFile credentials;
	listInit(credentials.users);
	getCredentials(credentials);
	MemberList memberList;
	listInit(memberList.members);
	getMemberList(memberList);
	List listBook;
	listInit(listBook);
	ReadBookFile(listBook);
	memberList.size = nodeCount(memberList.members);
	int choice = 1, key;
	while (1)
	{
		if (thisSessionUser.id == -1)
			signIn(credentials, thisSessionUser);
		else
		{
			ShowConsoleCursor(false);
			system("CLS");
			writeProgName();
			setTextColor(RED);
			wprintf(L"                                                                  Xin chào, %S\n", thisSessionUser.fullName);
			setTextColor(YELLOW);
			wprintf(L"                                            Chọn chức năng bạn muốn thực hiện (sử dụng phím mũi tên trái/phải)\n");	
			mainMenu:
			if (choice > 0 && choice < 4) drawFirstLineIcon(choice); else drawFirstLineIcon(-1);
			if (choice > 3 && choice < 7) drawSecondLineIcon(choice); else drawSecondLineIcon(-1);
			if (choice == 0) drawExitIcon(choice); else drawExitIcon(-1);
			setTextColor(BLACK);
			key = _getch();
			switch (key)
			{
				case KEY_LEFT:
					if (choice == MENU_MIN_OPTION) choice = MAIN_MENU_MAX_OPTION; else choice--;
					goto mainMenu;
					break;
				case KEY_RIGHT:
					if (choice == MAIN_MENU_MAX_OPTION) choice = MENU_MIN_OPTION; else choice++;
					goto mainMenu;
					break;
				case KEY_ENTER:
					goto runFeatures;
					break;
				default:
					goto mainMenu;
					break;
			}

		runFeatures:
			ShowConsoleCursor(true);
			switch (choice)
			{
			case 1:
				accountManage(credentials, thisSessionUser);
				break;
			case 2:
				memberManage(memberList, thisSessionUser);
				break;
			case 3:
				BookManage(thisSessionUser, listBook);
				break;
			case 4:
				bookBorrow(listBook, memberList);
				break;
			case 5:
				bookReturn(listBook, memberList);
				break;
			case 6:
				StatBasic(thisSessionUser, listBook, memberList);
				break;
			case 0:
				if (credentials.users.head != NULL)
				{
					deleteList(credentials.users);
				}
				if (memberList.members.head != NULL)
				{
					deleteList(memberList.members);
				}
				if (listBook.head != NULL)
				{
					deleteList(listBook);
				}
				consoleInit(1);
				exit(0);
				break;
			}
		}
	}
}