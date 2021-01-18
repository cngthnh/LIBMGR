#include "Books.h"

bool InputABook(BookInfo& a) {
	system("CLS");
	writeProgName();
	setTextColor(YELLOW);
	wprintf(L"                                                              --- Nhập thông tin sách ---\n");
	setTextColor(BLACK);
	char temp[10];
	if (!secureInp(a.ISBN, ISBN_LEN, L"  Mã ISBN (13 kí tự): ", ACCEPTED_CHARS_NUMERIC)) return false;
	if (!secureInp(a.bookName, BOOK_NAME_SIZE, L"  Tên sách: ", ACCEPTED_CHARS_REGULAR)) return false;
	if (!secureInp(a.authorName, BOOK_NAME_SIZE, L"  Tác giả: ", ACCEPTED_CHARS_REGULAR)) return false;
	if (!secureInp(a.NXB, BOOK_NAME_SIZE, L"  Nhà xuất bản: ", ACCEPTED_CHARS_REGULAR)) return false;
	if (!secureInp(temp, 5, L"  Năm xuất bản: ", ACCEPTED_CHARS_NUMERIC)) return false;
	a.namXB = atoi(temp);
	if (!secureInp(a.typeBook, BOOK_NAME_SIZE, L"  Thể loại: ", ACCEPTED_CHARS_REGULAR)) return false;
	if (!secureInp(temp, 10, L"  Giá sách: ", ACCEPTED_CHARS_NUMERIC)) return false;
	a.price = atoi(temp);
	if (!secureInp(temp, 5, L"  Số lượng sách: ", ACCEPTED_CHARS_NUMERIC)) return false;
	a.bookNum = atoi(temp);
	return true;
}
void OutputABook(BookInfo b) {
	wprintf(L"  ISBN: %S - Tên sách: %S - Tác giả: %S - Năm xuất bản: %d\n", b.ISBN, b.bookName, b.authorName, b.namXB);
}
void ReadBookFile(List& list) {
	FILE* f = fopen(BOOK_FILE, "r+");
	if (f == NULL)
	{
		if (_mkdir("Data") == 0) wprintf(L"Đã tạo thư mục Data");
		if (fopen(BOOK_FILE, "wb") == NULL)
		{
			setTextColor(RED);
			wprintf(L"Không thể mở file dữ liệu sách\n");
			setTextColor(BLACK);
			wprintf(L"Nhấn enter để quay lại menu");
			dustIgnore();
			return;
		}
		f = fopen(BOOK_FILE, "r+");
		fputs("ISBN,Ten sach,Tac gia,Nha xuat ban,Nam xuat ban,The loai,Gia sach,So sach\n", f);
		fclose(f);
		f = fopen(BOOK_FILE, "r+");
	}
	fseek(f, 0, SEEK_END);
	long long size = ftell(f);
	fseek(f, 0, SEEK_SET);
	if (size == 0)
	{
		fputs("ISBN,Ten sach,Tac gia,Nha xuat ban,Nam xuat ban,The loai,Gia sach,So sach\n", f);
		fclose(f);
		return;
	}
	else
	{
		char firstLine[100];
		fgets(firstLine, 100, f);
	}
	if (list.head != NULL)
	{
		deleteList(list);
		listInit(list);
	}
	while (1)
	{
		BookInfo* newBook = new BookInfo;
		int readArg = fscanf(f, "%[^']',%[^,],%[^,],%[^,],%d,%[^,],%d,%d\n", newBook->ISBN, newBook->bookName, newBook->authorName, newBook->NXB, &newBook->namXB, newBook->typeBook, &newBook->price, &newBook->bookNum);
		if (readArg == EOF) break;
		if (readArg != 8) // 8 trường thông tin
		{
			setTextColor(RED);
			wprintf(L"File sách sai định dạng hoặc không thể đọc\n");
			setTextColor(BLACK);
			dustIgnore();
			fclose(f);
			return;
		}
		appendList(list, newBook);
	}
	fclose(f);
}
void ReadBookList(List list) {
	system("CLS");
	writeProgName();
	setTextColor(YELLOW);
	wprintf(L"                                                        --- Danh sách các sách trong thư viện ---\n\n");
	setTextColor(BLACK);
	for (Node* p = list.head; p != NULL; p = p->next) {
		BookInfo temp = *(BookInfo*)(p->data);
		OutputABook(temp);
	}
	dustIgnore();
}
void AddABook(List& list)
{
	BookInfo* newBook = new BookInfo;
	if (newBook == NULL)
	{
		setTextColor(RED);
		wprintf(L"Không đủ dung lượng bộ nhớ để thêm sách\n");
		setTextColor(BLACK);
		dustIgnore();
		return;
	}
	if (!InputABook(*newBook)) return;
	if (!confBox()) return;
	FILE* f = fopen(BOOK_FILE, "a");
	if (f == NULL)
	{
		if (_mkdir("Data") == 0) wprintf(L"Đã tạo thư mục Data");
		if (fopen(BOOK_FILE, "wb") == NULL)
		{
			setTextColor(RED);
			wprintf(L"Không thể mở file dữ liệu sách\n");
			setTextColor(BLACK);
			wprintf(L"Nhấn enter để quay lại menu");
			dustIgnore();
			return;
		}
		f = fopen(BOOK_FILE, "a");
	}
	if (fprintf(f, "%s',%s,%s,%s,%d,%s,%d,%d\n", newBook->ISBN, newBook->bookName, newBook->authorName, newBook->NXB, newBook->namXB, newBook->typeBook, newBook->price, newBook->bookNum) == 0)
	{
		setTextColor(RED);
		wprintf(L"Không thể cập nhật file thông tin sách\n");
		fclose(f);
		dustIgnore();
		return;
	}
	else
	{
		setTextColor(GREEN);
		if (appendList(list, newBook) == NULL)
		{
			setTextColor(RED);
			wprintf(L"Không đủ dung lượng để lưu sách\n");
			fclose(f);
			dustIgnore();
			return;
		}
		wprintf(L"Thêm sách thành công!\n");
		fclose(f);
		dustIgnore();
		return;
	}
	fclose(f);
}
Node* findBookName(Node* book, char bookName[]) {
	if (nameCmp(bookName, ((BookInfo*)book->data)->bookName))
	{
		return book;
	}
	return NULL;
}

