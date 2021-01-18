#include "Member.h"
void printAMember(Member* member)
{
	//Giai ma cac field nhay cam de in ra man hinh
	caesarDecrypt(member->address);
	caesarDecrypt(member->idCard);
	caesarDecrypt(member->email);
	wprintf(L"-------------------------------------------------\n");
	wprintf(L" Mã độc giả: %d\n", member->id);
	wprintf(L"     + Họ tên: %S", member->fullName);
	wprintf(L"     + CMND/CCCD: %S", member->idCard);
	wprintf(L"     + Ngày sinh: %02d/%02d/%02d\n", member->birthday.day, member->birthday.month, member->birthday.year);
	if (member->gender == 1)
		wprintf(L"     + Giới tính: Nam\n");
	else
		wprintf(L"     + Giới tính: Nữ\n");
	wprintf(L"     + Email: %S\n", member->email);
	wprintf(L"     + Địa chỉ: %S\n", member->address);
	wprintf(L"     + Ngày cấp thẻ: %02d/%02d/%02d", member->issued.day, member->issued.month, member->issued.year);
	wprintf(L"     + Ngày hết hạn: %02d/%02d/%02d\n", member->expired.day, member->expired.month, member->expired.year);
	//Ma hoa lai
	caesarEncrypt(member->address);
	caesarEncrypt(member->idCard);
	caesarEncrypt(member->email);
}
void viewMembers(MemberList& memList)
{
	system("CLS");
	writeProgName();
	setTextColor(RED);
	wprintf(L"                                                             --- Xem Danh Sách Độc Giả ---\n");
	setTextColor(YELLOW);
	if (memList.size == 0)
	{
		setTextColor(YELLOW);
		wprintf(L"Không có độc giả nào để in danh sách\n");
		setTextColor(BLACK);
		wprintf(L"Nhấn enter để quay lại menu");
		dustIgnore();
		return;
	}
	wprintf(L"   # Tổng số độc giả: %d\n", memList.size);
	setTextColor(BLACK);
	for (Node* p = memList.members.head; p; p = p->next)
	{
		printAMember((Member*)p->data);
	}
	setTextColor(GREEN);
	wprintf(L"\nIn thành công danh sách độc giả!\n");
	setTextColor(BLACK);
	wprintf(L"Nhấn enter để quay lại menu\n");
	dustIgnore();
}
void getMemberList(MemberList& memList)
{
	listInit(memList.members);
	memList.size = 0;
	//Doc va kiem tra file
	FILE* f = fopen(MEMBER_DATABASE_FILE, "r+b");
	if (f == NULL)
	{
		if (_mkdir("Data") == 0) wprintf(L"Đã tạo thư mục Data\n");
		if (fopen(MEMBER_DATABASE_FILE, "wb") == NULL)
		{
			setTextColor(RED);
			wprintf(L"Không thể mở file dữ liệu độc giả\n");
			setTextColor(BLACK);
			exit(1);
		}
		f = fopen(MEMBER_DATABASE_FILE, "r+b");
	}
	//Doc size
	if (fread(&memList.size, sizeof(memList.size), 1, f) != 1) memList.size = 0;
	//Doc list
	for (int i = 0; i < memList.size; i++)
	{
		Member* buffer = new Member;
		if (buffer == NULL)
		{
			setTextColor(RED);
			wprintf(L"Không đủ bộ nhớ để chương trình hoạt động!\n");
			exit(1);
		}
		if (fread(buffer, sizeof(Member), 1, f) != 1)
		{
			setTextColor(RED);
			wprintf(L"Không thể đọc file dữ liệu độc giả\n");
			exit(1);
		}
		if (appendList(memList.members, buffer) == NULL)
		{
			setTextColor(RED);
			wprintf(L"Không đủ bộ nhớ để chương trình hoạt động!\n");
			exit(1);
		}
	}
}
bool memberInfoForm(MemberList memList, Member* newMember, int id, bool isUpdate)
{
	system("CLS");
	//Set MemberID
	newMember->id = id;
	writeProgName();
	setTextColor(RED);
	wprintf(L"                                                                 --- Thông Tin Độc Giả ---\n");
	setTextColor(YELLOW);
	wprintf(L"Nhập thông tin độc giả muốn thêm / cập nhật:\n");
	setTextColor(BLACK);
	//Full name
	if (!secureInp(newMember->fullName, MAX_NAME_LEN, L"Họ tên: ", ACCEPTED_CHARS_NAME)) return false;
	//Address
	if  (!secureInp(newMember->address, MAX_ADD_LEN, L"Địa chỉ: ", ACCEPTED_CHARS_ADDRESS)) return false;
	caesarEncrypt(newMember->address);
	char tmp[5];
	//Birthday
	setTextColor(PURPLE);
	wprintf(L"Ngày sinh:\n");
	if (!secureInp(tmp, 3, L"    Ngày: ", ACCEPTED_CHARS_NUMERIC)) return false;
	newMember->birthday.day = atoi(tmp);
	if (!secureInp(tmp, 3, L"    Tháng: ", ACCEPTED_CHARS_NUMERIC)) return false;
	newMember->birthday.month = atoi(tmp);
	if (!secureInp(tmp, 5, L"    Năm: ", ACCEPTED_CHARS_NUMERIC)) return false;
	newMember->birthday.year = atoi(tmp);
	while (newMember->birthday.day < 1 ||
		newMember->birthday.day>31 ||
		newMember->birthday.year < 0 ||
		newMember->birthday.month < 1 ||
		newMember->birthday.month>12 ||
		(newMember->birthday.month == 2 && newMember->birthday.day > 28 && newMember->birthday.year % 4 != 0) ||
		(newMember->birthday.month == 2 && newMember->birthday.year % 4 == 0 && newMember->birthday.day > 29))
	{
		wprintf(L"\033[F\33[2K\033[F\33[2K\033[F\33[2K\033[F\33[2K");
		wprintf(L"\x1b[38;2;223;21;62mNgày sinh không hợp lệ!\x1b[38;2;0;0;0m Nhập lại ngày sinh:\n");
		if (!secureInp(tmp, 3, L"    Ngày: ", ACCEPTED_CHARS_NUMERIC)) return false;
		newMember->birthday.day = atoi(tmp);
		if (!secureInp(tmp, 3, L"    Tháng: ", ACCEPTED_CHARS_NUMERIC)) return false;
		newMember->birthday.month = atoi(tmp);
		if (!secureInp(tmp, 5, L"    Năm: ", ACCEPTED_CHARS_NUMERIC)) return false;
		newMember->birthday.year = atoi(tmp);
	}
	//IDCard
	char prevId[MAX_IDCARD_LEN];
	strcpy(prevId, newMember->idCard);
	caesarDecrypt(prevId);
	if (!secureInp(newMember->idCard, MAX_IDCARD_LEN, L"CMND/CCCD (9 hoặc 12 chữ số): ", ACCEPTED_CHARS_NUMERIC)) return false;
	if (isUpdate == false)
	{
		while (memberIdCardCheck(memList.members, newMember->idCard) != NULL || (strlen(newMember->idCard) != 9 && strlen(newMember->idCard) != 12))
		{
			wprintf(L"\033[F\33[2K\r");
			if (!secureInp(newMember->idCard, MAX_IDCARD_LEN, L"\x1b[38;2;223;21;62mCMND/CCCD bị trùng lặp hoặc không hợp lệ!\x1b[38;2;0;0;0m Nhập lại CMND/CCCD: ", ACCEPTED_CHARS_NUMERIC)) return false;
		}
	}
	else
	{
		if (strcmp(prevId, newMember->idCard) != 0)
		while (memberIdCardCheck(memList.members, newMember->idCard) != NULL || (strlen(newMember->idCard) != 9 && strlen(newMember->idCard) != 12))
		{
			wprintf(L"\033[F\33[2K\r");
			if (!secureInp(newMember->idCard, MAX_IDCARD_LEN, L"\x1b[38;2;223;21;62mCMND/CCCD bị trùng lặp hoặc không hợp lệ!\x1b[38;2;0;0;0m Nhập lại CMND/CCCD: ", ACCEPTED_CHARS_NUMERIC)) return false;
			if (strcmp(prevId, newMember->idCard) == 0) break;
		}
	}
	caesarEncrypt(newMember->idCard);

	int choice = 0, key;
	//Chon gioi tinh
	setTextColor(PURPLE);
	wprintf(L"Giới tính:\n");
	genderChooser:
	if (choice == GENDER_FEMALE) setTextColor(RED); else setTextColor(BLACK);
	wprintf(L"   > Nữ\n");
	if (choice == GENDER_MALE) setTextColor(RED); else setTextColor(BLACK);
	wprintf(L"   > Nam\n");
	setTextColor(BLACK);
	key = _getch();
	switch (key)
	{
	case KEY_UP:
		if (choice == 0) choice = 1; else choice = 0;
		wprintf(L"\033[F\033[F");
		goto genderChooser;
		break;
	case KEY_DOWN:
		if (choice == 0) choice = 1; else choice = 0;
		wprintf(L"\033[F\033[F");
		goto genderChooser;
		break;
	case ESC_KEY:
		return false;
		break;
	case KEY_ENTER:
		break;
	default:
		wprintf(L"\033[F\033[F");
		goto genderChooser;
		break;
	}
	newMember->gender = choice;
	
	//Email
	if (!secureInp(newMember->email, MAX_EMAIL_LEN, L"Email: ", ACCEPTED_CHARS_EMAIL)) return false;
	caesarEncrypt(newMember->email);
	time_t now, end;
	time(&now);
	tm *gm = gmtime(&now);
	newMember->issued.day = gm->tm_mday;
	newMember->issued.month = gm->tm_mon + 1;
	newMember->issued.year = gm->tm_year + 1900;
	end = now + DAY_IN_SECOND * 365 * 2;
	gm = gmtime(&end);
	newMember->expired.day = gm->tm_mday;
	newMember->expired.month = gm->tm_mon + 1;
	newMember->expired.year = gm->tm_year + 1900;
	return true;
}
void addMember(MemberList& memList)
{
	system("CLS");
	Member* newMember = new Member;
	if (newMember == NULL)
	{
		setTextColor(RED);
		wprintf(L"Không đủ bộ nhớ để chương trình hoạt động\n");
		exit(1);
	}
	if (!memberInfoForm(memList, newMember, memList.size, 0)) return;
	//Check neu khong co file 
	FILE* f = fopen(MEMBER_DATABASE_FILE, "r+b");
	if (f == NULL)
	{
		if (_mkdir("Data") == 0) wprintf(L"Đã tạo thư mục Data\n");
		if (fopen(MEMBER_DATABASE_FILE, "wb") == NULL)
		{
			setTextColor(RED);
			wprintf(L"Không thể mở file dữ liệu độc giả\n");
			setTextColor(BLACK);
			wprintf(L"Nhấn enter để quay lại menu");
			dustIgnore();
			return;
		}
		f = fopen(MEMBER_DATABASE_FILE, "r+b");
	}
	if (!confBox()) return;
	//Ghi file thong tin doc gia
	memList.size++;
	if (fwrite(&memList.size, sizeof(memList.size), 1, f) != 1)
	{
		setTextColor(RED);
		wprintf(L"Không thể cập nhật file dữ liệu độc giả\n");
		setTextColor(BLACK);
		wprintf(L"Nhấn enter để quay lại menu");
		fclose(f);
		dustIgnore();
		return;
	}
	fseek(f, 0, SEEK_END);
	if (fwrite(newMember, sizeof(Member), 1, f) == 1)
	{
		if (appendList(memList.members, newMember) == NULL) wprintf(L"Không đủ bộ nhớ để chương trình hoạt động\n");
		setTextColor(GREEN);
		wprintf(L"Thêm độc giả thành công!\n");
		setTextColor(BLACK);
		fclose(f);
		wprintf(L"Nhấn enter để quay lại menu");
		dustIgnore();
		return;
	}
	else
	{
		setTextColor(RED);
		memList.size--;
		fwrite(&memList.size, sizeof(memList.size), 1, f);
		wprintf(L"Khong the cap nhat file du lieu tai khoan\nThem tai khoan khong thanh cong\n");
		setTextColor(BLACK);
		fclose(f);
		wprintf(L"Nhan enter de quay lai menu");
		dustIgnore();
		return;
	}
}
void updateMember(MemberList memList)
{
	system("CLS");
	Member* chosenMember = NULL;
	writeProgName();
	setTextColor(RED);
	wprintf(L"                                                          --- Cập Nhật Thông Tin Độc Giả ---\n");
	setTextColor(BLACK);
	char chosenIdCard[MAX_IDCARD_LEN];
	if (!secureInp(chosenIdCard, MAX_IDCARD_LEN, L"Nhập CMND của độc giả cần cập nhật thông tin: ", ACCEPTED_CHARS_NUMERIC)) return;
	for (Node* currentNode = memList.members.head; currentNode; currentNode = currentNode->next)
	{
		char tmp[MAX_IDCARD_LEN];
		strcpy(tmp, ((Member*)currentNode->data)->idCard);
		caesarDecrypt(tmp);
		if (strcmp(tmp, chosenIdCard) == 0)
		{
			chosenMember = (Member*)currentNode->data;
			break;
		}
	}
	if (chosenMember == NULL)
	{
		setTextColor(RED);
		wprintf(L"Độc giả không tồn tại\n");
		dustIgnore();
		return;
	}
	Member thisMember = *chosenMember;
	if (!memberInfoForm(memList, &thisMember, thisMember.id, true)) return;
	//Check neu khong co file 
	FILE* f = fopen(MEMBER_DATABASE_FILE, "r+b");
	if (f == NULL)
	{
		if (_mkdir("Data") == 0) wprintf(L"Đã tạo thư mục Data\n");
		if (fopen(MEMBER_DATABASE_FILE, "wb") == NULL)
		{
			setTextColor(RED);
			wprintf(L"Không thể mở file dữ liệu độc giả\n");
			setTextColor(BLACK);
			wprintf(L"Nhấn enter để quay lại menu");
			dustIgnore();
			return;
		}
		f = fopen(MEMBER_DATABASE_FILE, "r+b");
	}
	if (!confBox()) return;
	if (!rewriteReceiptMemID(chosenMember->idCard, thisMember.idCard)) return;
	//Ghi file thong tin doc gia
	fseek(f, sizeof(memList.size) + thisMember.id * sizeof(Member), SEEK_SET);
	if (fwrite(&thisMember, sizeof(Member), 1, f) == 1)
	{
		setTextColor(GREEN);
		wprintf(L"Cập nhật thông tin độc giả thành công!\n");
		setTextColor(BLACK);
		*chosenMember = thisMember;
		fclose(f);
		wprintf(L"Nhấn enter để quay lại menu");
		dustIgnore();
		return;
	}
	else
	{
		setTextColor(RED);
		wprintf(L"Không thể cập nhật file dữ liệu độc giả\nCập nhật thông tin độc giả không thành công\n");
		setTextColor(BLACK);
		fclose(f);
		wprintf(L"Nhấn enter để quay lại menu");
		dustIgnore();
		return;
	}
}
void searchMemberByIDCard(MemberList memList)
{
	char idCardForSearch[MAX_IDCARD_LEN];
	system("CLS");
	writeProgName();
	setTextColor(RED);
	wprintf(L"                                                       --- Tìm Thông Tin Độc Giả Bằng CMND ---\n");
	setTextColor(BLACK);
	if (!secureInp(idCardForSearch, MAX_IDCARD_LEN, L"Nhập CMND độc giả cần tìm thông tin: ", ACCEPTED_CHARS_NUMERIC)) return;
	for (Node* currentNode = memList.members.head; currentNode; currentNode = currentNode->next)
	{
		char thisNodeIdCard[MAX_IDCARD_LEN];
		strcpy(thisNodeIdCard, ((Member*)currentNode->data)->idCard);
		caesarDecrypt(thisNodeIdCard);
		if (strcmp(thisNodeIdCard, idCardForSearch) == 0)
		{
			printAMember((Member*)currentNode->data);
			setTextColor(GREEN);
			wprintf(L"Đã tìm thấy độc giả có số CMND cần tìm!\n");
			dustIgnore();
			return;
		}
	}
	setTextColor(RED);
	wprintf(L"Độc giả không tồn tại\n");
	dustIgnore();
	return;
}
void searchMemberByName(MemberList memList)
{
	char nameForSearch[MAX_IDCARD_LEN];
	system("CLS");
	writeProgName();
	setTextColor(RED);
	wprintf(L"                                                         -- Tìm Thông Tin Độc Giả Bằng Họ Tên ---\n");
	setTextColor(BLACK);
	int n = 0;
	if (!secureInp(nameForSearch, MAX_NAME_LEN, L"Nhập họ tên độc giả cần tìm: ", ACCEPTED_CHARS_NAME)) return;
	//to lower
	for (int i = 0; i < strlen(nameForSearch); i++) nameForSearch[i] = tolower(nameForSearch[i]);
	for (Node* currentNode = memList.members.head; currentNode; currentNode = currentNode->next)
	{
		if (nameCmp(nameForSearch,((Member*)currentNode->data)->fullName))
		{
			printAMember((Member*)currentNode->data);
			n++;
		}
	}
	if (n != 0)
	{
		setTextColor(GREEN);
		wprintf(L"-------------------------------------------------\nĐã tìm được %d độc giả có họ tên đã nhập\n", n);
	}
	else
	{
		setTextColor(RED);
		wprintf(L"-------------------------------------------------\nKhông tồn tại độc giả nào có họ tên bạn đã nhập\n");
	}
	dustIgnore();
	return;
}
void deleteMember(MemberList& memList)
{
	system("CLS");
	writeProgName();
	setTextColor(RED); 
	wprintf(L"                                                             --- Xoá Thông Tin Một Độc Giả ---\n");
	setTextColor(BLACK);  
	char chosenIDC[MAX_IDCARD_LEN];
	if (!secureInp(chosenIDC, MAX_IDCARD_LEN, L"Nhập CMND của độc giả bạn muốn xoá thông tin: ", ACCEPTED_CHARS_NUMERIC)) return;
	int idx = -1, count = 0;
	for (Node* currentNode = memList.members.head; currentNode; currentNode = currentNode->next)
	{
		count++;
		char tmp[MAX_IDCARD_LEN];
		strcpy(tmp, ((Member*)currentNode->data)->idCard);
		caesarDecrypt(tmp);
		if (strcmp(tmp, chosenIDC) != 0)
		{
			if (idx != -1) ((Member*)currentNode->data)->id--;
		}
		else idx = count - 1;
	}
	//check file
	if (!confBox()) return;
	FILE* f = fopen(MEMBER_DATABASE_FILE, "wb");
	if (f == NULL)
	{
		if (_mkdir("Data") == 0) wprintf(L"Đã tạo thư mục Data\n");
		if (fopen(MEMBER_DATABASE_FILE, "wb") == NULL)
		{
			setTextColor(RED);
			wprintf(L"Không thể mở file dữ liệu thông tin độc giả\n");
			setTextColor(BLACK);
			wprintf(L"Nhấn enter để quay lại menu");
			dustIgnore();
			return;
		}
		f = fopen(MEMBER_DATABASE_FILE, "r+b");
	}
	if (idx != -1)
	{
		memList.size--;
		deleteAtIndex(memList.members, idx);
	}
	else
	{
		setTextColor(RED);
		wprintf(L"Độc giả không tồn tại\n");
		dustIgnore();
		return;
	}
	if (fwrite(&memList.size, sizeof(memList.size), 1, f) != 1)
	{
		wprintf(L"\x1b[38;2;223;21;62mKhông thể cập nhật file thông tin độc giả\n\x1b[38;2;0;0;0mNhấn enter để quay lại");
		fclose(f);
		dustIgnore();
		exit(1);
	}
	for (Node* currentNode = memList.members.head; currentNode; currentNode = currentNode->next)
	{
		if (fwrite((Member*)currentNode->data, sizeof(Member), 1, f) != 1)
		{
			setTextColor(RED);
			wprintf(L"Không thể ghi file thông tin độc giả\n");
			fclose(f);
			dustIgnore();
			return;
		}
	}
	fclose(f);
	setTextColor(GREEN);
	wprintf(L"Xoá thông tin độc giả thành công\n");
	dustIgnore();
}
void cardExt(MemberList memList)
{
	system("CLS");
	writeProgName();
	setTextColor(RED);
	wprintf(L"                                                             --- Gia Hạn Thẻ Độc Giả ---\n");
	setTextColor(BLACK);
	char chosenIDC[MAX_IDCARD_LEN];
	if (!secureInp(chosenIDC, MAX_IDCARD_LEN, L"Nhập CMND của độc giả bạn muốn gia hạn thẻ: ", ACCEPTED_CHARS_NUMERIC)) return;
	int idx = -1, count = 0;
	Member* found = NULL;
	for (Node* currentNode = memList.members.head; currentNode; currentNode = currentNode->next)
	{
		count++;
		char tmp[MAX_IDCARD_LEN];
		strcpy(tmp, ((Member*)currentNode->data)->idCard);
		caesarDecrypt(tmp);
		if (strcmp(tmp, chosenIDC) == 0)
		{
			idx = count - 1;
			found = (Member*)currentNode->data;
		}
	}
	//check file
	FILE* f = fopen(MEMBER_DATABASE_FILE, "r+b");
	if (f == NULL)
	{
		if (_mkdir("Data") == 0) wprintf(L"Đã tạo thư mục Data\n");
		if (fopen(MEMBER_DATABASE_FILE, "wb") == NULL)
		{
			setTextColor(RED);
			wprintf(L"Không thể mở file dữ liệu thông tin độc giả\n");
			setTextColor(BLACK);
			wprintf(L"Nhấn enter để quay lại menu");
			dustIgnore();
			return;
		}
		f = fopen(MEMBER_DATABASE_FILE, "r+b");
	}
	if (idx != -1)
	{
		time_t now, end;
		time(&now);
		tm* gm = gmtime(&now);
		found->issued.day = gm->tm_mday;
		found->issued.month = gm->tm_mon + 1;
		found->issued.year = gm->tm_year + 1900;
		end = now + DAY_IN_SECOND * 365 * 2;
		gm = gmtime(&end);
		found->expired.day = gm->tm_mday;
		found->expired.month = gm->tm_mon + 1;
		found->expired.year = gm->tm_year + 1900;
	}
	else
	{
		setTextColor(RED);
		wprintf(L"Độc giả không tồn tại\n");
		dustIgnore();
		return;
	}
	fseek(f, sizeof(memList.size) + sizeof(Member) * idx, SEEK_SET);
	if (fwrite(found, sizeof(Member*), 1, f) != 1)
	{
		setTextColor(RED);
		wprintf(L"Không thể ghi file thông tin độc giả\n");
		fclose(f);
		dustIgnore();
		return;
	}
	fclose(f);
	setTextColor(GREEN);
	wprintf(L"Gia hạn thẻ độc giả thành công\n");
	dustIgnore();
}
bool rewriteReceiptMemID(char* oldId, char* newId)
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
		if (strcmp(tmp.idCard, oldId) == 0)
		{
			strcpy(tmp.idCard, newId);
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
	fclose(f);
	return 1;
}
void memberManage(MemberList& memList, User user)
{
	system("CLS");
	int choice = 0, key;
	while (1)
	{
		if (user.id < 0) return;
		system("CLS");
		writeProgName();
		ShowConsoleCursor(false);
		setTextColor(RED);
		wprintf(L"                                                                 === QUẢN LÝ ĐỘC GIẢ ===\n");
		setTextColor(YELLOW);
		wprintf(L"\n                                              Chọn chức năng bạn muốn thực hiện (sử dụng phím mũi tên lên/xuống)\n");
	memberMenu:
		gotoxy(0, 15);
		if (choice == 0) setTextColor(RED); else setTextColor(BLACK);
		wprintf(L"\n                                                                 <  Trở về menu chính >\n");
		if (choice == 1) setTextColor(RED); else setTextColor(BLACK);
		wprintf(L"\n                                                        <  Xem danh sách độc giả trong thư viện  >\n");
		if (choice == 2) setTextColor(RED); else setTextColor(BLACK);
		wprintf(L"\n                                                                   <  Thêm độc giả  >\n");
		if (choice == 3) setTextColor(RED); else setTextColor(BLACK);
		wprintf(L"\n                                                          <  Chỉnh sửa thông tin một độc giả  >\n");
		if (choice == 4) setTextColor(RED); else setTextColor(BLACK);
		wprintf(L"\n                                                          <  Tìm kiếm độc giả theo CMND/CCCD  >\n");
		if (choice == 5) setTextColor(RED); else setTextColor(BLACK);
		wprintf(L"\n                                                            <  Tìm kiếm độc giả theo họ tên  >\n");
		if (choice == 6) setTextColor(RED); else setTextColor(BLACK);
		wprintf(L"\n                                                                <  Gia hạn thẻ độc giả  >\n");
		if (user.role == ADMIN_ROLE || user.role == MANAGER_ROLE)
		{
			if (choice == 7) setTextColor(RED); else setTextColor(BLACK);
			wprintf(L"\n                                                             <  Xoá thông tin một độc giả  >\n");
		}
		setTextColor(BLACK);
		key = _getch();
		switch (key)
		{
		case KEY_UP:
			if (choice == MENU_MIN_OPTION && (user.role == ADMIN_ROLE || user.role == MANAGER_ROLE)) choice = MEMBER_MENU_MAX_OPTION;
			else if (choice == MENU_MIN_OPTION && user.role == SPECIALIST_ROLE) choice = MEMBER_MENU_MAX_SPECIALIST_OPTION; else choice--;
			goto memberMenu;
			break;
		case KEY_DOWN:
			if (choice == MEMBER_MENU_MAX_OPTION) choice = MENU_MIN_OPTION;
			else if (choice == MEMBER_MENU_MAX_SPECIALIST_OPTION && user.role == SPECIALIST_ROLE) choice = MENU_MIN_OPTION; else choice++;
			goto memberMenu;
			break;
		case KEY_ENTER:
			goto memberChooser;
			break;
		case ESC_KEY:
			return;
			break;
		default:
			goto memberMenu;
			break;
		}

	memberChooser:
		ShowConsoleCursor(true);
		switch (choice)
		{
		case 1:
			viewMembers(memList);
			break;
		case 2:
			addMember(memList);
			break;
		case 3:
			updateMember(memList);
			break;
		case 4:
			searchMemberByIDCard(memList);
			break;
		case 5:
			searchMemberByName(memList);
			break;
		case 6:
			cardExt(memList);
			break;
		case 7:
			if (user.role == ADMIN_ROLE || user.role == MANAGER_ROLE)
			{
				deleteMember(memList);
			}
			else wprintf(L"Chức năng không tồn tại hoặc bạn không có quyền sử dụng chức năng này!\n");
			break;
		case 0:
			return;
			break;
		default:
			wprintf(L"Chức năng không tồn tại hoặc bạn không có quyền sử dụng chức năng này!\n");
			break;
		}
	}
}