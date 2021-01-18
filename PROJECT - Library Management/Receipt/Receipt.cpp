#include "Receipt.h"
#include "..\Books\Books.h"
void bookBorrow(List& bookList, MemberList memberList)
{
	system("CLS");
	time_t now = time(0);
	time_t exp = now + BRW_PERIOD;
	gotoxy(0, 4);
	wprintf(L"                                                                          █▀▀■▄ █▀▀▀ █▀▀▀ █▀▀▀ █ █▀▀■▄▀▀█▀▀\n");
	wprintf(L"                                                                          █ ▄■▀ █▄▄▄ █    █▄▄▄ █ █ ▄■▀  █\n");
	wprintf(L"                                                                          █▀▄   █    █    █    █ █▀     █\n");
	wprintf(L"                                                                          █  ▀▄ █▄▄▄ █▄▄▄ █▄▄▄ █ █      █\n");
	drawLeftLogo();
	setTextColor(BLACK);
	char idCard[MAX_NAME_LEN];
	gotoxy(0, 14);
	if (!secureInp(idCard, MAX_IDCARD_LEN, L"  Nhập CMND độc giả muốn mượn sách: ", ACCEPTED_CHARS_NUMERIC)) return;
	Member* found = NULL;
	for (Node* p = memberList.members.head; p; p = p->next)
	{
		char tmp[MAX_NAME_LEN];
		strcpy(tmp, ((Member*)p->data)->idCard);
		caesarDecrypt(tmp);
		if (strcmp(tmp, idCard) == 0)
		{
			found = (Member*)p->data;
			break;
		}
	}
	if (found == NULL)
	{
		setTextColor(RED);
		wprintf(L"  Độc giả không tồn tại\n");
		dustIgnore();
		return;
	}
	//make time of card expired day for checking
	tm* gm = new tm;
	gm->tm_hour = 0;
	gm->tm_min = 0;
	gm->tm_sec = 0;
	gm->tm_mday = found->expired.day;
	gm->tm_mon = found->expired.month - 1;
	gm->tm_year = found->expired.year - 1900;
	time_t cardExp = mktime(gm);
	delete gm;
	if (difftime(cardExp, now) < 0.0)
	{
		setTextColor(RED);
		wprintf(L"  Thẻ độc giả đã hết hạn, vui lòng gia hạn thẻ để tiếp tục sử dụng các dịch vụ của thư viện\n");
		setTextColor(BLACK);
		wprintf(L"  Nhấn enter để quay lại menu\n");
		dustIgnore();
		return;
	}
	//init
	List borrow;
	listInit(borrow);
	List qty;
	listInit(qty);
	int count = 0;
	while (1)
	{
		char tmp[ISBN_LEN];
	isbnInp:
		if (!secureInp(tmp, ISBN_LEN, L"  Nhập ISBN những cuốn sách cần mượn (nhập . để dừng): ", ACCEPTED_CHARS_NUMERIC_DOT))
		{
			deleteList(qty);
			ReadBookFile(bookList);
			return;
		}
		if (strcmp(tmp, ".") != 0)
		{
			bool check = false;
			for (Node* p = bookList.head; p; p = p->next)
				if (strcmp(tmp, ((BookInfo*)p->data)->ISBN) == 0)
				{
					int n = 0;
					char ntmp[10];
					if (((BookInfo*)p->data)->bookNum == 0)
					{
						setTextColor(RED);
						wprintf(L"  Đã hết sách này trong kho!");
						dustIgnore();
						wprintf(L"\33[2K\033[F\33[2K");
						setTextColor(BLACK);
						goto isbnInp;
					}
					else
					{
						setTextColor(YELLOW);
						wprintf(L"  Số lượng sách còn trong kho: %d\n", ((BookInfo*)p->data)->bookNum);
					}
					if (!secureInp(ntmp, 10, L"  Nhập số lượng sách muốn mượn: ", ACCEPTED_CHARS_NUMERIC))
					{
						wprintf(L"\33[2K\033[F\33[2K\033[F\33[2K");
						goto isbnInp;
					}
					n = atoi(ntmp);
					while (n<1 || n>((BookInfo*)p->data)->bookNum)
					{
						setTextColor(RED);
						wprintf(L"\33[2K\033[F\33[2K");
						wprintf(L"  Số lượng sách bé hơn 1 hoặc lớn hơn số lượng sách còn trong kho!\n");
						if (!secureInp(ntmp, 10, L"  Nhập lại số lượng sách muốn mượn: ", ACCEPTED_CHARS_NUMERIC))
						{
							wprintf(L"\33[2K\033[F\33[2K\033[F\33[2K\033[F\33[2K");
							goto isbnInp;
						}
						n = atoi(ntmp);
						if (n<1 || n>((BookInfo*)p->data)->bookNum) wprintf(L"\33[2K\033[F");
					}
					// trừ đi những cuốn sách đã mượn
					((BookInfo*)p->data)->bookNum -= n;
					int* bf = new int;
					*bf = n;
					if (appendList(qty, bf) == NULL || appendList(borrow, p->data) == NULL)
					{
						setTextColor(RED);
						wprintf(L"Không đủ bộ nhớ để hoạt động!");
						deleteList(qty);
						dustIgnore();
						return;
					}
					setTextColor(GREEN);
					wprintf(L"  Thêm sách thành công");
					setTextColor(BLACK);
					dustIgnore();
					wprintf(L"\33[2K\033[F\33[2K\033[F\33[2K\033[F\33[2K");
					count++;
					check = true;
					break;
				}
			if (!check)
			{
				setTextColor(RED);
				wprintf(L"  Sách không tồn tại");
				dustIgnore();
				wprintf(L"\33[2K\033[F\33[2K");
			}
		}
		else break;
	}
	FILE* f = fopen(RECEIPT_FILE, "ab");
	if (f == NULL)
	{
		if (_mkdir("Data") == 0) wprintf(L"  Đã tạo thư mục Data");
		if (fopen(RECEIPT_FILE, "wb") == NULL)
		{
			setTextColor(RED);
			wprintf(L"  Không thể mở file dữ liệu phiếu\n");
			setTextColor(BLACK);
			wprintf(L"  Nhấn enter để quay lại menu");
			deleteList(qty);
			ReadBookFile(bookList);
			dustIgnore();
			return;
		}
		f = fopen(RECEIPT_FILE, "ab");
	}
	//thời gian mượn sách là hiện tại
	gm = gmtime(&now);
	//ghi record
	Node* nq = qty.head;
	ReceiptRecord bf;
	caesarEncrypt(idCard);
	strcpy(bf.idCard, idCard);
	bf.time = *gm;
	bf.returned = 0;
	for (Node* p = borrow.head; p; p = p->next)
	{
		strcpy(bf.ISBN, ((BookInfo*)p->data)->ISBN);
		for (int i = 0; i < *((int*)nq->data); i++)
		{
			if (fwrite(&bf, sizeof(ReceiptRecord), 1, f) != 1)
			{
				setTextColor(RED);
				wprintf(L"  Không thể ghi file dữ liệu phiếu\n");
				deleteList(qty);
				ReadBookFile(bookList);
				dustIgnore();
				return;
			}
		}
		nq = nq->next;
	}
	fclose(f);
	//cập nhật số lượng sách còn lại
	updateBookQty(bookList);
	if (qty.head == NULL)
	{
		setTextColor(RED);
		wprintf(L"  Bạn chưa chọn bất kì cuốn sách nào để mượn\n");
		setTextColor(BLACK);
		wprintf(L"  Nhấn enter để trở về menu");
		dustIgnore();
		ReadBookFile(bookList);
		return;
	}
	setTextColor(GREEN);
	wprintf(L"  Ghi phiếu mượn sách thành công!\n");
	setTextColor(BLACK);
	wprintf(L"  Nhấn enter để in phiếu mượn sách ra màn hình");
	dustIgnore();
	wprintf(L"\33[2K\033[F\33[2K\033[F\33[2K\033[F\33[2K");
	setTextColor(YELLOW);
	wprintf(L"  Thông tin độc giả\n");
	setTextColor(PURPLE);
	wprintf(L"     Tên độc giả: ");
	setTextColor(BLACK);
	wprintf(L"%S\n", found->fullName);
	setTextColor(PURPLE);
	wprintf(L"     CMND: ");
	setTextColor(BLACK);
	caesarDecrypt(idCard);
	wprintf(L"%S\n", idCard);
	setTextColor(YELLOW);
	wprintf(L"  Thông tin sách mượn\n");
	nq = qty.head;
	for (Node* p = borrow.head; p; p = p->next)
	{
		setTextColor(PURPLE);
		wprintf(L"     + Tên sách: ");
		setTextColor(BLACK);
		wprintf(L"%S - ISBN: %S - Số lượng: %d\n", ((BookInfo*)p->data)->bookName, ((BookInfo*)p->data)->ISBN, *(int*)nq->data);
		nq=nq->next;
	}
	setTextColor(RED);
	wprintf(L"  Ngày lập phiếu: %02d/%02d/%02d - %02d:%02d:%02d\n", gm->tm_mday, 1 + gm->tm_mon, 1900 + gm->tm_year, (gm->tm_hour + _TIME_ZONE) % 24, gm->tm_min, gm->tm_sec);
	gm = gmtime(&exp);
	wprintf(L"  Hạn trả sách: %02d/%02d/%02d - %02d:%02d:%02d\n", gm->tm_mday, 1 + gm->tm_mon, 1900 + gm->tm_year, (gm->tm_hour + _TIME_ZONE) % 24, gm->tm_min, gm->tm_sec);
	deleteList(qty);
	dustIgnore();
}
void updateBookQty(List& bookList)
{
	FILE* f = fopen(BOOK_FILE, "wt");
	if (f == NULL)
	{
		setTextColor(RED);
		wprintf(L"  Không thể cập nhật file dữ liệu sách\n");
		ReadBookFile(bookList);
		dustIgnore();
		return;
	}
	fputs("ISBN,Ten sach,Tac gia,Nha xuat ban,Nam xuat ban,The loai,Gia sach,So sach\n", f);
	for (Node* p = bookList.head; p; p = p->next)
	{
		BookInfo* cNode = (BookInfo*)p->data;
		if (fprintf(f, "%s',%s,%s,%s,%d,%s,%d,%d\n", cNode->ISBN, cNode->bookName, cNode->authorName, cNode->NXB, cNode->namXB, cNode->typeBook, cNode->price, cNode->bookNum) < 0)
		{
			setTextColor(RED);
			wprintf(L"  Không thể cập nhật file dữ liệu sách\n");
			ReadBookFile(bookList);
			dustIgnore();
			return;
		}
	}
	fclose(f);
}
void bookReturn(List& bookList, MemberList memberList)
{
	system("CLS");
	gotoxy(0, 4);
	wprintf(L"                                                                          █▀▀■▄ █▀▀▀ █▀▀▀ █▀▀▀ █ █▀▀■▄▀▀█▀▀\n");
	wprintf(L"                                                                          █ ▄■▀ █▄▄▄ █    █▄▄▄ █ █ ▄■▀  █\n");
	wprintf(L"                                                                          █▀▄   █    █    █    █ █▀     █\n");
	wprintf(L"                                                                          █  ▀▄ █▄▄▄ █▄▄▄ █▄▄▄ █ █      █\n");
	drawLeftLogo();
	FILE* f = fopen(RECEIPT_FILE, "r+b");
	if (f == NULL)
	{
		if (_mkdir("Data") == 0) wprintf(L"  Đã tạo thư mục Data");
		if (fopen(RECEIPT_FILE, "wb") == NULL)
		{
			setTextColor(RED);
			wprintf(L"  Không thể mở file dữ liệu phiếu\n");
			setTextColor(BLACK);
			wprintf(L"  Nhấn enter để quay lại menu");
			ReadBookFile(bookList);
			dustIgnore();
			return;
		}
		f = fopen(RECEIPT_FILE, "r+b");
	}
	//get file size
	fseek(f, 0, SEEK_END);
	long long size = ftell(f);
	size /= sizeof(ReceiptRecord);
	fseek(f, 0, SEEK_SET);
	gotoxy(0, 14);
	time_t now = time(NULL);
	tm gm = *gmtime(&now);
	//convert to UTC
	now = mktime(&gm);
	char idCard[MAX_IDCARD_LEN];
	//Nhập cmnd
	if (!secureInp(idCard, MAX_IDCARD_LEN, L"  Nhập CMND của độc giả muốn trả sách: ", ACCEPTED_CHARS_NUMERIC)) return;
	Member* found = NULL;
	for (Node* p = memberList.members.head; p; p = p->next)
	{
		char tmp[MAX_IDCARD_LEN];
		strcpy(tmp, ((Member*)p->data)->idCard);
		caesarDecrypt(tmp);
		if (strcmp(tmp, idCard) == 0)
		{
			found = (Member*)p->data;
			break;
		}
	}
	//kiểm tra độc giả có tồn tại hay ko
	if (found == NULL)
	{
		setTextColor(RED);
		wprintf(L"  Độc giả không tồn tại!\n");
		setTextColor(BLACK);
		wprintf(L"  Nhấn enter để quay lại menu");
		dustIgnore();
		fclose(f);
		return;
	}
	//init buffer, phí và danh sách các sách sẽ trả
	ReceiptRecord bf;
	unsigned int totalFee = 0;
	List returnedBooks;
	listInit(returnedBooks);
	while (1)
	{
		char tmp[ISBN_LEN];
	isbnInp:
		if (!secureInp(tmp, ISBN_LEN, L"  Nhập ISBN những cuốn sách cần trả (nhập . để dừng): ", ACCEPTED_CHARS_NUMERIC_DOT))
		{
			ReadBookFile(bookList);
			fclose(f);
			return;
		}
		if (strcmp(tmp, ".") == 0) break; 
		else
		{
			fseek(f, 0, SEEK_SET);
			for (int i = 0; i < size; i++)
			{
				if (fread(&bf, sizeof(ReceiptRecord), 1, f) != 1)
				{
					setTextColor(RED);
					wprintf(L"  Không thể đọc file dữ liệu phiếu\n");
					setTextColor(BLACK);
					wprintf(L"  Nhấn enter để quay lại menu");
					dustIgnore();
					ReadBookFile(bookList);
					fclose(f);
					return;
				}
				caesarDecrypt(bf.idCard);
				if (strcmp(bf.idCard, idCard) == 0 && strcmp(tmp, bf.ISBN) == 0 && !bf.returned)
				{
					for (Node* p = bookList.head; p; p = p->next)
					{
						if (strcmp(((BookInfo*)p->data)->ISBN, bf.ISBN) == 0)
						{
							((BookInfo*)p->data)->bookNum++;
							bf.returned = 1;
							updateBookQty(bookList);
							caesarEncrypt(bf.idCard);
							//seek ngược lại để ghi đè lên record vừa tìm thấy
							fseek(f, 0 - sizeof(ReceiptRecord), SEEK_CUR);
							if (fwrite(&bf, sizeof(ReceiptRecord), 1, f) != 1)
							{
								setTextColor(RED);
								wprintf(L"  Không thể ghi file dữ liệu phiếu\n");
								setTextColor(BLACK);
								wprintf(L"  Nhấn enter để quay lại menu");
								dustIgnore();
								ReadBookFile(bookList);
								fclose(f);
								return;
							}
							setTextColor(GREEN);
							if (appendList(returnedBooks, p->data) == NULL)
							{
								setTextColor(RED);
								wprintf(L"  Không đủ bộ nhớ để hoạt động!");
								ReadBookFile(bookList);
								fclose(f);
								return;
							}
							time_t brw = mktime(&bf.time);
							int daysLate = difftime(now, brw);
							if (daysLate > BRW_PERIOD)
							{
								daysLate = daysLate - BRW_PERIOD;
								if (daysLate % DAY_IN_SECOND != 0)
									daysLate = (daysLate / DAY_IN_SECOND) + 1;
								else daysLate = (daysLate / DAY_IN_SECOND);
								totalFee += daysLate * FINE_PER_DAY;
								setTextColor(RED);
								wprintf(L"  Bạn trả sách trễ %d ngày, bạn bị phạt %d.000đ\n", daysLate, daysLate* FINE_PER_DAY);
								dustIgnore();
								wprintf(L"\33[2K\033[F\33[2K");
							}
							setTextColor(GREEN);
							wprintf(L"  Trả sách thành công");
							dustIgnore();
							wprintf(L"\33[2K\033[F\33[2K");
							goto isbnInp;
						}
					}
					setTextColor(RED);
					wprintf(L"  Sách này hiện không còn trong cơ sở dữ liệu của thư viện\n");
					dustIgnore();
					wprintf(L"\33[2K\033[F\33[2K");
					goto isbnInp;
				}
			}
			setTextColor(RED);
			wprintf(L"  Bạn không mượn sách này!");
			dustIgnore();
			wprintf(L"\33[2K\033[F\33[2K");
		}
	}
	//đóng file
	fclose(f);
	wprintf(L"\33[2K\033[F\33[2K");
	if (returnedBooks.head == NULL)
	{
		setTextColor(RED);
		wprintf(L"  Bạn chưa chọn bất kì cuốn sách nào để trả\n");
		setTextColor(BLACK);
		wprintf(L"  Nhấn enter để trở về menu");
		dustIgnore();
		ReadBookFile(bookList);
		return;
	}
	setTextColor(YELLOW);
	wprintf(L"  Thông tin độc giả\n");
	setTextColor(PURPLE);
	wprintf(L"     Tên độc giả: ");
	setTextColor(BLACK);
	wprintf(L"%S\n", found->fullName);
	setTextColor(PURPLE);
	wprintf(L"     CMND: ");
	setTextColor(BLACK);
	wprintf(L"%S\n", idCard);
	setTextColor(YELLOW);
	wprintf(L"  Thông tin sách trả\n");
	for (Node* p = returnedBooks.head; p; p = p->next)
	{
		setTextColor(PURPLE);
		wprintf(L"     + Tên sách: ");
		setTextColor(BLACK);
		wprintf(L"%S - ISBN: %S\n", ((BookInfo*)p->data)->bookName, ((BookInfo*)p->data)->ISBN);
	}
	setTextColor(YELLOW);
	if (totalFee != 0) wprintf(L"  Tổng chi phí mà độc giả phải trả: %d.000đ", totalFee); else wprintf(L"  Độc giả được miễn phí trả sách!");
	dustIgnore();
}