Node* findISBN(Node* book, char isbn[]) {
	if (_strcmpi(isbn, ((BookInfo*)book->data)->ISBN) == 0)
	{
		return book;
	}
	return NULL;
}
void OutputAllABook(BookInfo b) 
{
	wprintf(L"  --------------------------------------------------\n");
	wprintf(L"  ISBN: %S\n", b.ISBN);
	wprintf(L"  Tên sách: %S\n", b.bookName);
	wprintf(L"  Tên tác giả: %S\n", b.authorName);
	wprintf(L"  Nhà xuất bản: %S\n", b.NXB);
	wprintf(L"  Năm xuất bản: %d\n", b.namXB);
	wprintf(L"  Thể loai: %S\n", b.typeBook);
	wprintf(L"  Giá tiền: %d\n", b.price);
	wprintf(L"  Số lượng sách: %d\n", b.bookNum);	
}
bool rewriteReceiptISBN(char* oldISBN, char* newISBN)
{
	ReceiptRecord tmp;
	FILE* f = fopen(RECEIPT_FILE, "r+b");
	if (f == NULL) return true;
	fseek(f, 0, SEEK_END);
	long long int size = ftell(f);
	fseek(f, 0, SEEK_SET);
	size /= sizeof(ReceiptRecord);
	for (int i = 0; i < size; i++)
	{
		if (!fread(&tmp, sizeof(ReceiptRecord), 1, f))
		{
			setTextColor(RED);
			wprintf(L"Không thể cập nhật file phiếu mượn sách\n");
			fclose(f);
			dustIgnore();
			return 0;
		}
		if (strcmp(tmp.ISBN, oldISBN) == 0)
		{
			strcpy(tmp.ISBN, newISBN);
			fseek(f, 0 - sizeof(ReceiptRecord), SEEK_CUR);
			if (!fwrite(&tmp, sizeof(ReceiptRecord), 1, f))
			{
				setTextColor(RED);
				wprintf(L"Không thể cập nhật file phiếu mượn sách\n");
				fclose(f);
				dustIgnore();
				return 0;
			}
		}
	}
	return 1;
	fclose(f);
}
int updateBook(List& list) {
	int ret;
	system("CLS");
	writeProgName();
	setTextColor(YELLOW);
	wprintf(L"                                                        --- Chỉnh sửa thông tin một quyển sách ---\n\n");
	char ISBN[ISBN_LEN];
	if (!secureInp(ISBN, ISBN_LEN, L"  ISBN cần chỉnh sửa: ", ACCEPTED_CHARS_NUMERIC)) {
		return ESC_RETURN;
	}
	setTextColor(BLACK);
	Node* pBook = NULL;
	BookInfo tmpBook;
	for (Node* p = list.head; p; p = p->next)
	{
		if (_strcmpi(ISBN, ((BookInfo*)p->data)->ISBN) == 0)
		{
			pBook = p;
			tmpBook = *(BookInfo*)pBook->data;
			break;
		}
	}
	if (pBook == NULL) {
		setTextColor(RED);
		wprintf(L"Sách không tồn tại trong thư viện!\n ");
		dustIgnore();
		ret = NOT_FOUND;
	}
	else
	{
		char temp[10];
		setTextColor(GREEN);
		wprintf(L"  Thông tin về sách bạn tìm:\n");
		setTextColor(BLACK);
		OutputAllABook(tmpBook);
		wprintf(L"  --------------------------------------------------\n");
		setTextColor(YELLOW);
		wprintf(L"  Nhập thông tin mới về quyển sách:\n");
		setTextColor(BLACK);
		if (!secureInp(tmpBook.ISBN, ISBN_LEN, L"  Mã ISBN: ", ACCEPTED_CHARS_REGULAR)) {
			return ESC_RETURN;
		}
		if (!secureInp(tmpBook.bookName, BOOK_NAME_SIZE, L"  Tên sách: ", ACCEPTED_CHARS_REGULAR))
		{
			return ESC_RETURN;
		}
		if (!secureInp(tmpBook.authorName, BOOK_NAME_SIZE, L"  Tên tác giả: ", ACCEPTED_CHARS_REGULAR))
		{
			return ESC_RETURN;
		}
		if (!secureInp(tmpBook.NXB, BOOK_NAME_SIZE, L"  Nhà xuất bản: ", ACCEPTED_CHARS_REGULAR))
		{
			return ESC_RETURN;
		}
		if (!secureInp(temp, 5, L"  Năm xuất bản: ", ACCEPTED_CHARS_NUMERIC))
		{
			return ESC_RETURN;
		}
		tmpBook.namXB = atoi(temp);
		if (!secureInp(tmpBook.typeBook, BOOK_NAME_SIZE, L"  Thể loại: ", ACCEPTED_CHARS_REGULAR))
		{
			return ESC_RETURN;
		}
		if (!secureInp(temp, 10, L"  Giá tiền: ", ACCEPTED_CHARS_NUMERIC))
		{
			return ESC_RETURN;
		}
		tmpBook.price = atoi(temp);
		if (!secureInp(temp, 10, L"  Số lượng sách: ", ACCEPTED_CHARS_NUMERIC))
		{
			return ESC_RETURN;
		}
		tmpBook.bookNum = atoi(temp);
		if (!confBox()) return ESC_RETURN;
		if (!rewriteReceiptISBN(((BookInfo*)pBook->data)->ISBN, tmpBook.ISBN)) return NOT_OPEN_FILE;
		*((BookInfo*)pBook->data) = tmpBook;
		//ghi lai toan bo file
		FILE* f = fopen(BOOK_FILE, "wt");
		if (f != NULL) {
			fputs("ISBN,Ten sach,Tac gia,Nha xuat ban,Nam xuat ban,The loai,Gia sach,So sach\n", f);
			BookInfo a;
			for (Node* p = list.head; p != NULL; p = p->next) {
				a = *(BookInfo*)(p->data);
				fprintf(f, "%s',%s,%s,%s,%d,%s,%d,%d\n", a.ISBN, a.bookName, a.authorName, a.NXB, a.namXB, a.typeBook, a.price, a.bookNum);
			}
			ret = UPDATE_SUCCESS;
			fclose(f);
		}
		else
		{
			ret = NOT_OPEN_FILE;
		}
		pBook = NULL;
	}
	return ret;
}
void findBookBy(findString f, const wchar_t fieldName[], List list, int size, const char* accepted) {
	char* temp = new char[size];
	system("CLS");
	writeProgName();
	setTextColor(YELLOW);
	wprintf(L"                                                        --- Tìm kiếm sách theo %s ---\n", fieldName);
	if (!secureInp(temp, size, fieldName, accepted)) return;
	setTextColor(BLACK);
	Node* x;
	int n = 0;
	//to lower
	for (int i = 0; i < size; i++) temp[i] = tolower(temp[i]);
	for (Node* p = list.head; p != NULL; p = p->next) 
	{
		x = f(p, temp);
		if (x != NULL)
		{
			n++;
			OutputAllABook(*((BookInfo*)x->data));
		}
	}
	if (n != 0)
	{
		wprintf(L"  --------------------------------------------------\n");
		setTextColor(GREEN);
		wprintf(L"  Đã tìm thấy %d kết quả phù hợp\n", n);
	}
	else
	{
		wprintf(L"  --------------------------------------------------\n");
		setTextColor(RED);
		wprintf(L"  Không tìm thấy kết quả nào phù hợp\n");
	}
	dustIgnore();
}
int RemoveBook(List& list) {
	system("CLS");
	writeProgName();
	setTextColor(YELLOW);
	wprintf(L"                                                              --- Xoá thông tin sách ---\n\n");
	int ret;
	char ISBN[ISBN_LEN];
	if (!secureInp(ISBN, ISBN_LEN, L"ISBN sách cần xoá: ", ACCEPTED_CHARS_NUMERIC))
	{
		return ESC_RETURN;
	}
	setTextColor(BLACK);
	Node* pBook = NULL;
	for (Node* p = list.head; p; p = p->next)
	{
		if (_strcmpi(ISBN, ((BookInfo*)p->data)->ISBN) == 0)
		{
			pBook = p;
			break;
		}
	}
	if (pBook == NULL)
	{
		wprintf(L"Sách không có trong thư viện!\n");
		ret = 0;
	}
	else
	{
		if (!confBox()) return ESC_RETURN;
		ret = 1;
		int count = 0;
		for (Node* q = list.head; q != pBook; q = q->next) {
			count++;
		}
		deleteAtIndex(list, count);
		//ghi lai toan bo file
		FILE* f = fopen(BOOK_FILE, "wt");
		if (f != NULL) {
			fputs("ISBN,Ten sach,Tac gia,Nha xuat ban,Nam xuat ban,The loai,Gia sach,So sach\n", f);
			BookInfo a;
			for (Node* p = list.head; p != NULL; p = p->next) {
				a = *(BookInfo*)(p->data);
				fprintf(f, "%s',%s,%s,%s,%d,%s,%d,%d\n", a.ISBN, a.bookName, a.authorName, a.NXB, a.namXB, a.typeBook, a.price, a.bookNum);
			}
			fclose(f);
		}
		else return NOT_OPEN_FILE;
	}
	return ret;
}
void BookManage(User user, List listBook) {
	system("CLS");
	int choice = 0, key;
	while (1) {
		system("CLS");
		writeProgName();
		ShowConsoleCursor(false);
		setTextColor(RED);
		wprintf(L"                                                                  === QUẢN LÝ SÁCH ===\n");
		setTextColor(YELLOW);
		wprintf(L"\n                                              Chọn chức năng bạn muốn thực hiện (sử dụng phím mũi tên lên/xuống)\n");
	bookMenu:
		gotoxy(0, 15);
		if (choice == 0) setTextColor(RED); else setTextColor(BLACK);
		wprintf(L"\n                                                                 <  Trở về menu chính  >\n");
		if (choice == 1) setTextColor(RED); else setTextColor(BLACK);
		wprintf(L"\n                                                              <  Tìm kiếm sách theo ISBN  >\n");
		if (choice == 2) setTextColor(RED); else setTextColor(BLACK);
		wprintf(L"\n                                                              <  Tìm kiếm sách theo tên  >\n");
		setTextColor(BLACK);
		if (user.role == ADMIN_ROLE || user.role == MANAGER_ROLE)
		{
			if (choice == 3) setTextColor(RED); else setTextColor(BLACK);
			wprintf(L"\n                                                        <  Xem danh sách các sách trong thư viện  >\n");
			if (choice == 4) setTextColor(RED); else setTextColor(BLACK);
			wprintf(L"\n                                                                    <  Thêm sách  >\n");
			if (choice == 5) setTextColor(RED); else setTextColor(BLACK);
			wprintf(L"\n                                                             <  Chỉnh sửa thông tin sách  >\n");
			if (choice == 6) setTextColor(RED); else setTextColor(BLACK);
			wprintf(L"\n                                                                <  Xoá thông tin sách  >\n");
		}
		key = _getch();
		switch (key)
		{
		case KEY_UP:
			if (choice == MENU_MIN_OPTION && (user.role == ADMIN_ROLE || user.role == MANAGER_ROLE)) choice = BOOKS_MENU_MAX_OPTION;
			else if (choice == MENU_MIN_OPTION && user.role == SPECIALIST_ROLE) choice = BOOKS_MENU_MAX_SPECIALIST_OPTION; else choice--;
			goto bookMenu;
			break;
		case KEY_DOWN:
			if (choice == BOOKS_MENU_MAX_OPTION) choice = MENU_MIN_OPTION;
			else if (choice == BOOKS_MENU_MAX_SPECIALIST_OPTION && user.role == SPECIALIST_ROLE) choice = MENU_MIN_OPTION; else choice++;
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
			findBookBy(findISBN, L"  ISBN: ", listBook, ISBN_LEN, ACCEPTED_CHARS_NUMERIC);
			break;
		case 2:
			findBookBy(findBookName, L"  Tên sách: ", listBook, BOOK_NAME_SIZE, ACCEPTED_CHARS_REGULAR);
			break;
		case 3:
			ReadBookList(listBook);
			break;
		case 4:
		{
			AddABook(listBook);
			break;
		}
		case 5:
		{
			int result = updateBook(listBook);
			if (result == UPDATE_SUCCESS)
			{
				setTextColor(GREEN);
				wprintf(L"Cập nhật thông tin sách thành công\n");
				dustIgnore();
			}
			else if (result == NOT_OPEN_FILE)
			{
				setTextColor(RED);
				wprintf(L"Không thể cập nhật file thông tin sách\n");
				dustIgnore();
			}
			break;
		}
		case 6:
		{
			int result = RemoveBook(listBook);
			if (result == UPDATE_SUCCESS)
			{
				setTextColor(GREEN);
				wprintf(L"Xoá thông tin sách thành công\n");
				dustIgnore();
			}
			else if (result == NOT_OPEN_FILE)
			{
				setTextColor(RED);
				wprintf(L"Không thể cập nhật file thông tin sách\n");
				dustIgnore();
			}
			break;
		}
		case 0:
			return;
			break;
		}
	}
}