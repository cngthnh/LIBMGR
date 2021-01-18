#include "Stat.h"

void StatBook(List list) {
	long long int num = 0;
	system("CLS");
	writeProgName();
	setTextColor(YELLOW);
	wprintf(L"                                                      --- Thống kê số lượng sách trong thư viện ---\n\n");
	setTextColor(BLACK);
	for (Node* p = list.head; p; p = p->next)
	{
		num += ((BookInfo*)p->data)->bookNum;
	}
	wprintf(L"                                                           Số lượng sách trong thư viện: %lld", num);
	dustIgnore();
}
void StatReader(MemberList list) {
	system("CLS");
	writeProgName();
	setTextColor(YELLOW);
	wprintf(L"                                                           --- Thống kê số lượng độc giả ---\n\n");
	setTextColor(BLACK);
	wprintf(L"                                                                 Số lượng độc giả: %d", list.size);
	dustIgnore();
}
void StatReaderType(MemberList list) {
	int countMale = 0, countFemale = 0;
	for (Node* p = list.members.head; p; p = p->next) {
		if (((Member*)p->data)->gender == GENDER_FEMALE)
		{
			countFemale++;
		}
		else
		{
			countMale++;
		}
	}
	system("CLS");
	writeProgName();
	setTextColor(YELLOW);
	wprintf(L"                                                     --- Thống kê số lượng độc giả theo giới tính ---\n\n");
	setTextColor(BLACK);
	wprintf(L"\n                                                             Số lượng độc giả nam: %d\n", countMale);
	wprintf(L"\n                                                             Số lượng độc giả nữ: %d\n", countFemale);
	dustIgnore();
}
bool isExistType(List list, char* typeName) {
	bool ret = false; // chua co trong list the loai
	for (Node* p = list.head; p != NULL; p = p->next) {
		char* s = ((typeBook*)(p->data))->nameType;
		if (_stricmp(s, typeName) == 0)
		{
			ret = true;
			break;
		}
	}
	return ret;
}
void makeListType(List listBook, List& listType) {
	//thong ke tat ca cac type trong list
	for (Node* p = listBook.head; p; p = p->next) {
		if (!isExistType(listType, ((BookInfo*)(p->data))->typeBook)) {
			typeBook* s = new typeBook;
			s->num = 0;
			strcpy(s->nameType, ((BookInfo*)(p->data))->typeBook);
			appendList(listType, s);
		}
	}
}
void Classify(char* type, List& listType) {
	for (Node* p = listType.head; p; p = p->next) {
		char* s = ((typeBook*)p->data)->nameType;
		if (_strcmpi(type, s) == 0)
		{
			((typeBook*)p->data)->num++;
			s = NULL;
			break;
		}
	}
}
void countBookByType(List listBook, List& listType) {
	//duyet qua cac node book
	for (Node* p = listBook.head; p; p = p->next) {
		Classify(((BookInfo*)p->data)->typeBook, listType);
	}
} 
void writeDownTypes(List listType)
{
	system("CLS");
	writeProgName();
	setTextColor(YELLOW);
	wprintf(L"\n                                                        --- Thống kê số lượng sách theo thể loại ---\n\n");
	setTextColor(BLACK);
	if (listType.head == NULL)
	{
		setTextColor(RED);
		wprintf(L"\n                                                       Không có cuốn sách nào để thống kê theo thể loại");
		setTextColor(BLACK);
		wprintf(L"\n                                                                Nhấn enter để quay lại menu\n");
	}
	for (Node* p = listType.head; p; p = p->next)
	{
		wprintf(L"                                                      Thể loại: %S - Số lượng: %d\n", ((typeBook*)p->data)->nameType, ((typeBook*)p->data)->num);
	}
	dustIgnore();
}
int StatBookNotReturned() {
	int countNotReturned = 0;
	//doc file phieu muon sach 
	FILE* f = fopen(RECEIPT_FILE, "rb");
	if (f != NULL)
	{
		ReceiptRecord bf;
		//doc mot phieu (1 receipt record tu file) 
		// kiem tra phan tu cua phieu 'returned' => count 
		//get file size 
		fseek(f, 0, SEEK_END);
		long long size = ftell(f);
		size /= sizeof(ReceiptRecord);
		fseek(f, 0, SEEK_SET);
		for (int i = 0; i < size; i++)
		{
			fread(&bf, sizeof(ReceiptRecord), 1, f);
			if (bf.returned == false)
			{
				countNotReturned++;
			}
		}
	}
	else
	{
		wprintf(L"\nKhông thể mở file phiếu mượn trả sách!\n");
	}
	return countNotReturned;
}
void ShowBookNotReturned() {
	system("CLS");
	writeProgName();
	setTextColor(YELLOW);
	wprintf(L"                                                          --- Thống kê số sách đang mượn ---\n\n");
	int num = StatBookNotReturned();
	setTextColor(BLACK);
	wprintf(L"\n                                                         Số lượng sách đang được mượn: %d\n", num);
	dustIgnore();
}
void ShowLateReader(MemberList listMem) {
	system("CLS");
	writeProgName();
	setTextColor(YELLOW);
	wprintf(L"                                                          --- Thống kê độc giả bị trễ hạn ---\n\n");

	time_t now = time(NULL);
	tm gm = *gmtime(&now);
	//convert to UTC
	now = mktime(&gm);
	int numLate = 0;
	FILE* f = fopen(RECEIPT_FILE, "rb");
	List late;
	listInit(late);
	if (f != NULL)
	{
		ReceiptRecord bf;
		fseek(f, 0, SEEK_END);
		long long size = ftell(f);
		fseek(f, 0, SEEK_SET);
		size /= sizeof(ReceiptRecord);
		for (int i = 0; i < size; i++)
		{
			if (!fread(&bf, sizeof(ReceiptRecord), 1, f))
			{
				setTextColor(RED);
				wprintf(L"  Không thể đọc file phiếu mượn/trả");
				return;
			}
			time_t brw = mktime(&bf.time);
			int daysLate = difftime(now, brw);
			if (daysLate > BRW_PERIOD && !bf.returned)
			{
				caesarDecrypt(bf.idCard);
				for (Node* p = listMem.members.head; p; p = p->next)
				{
					char tmp[MAX_IDCARD_LEN];
					strcpy(tmp,((Member*)p->data)->idCard);
					caesarDecrypt(tmp);
					if (strcmp(bf.idCard, tmp) == 0)
					{
						bool exist = 0;
						for (Node* q = late.head; q; q = q->next)
						{
							char tmp1[MAX_IDCARD_LEN];
							strcpy(tmp1, ((Member*)q->data)->idCard);
							caesarDecrypt(tmp1);
							if (strcmp(tmp1, tmp) == 0)
							{
								exist = 1;
								break;
							}
						}
						if (!exist)
						{
							numLate++;
							appendList(late, p->data);
						}
						break;
					}
				}
			}
		}
	}
	else
	{
		wprintf(L"\n  Không thể mở file ghi phiếu!\n");
	}
	setTextColor(RED);
	wprintf(L"\n                                                        --------------------------------------");
	wprintf(L"\n                                                            Số lượng độc giả bị trễ hạn: %d\n", numLate);
	setTextColor(BLACK);
	for (Node* p = late.head; p; p = p->next)
	{
		printAMember((Member*)p->data);
	}
	dustIgnore();
}
void StatBasic(User user, List listBook, MemberList listMem) {
	List listType;
	listInit(listType);
	makeListType(listBook, listType);
	system("CLS");
	int choice = 0, key;
	while (1) {
		system("CLS");
		writeProgName();
		ShowConsoleCursor(false);
		setTextColor(RED);
		wprintf(L"                                                                  === THỐNG KÊ CƠ BẢN ===\n");
		setTextColor(YELLOW);
		wprintf(L"\n                                              Chọn chức năng bạn muốn thực hiện (sử dụng phím mũi tên lên/xuống)\n");
	bookMenu:
		gotoxy(0, 15);
		if (choice == 0) setTextColor(RED); else setTextColor(BLACK);
		wprintf(L"\n                                                                 <  Trở về menu chính  >\n");
		if (choice == 1) setTextColor(RED); else setTextColor(BLACK);
		wprintf(L"\n                                                          <  Thống kê số sách đang được mượn  >\n");
		if (choice == 2) setTextColor(RED); else setTextColor(BLACK);
		wprintf(L"\n                                                            <  Thống kê độc giả bị trễ hạn  >\n");
		setTextColor(BLACK);
		if (user.role == ADMIN_ROLE || user.role == MANAGER_ROLE)
		{
			if (choice == 3) setTextColor(RED); else setTextColor(BLACK);
			wprintf(L"\n                                                        <  Thống kê số lượng sách trong thư viện  >\n");
			if (choice == 4) setTextColor(RED); else setTextColor(BLACK);
			wprintf(L"\n                                                        <  Thống kê số lượng sách theo thể loại  >\n");
			if (choice == 5) setTextColor(RED); else setTextColor(BLACK);
			wprintf(L"\n                                                             <  Thống kê số lượng độc giả  >\n");
			if (choice == 6) setTextColor(RED); else setTextColor(BLACK);
			wprintf(L"\n                                                      <  Thống kê số lượng độc giả theo giới tính  >\n");
		}
		key = _getch();
		switch (key)
		{
		case KEY_UP:
			if (choice == MENU_MIN_OPTION && (user.role == ADMIN_ROLE || user.role == MANAGER_ROLE)) choice = STAT_MENU_MAX_OPTION;
			else if (choice == MENU_MIN_OPTION && user.role == SPECIALIST_ROLE) choice = STAT_MENU_MAX_SPECIALIST_OPTION; else choice--;
			goto bookMenu;
			break;
		case KEY_DOWN:
			if (choice == STAT_MENU_MAX_OPTION) choice = MENU_MIN_OPTION;
			else if (choice == STAT_MENU_MAX_SPECIALIST_OPTION && user.role == SPECIALIST_ROLE) choice = MENU_MIN_OPTION; else choice++;
			goto bookMenu;
			break;
		case KEY_ENTER:
			goto runFeatures;
			break;
		case ESC_KEY:
			return;
			break;
		default:
			goto bookMenu;
			break;
		}

	runFeatures:
		ShowConsoleCursor(true);

		switch (choice)
		{
		case 1:
			ShowBookNotReturned();
			break;
		case 2:
			ShowLateReader(listMem);
			break;
		case 3:
			StatBook(listBook);
			break;
		case 4:
		{
			countBookByType(listBook, listType);
			writeDownTypes(listType);
			break;
		}
		case 5:
		{
			StatReader(listMem);
			break;
		}
		case 6:
		{
			StatReaderType(listMem);
			break;
		}
		case 0:
			if (listType.head != NULL)
			{
				deleteList(listType);
			}
			return;
			break;
		}
	}
}