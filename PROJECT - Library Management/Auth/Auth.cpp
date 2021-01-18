#include "Auth.h"
const int base = 311;
const long long modulo = 1000000003;
long long hashGen(char pwd[])
{
	long long hash = 0;
	for (int i = 0; i < strlen(pwd); i++)
	{
		hash = (hash * base + pwd[i] - 'a' + 1) % modulo;
	}
	return hash;
}
bool userInfoInput(AuthFile& credentials, User*& user, int id, int role)
{
	handleChooser:
	system("CLS");
	writeProgName();
	setTextColor(RED);
	wprintf(L"                                                           --- Nhập Thông Tin Tài Khoản ---\n");
	setTextColor(BLACK);
	//Set id theo thu tu, role theo quyet dinh cua admin
	user->id = id;
	user->role = role;
	//Nhap cac field lien quan den thong tin tai khoan
	if (!secureInp(user->handle, MAX_USR_LEN, L"Tên tài khoản (tối đa 50 kí tự): ", ACCEPTED_CHARS_HANDLE)) return false;
	//Check handle xem co trung lap hay khong
	if (handleCheck(credentials.users, user->handle) != NULL)
	{
		setTextColor(RED);
		wprintf(L"Tên tài khoản bị trùng lặp!\n");
		setTextColor(BLACK);
		wprintf(L"Nhấn enter để chọn lại tên tài khoản khác");
		dustIgnore();
		goto handleChooser;
	}
	char password[MAX_PWD_LEN];
	pwdRetype:
	if (!securePassword(password, MAX_PWD_LEN, L"Mật khẩu (tối đa 50 kí tự): ")) return false;
	//Hash
	user->hash = hashGen(password);
	if (!securePassword(password, MAX_PWD_LEN, L"Xác nhận mật khẩu: ")) return false;
	if (hashGen(password) != user->hash)
	{
		for (int i = 2; i >= 1; i--)
		{
			wprintf(L"\033[F\33[2K");
			if (!securePassword(password, MAX_PWD_LEN, L"\x1b[38;2;247;30;112mMật khẩu không trùng khớp.\x1b[38;2;0;0;0m Xác nhận lại mật khẩu: ")) return false;
			if (hashGen(password) == user->hash) break;
			if (i == 1)
			{
				wprintf(L"\x1b[38;2;247;30;112mXác nhận mật khẩu không thành công\x1b[38;2;0;0;0m Nhấn enter để đặt lại mật khẩu mới và xác nhận lại mật khẩu\n");
				dustIgnore();
				wprintf(L"\033[F\33[2K\033[F\33[2K\033[F\33[2K");
				goto pwdRetype;
			}
		}
	}
	if (!secureInp(user->fullName, MAX_NAME_LEN, L"Họ tên (tối đa 50 kí tự): ", ACCEPTED_CHARS_NAME)) return false;
	if (!secureInp(user->address, MAX_NAME_LEN, L"Địa chỉ (tối đa 100 kí tự): ", ACCEPTED_CHARS_ADDRESS)) return false;
	caesarEncrypt(user->address);
	char tmp[5];
	//Doc ngay sinh
	setTextColor(PURPLE);
	wprintf(L"Ngày sinh:\n");
	if (!secureInp(tmp, 3, L"    Ngày: ", ACCEPTED_CHARS_NUMERIC)) return false;
	user->birthday.day = atoi(tmp);
	if (!secureInp(tmp, 3, L"    Tháng: ", ACCEPTED_CHARS_NUMERIC)) return false;
	user->birthday.month = atoi(tmp);
	if (!secureInp(tmp, 5, L"    Năm: ", ACCEPTED_CHARS_NUMERIC)) return false;
	user->birthday.year = atoi(tmp);
	while (user->birthday.day < 1 || 
		user->birthday.day>31 || 
		user->birthday.year < 0 || 
		user->birthday.month < 1 ||
		user->birthday.month > 12 ||
		(user->birthday.month == 2 && user->birthday.day > 28 && user->birthday.year % 4 != 0) ||
		(user->birthday.month == 2 && user->birthday.year % 4 == 0 && user->birthday.day > 29) || 
		(user->birthday.day>30&&(user->birthday.month==4|| user->birthday.month == 6|| user->birthday.month == 9|| user->birthday.month == 11)))
	{
		wprintf(L"\033[F\33[2K\033[F\33[2K\033[F\33[2K\033[F\33[2K");
		setTextColor(PURPLE);
		wprintf(L"\x1b[38;2;247;30;112mNgày sinh không hợp lệ! \x1b[38;2;0;0;0mNhập lại ngày sinh:\n");
		if (!secureInp(tmp, 3, L"    Ngày: ", ACCEPTED_CHARS_NUMERIC)) return false;
		user->birthday.day = atoi(tmp);
		if (!secureInp(tmp, 3, L"    Tháng: ", ACCEPTED_CHARS_NUMERIC)) return false;
		user->birthday.month = atoi(tmp);
		if (!secureInp(tmp, 5, L"    Năm: ", ACCEPTED_CHARS_NUMERIC)) return false;
		user->birthday.year = atoi(tmp);
	}
	if (!secureInp(user->idCard, MAX_IDCARD_LEN, L"CMND/CCCD (9 hoặc 12 chữ số): ", ACCEPTED_CHARS_NUMERIC)) return false;
	//Check trung lap IDCard
	while (userIdCardCheck(credentials.users, user->idCard) != NULL || (strlen(user->idCard) != 9 && strlen(user->idCard) != 12))
	{
		wprintf(L"\033[F\33[2K\r");
		if (!secureInp(user->idCard, MAX_IDCARD_LEN, L"\x1b[38;2;247;30;112mCMND/CCCD bị trùng lặp hoặc không hợp lệ!\x1b[38;2;0;0;0m Nhập lại CMND/CCCD: ", ACCEPTED_CHARS_NUMERIC)) return false;
	}
	//Ma hoa IDCard
	caesarEncrypt(user->idCard);
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
	user->gender = choice;
	//Chon trang thai tai khoan
	choice = S_ACTIVATED;
	setTextColor(PURPLE);
	wprintf(L"Trạng thái tài khoản:\n");
	statusChooser:
	if (choice == S_ACTIVATED) setTextColor(RED); else setTextColor(BLACK);
	wprintf(L"   > Activated\n");
	if (choice == S_BLOCKED) setTextColor(RED); else setTextColor(BLACK);
	wprintf(L"   > Blocked\n");
	setTextColor(BLACK);
	key = _getch();
	switch (key)
	{
	case KEY_UP:
		if (choice == 0) choice = 1; else choice = 0;
		wprintf(L"\033[F\033[F");
		goto statusChooser;
		break;
	case KEY_DOWN:
		if (choice == 0) choice = 1; else choice = 0;
		wprintf(L"\033[F\033[F");
		goto statusChooser;
		break;
	case ESC_KEY:
		return false;
		break;
	case KEY_ENTER:
		break;
	default:
		wprintf(L"\033[F\033[F");
		goto statusChooser;
		break;
	}
	user->status = choice;
	return true;
}
void getCredentials(AuthFile& credentials)
{
	//Doc va kiem tra file
	FILE* f = fopen(USER_CREDENTIALS_FILE, "r+b");
	if (f == NULL)
	{
		if (_mkdir("Data") == 0) wprintf(L"Đã tạo thư mục Data");
		if (fopen(USER_CREDENTIALS_FILE, "wb") == NULL)
		{
			setTextColor(RED);
			wprintf(L"Không thể mở file dữ liệu thông tin tài khoản\n");
			setTextColor(BLACK);
			exit(1);
		}
		f = fopen(USER_CREDENTIALS_FILE, "r+b");
	}
	//Init size
	credentials.size = 0;
	if (fread(&credentials.size, sizeof(credentials.size), 1, f) != 1)
	{
		system("CLS");
		User* admin = new User;
		if (admin == NULL)
		{
			setTextColor(RED);
			wprintf(L"\nKhông đủ vùng nhớ để chương trình hoạt động\n");
			setTextColor(BLACK);
		}
		credentials.size = 1;
		writeProgName();
		setTextColor(RED); 
		wprintf(L"                                                               === THIẾT LẬP PHẦN MỀM ===\n");
		setTextColor(PURPLE);
		wprintf(L"\n\n         Đây là lần đầu tiên bạn sử dụng phần mềm, bạn cần tạo tài khoản admin!\n         Nhấn enter để tạo tài khoản admin mới\n");
		setTextColor(BLACK);
		dustIgnore();
		while (!userInfoInput(credentials, admin, 0, ADMIN_ROLE) || (!confBox()));
		if (fwrite(&credentials.size, sizeof(credentials.size), 1, f) != 1 || fwrite(admin, sizeof(*admin), 1, f) != 1)
		{
			setTextColor(RED);
			wprintf(L"Không thể ghi thông tin tài khoản admin\n");
			setTextColor(BLACK);
			wprintf(L"Nhấn enter để tiếp tục\n");
			dustIgnore();
			exit(1);
		} 
		else
		{
			setTextColor(GREEN);
			wprintf(L"Tạo tài khoản admin thành công!\n");
			setTextColor(BLACK);
			wprintf(L"Nhấn enter để tiếp tục\n");
			dustIgnore();
		}
		fseek(f, sizeof(credentials.size), SEEK_SET);
	}
	//Neu co list cu thi xoa, update list moi
	if (credentials.users.head != NULL)
	{
		deleteList(credentials.users);
	}
	User* buffer;
	//Doc list len tu file
	for (int i = 0; i < credentials.size; i++)
	{
		buffer = new User;
		if (buffer == NULL)
		{
			setTextColor(RED);
			wprintf(L"Không đủ bộ nhớ để đọc dữ liệu người dùng!\n");
			exit(1);
		}
		if (fread(buffer, sizeof(User), 1, f) != 1)
		{
			setTextColor(RED);
			wprintf(L"Không thể đọc dữ liệu người dùng!\n");
			exit(1);
		}
		if (appendList(credentials.users, buffer) == NULL)
		{
			setTextColor(RED);
			wprintf(L"Không thể đọc dữ liệu người dùng!\n");
			exit(1);
		}
	}
	fclose(f);
}
void updateAccountStatus(User& user, int status)
{
	//Doc va kiem tra file
	FILE* f = fopen(USER_CREDENTIALS_FILE, "r+b");
	if (f == NULL)
	{
		setTextColor(RED);
		wprintf(L"Không thể mở file thông tin người dùng\n");
		wprintf(L"Không thể đổi mật khẩu!\n");
		setTextColor(BLACK);
		wprintf(L"Nhấn enter để quay lại menu");
		dustIgnore();
		return;
	}
	//Seek den vi tri chua thong tin user tuong ung trong file
	fseek(f, user.id * sizeof(User) + sizeof(int), SEEK_SET);
	user.status = status;
	if (fwrite(&user, sizeof(User), 1, f) != 1)
	{
		setTextColor(RED);
		wprintf(L"Không thể cập nhật file thông tin tài khoản\n");
		fclose(f);
		return;
	}
	fclose(f);
}
void updateStatusChooser(AuthFile credentials)
{
	//Init lua chon dau tien
	int choice = S_ACTIVATED, key;
	system("CLS");
	writeProgName();
	setTextColor(RED);
	wprintf(L"                                                           --- Cập Nhật Trạng Thái Tài Khoản ---\n");
	setTextColor(BLACK);
	//Neu chi co 1 user la admin thi khong con user nao de cap nhat trang thai
	if (credentials.size == 1)
	{
		setTextColor(RED);
		wprintf(L"Không có tài khoản nào để cập nhật quyền\n");
		setTextColor(BLACK);
		wprintf(L"Nhấn enter để quay lại menu chính\n");
		dustIgnore();
		return;
	}
	char handle[MAX_USR_LEN];
	if (!secureInp(handle, MAX_USR_LEN, L"Nhập tên đăng nhập của tài khoản cần cập nhật trạng thái: ", ACCEPTED_CHARS_HANDLE)) return;
	for (Node* currentNode = credentials.users.head; currentNode; currentNode = currentNode->next)
	{
		if (strcmp(handle, ((User*)currentNode->data)->handle) == 0 && ((User*)currentNode->data)->id != 0)
		{
			setTextColor(YELLOW);
			wprintf(L"Chọn tác vụ bạn muốn thực hiện:\n");
			taskChooser:
			if (choice == S_ACTIVATED) setTextColor(RED); else setTextColor(BLACK);
			wprintf(L"   >  Kích hoạt tài khoản\n");
			if (choice == S_BLOCKED) setTextColor(RED); else setTextColor(BLACK);
			wprintf(L"   >  Khoá tài khoản\n");
			setTextColor(BLACK);
			key = _getch();
			switch (key)
			{
				case KEY_UP:
					if (choice == S_BLOCKED) choice = S_ACTIVATED; else choice = S_BLOCKED;
					wprintf(L"\033[F\033[F");
					goto taskChooser;
					break;
				case KEY_DOWN:
					if (choice == S_BLOCKED) choice = S_ACTIVATED; else choice = S_BLOCKED;
					wprintf(L"\033[F\033[F");
					goto taskChooser;
					break;
				case KEY_ENTER:
					updateAccountStatus(*((User*)currentNode->data), choice);
					setTextColor(GREEN);
					wprintf(L"Đã cập nhật trạng thái tài khoản\n");
					dustIgnore();
					return;
					break;
				case ESC_KEY:
					return;
					break;
				default:
					wprintf(L"\033[F\033[F");
					goto taskChooser;
					break;
			}
		}
	}
	setTextColor(RED);
	wprintf(L"Không tìm thấy tài khoản với tên đăng nhập bạn đã chọn hoặc tài khoản này không thể cập nhật trạng thái\n");
	setTextColor(BLACK);
	wprintf(L"Nhấn enter để quay lại menu");
	dustIgnore();
}
void signIn(AuthFile credentials, User& thisSessionUser)
{
	char handle[MAX_USR_LEN], password[MAX_PWD_LEN];
	system("CLS");
	writeProgName();
	setTextColor(RED); 
	wprintf(L"                                                                    --- Đăng nhập ---\n\n\n\n\n\n");
	setTextColor(BLACK);
	if (!secureInp(handle, MAX_USR_LEN, L"                                           Tên đăng nhập: ", ACCEPTED_CHARS_HANDLE)) return;
	if (!securePassword(password, MAX_PWD_LEN, L"                                           Mật khẩu: ")) return;
	for (Node* currentNode = credentials.users.head; currentNode; currentNode = currentNode->next)
		//check handle
		if (strcmp(handle, ((User*)(currentNode->data))->handle) == 0)
		{
			//check availability
			if (((User*)(currentNode->data))->status == S_ACTIVATED)
			{
				//check password
				if (hashGen(password) == ((User*)(currentNode->data))->hash)
				{
					thisSessionUser = *(User*)(currentNode->data);
					setTextColor(GREEN);
					wprintf(L"                                           Đăng nhập thành công!\n");
					setTextColor(BLACK);
					wprintf(L"                                           Nhấn enter để quay lại menu");
					dustIgnore();
					return;
				}
				else
				{
					// 3 lan thu neu nhap sai password
					for (int j = 2; j >= 1; j--)
					{
						wprintf(L"                                           Bạn đã nhập sai mật khẩu, còn %d lần thử\n", j);
						if (!securePassword(password, MAX_PWD_LEN, L"                                           Mật khẩu: ")) return;
						if (hashGen(password) == ((User*)(currentNode->data))->hash)
						{
							thisSessionUser = *(User*)(currentNode->data);
							setTextColor(GREEN);
							wprintf(L"                                           Đăng nhập thành công!\n");
							setTextColor(BLACK);
							wprintf(L"                                           Nhấn enter để quay lại menu");
							dustIgnore();
							return;
						}
					}
					//Neu sai password 3 lan ma ko phai admin thi khoa tai khoan
					if (((User*)currentNode->data)->id != 0)
					{
						setTextColor(RED);
						wprintf(L"  \x1b[38;2;247;30;112mBạn đã nhập sai mật khẩu 3 lần\n  \x1b[38;2;0;0;0mTài khoản của bạn đã bị khoá, liên hệ admin để kích hoạt lại tài khoản!\n");
						updateAccountStatus(*((User*)currentNode->data), S_BLOCKED);
					}
					else
					{
						setTextColor(RED);
						wprintf(L"                                           \x1b[38;2;247;30;112mBạn đã nhập sai mật khẩu 3 lần\n                                           \x1b[38;2;0;0;0mBuộc dừng chương trình\n");
						exit(0);
					}
					setTextColor(BLACK);
					dustIgnore();
					return;
				}
			}
			else
			{
				setTextColor(RED);
				wprintf(L"                                           Tài khoản của bạn đã bị khoá, liên hệ admin để kích hoạt lại tài khoản!\n");
				setTextColor(BLACK);
				dustIgnore();
				return;
			}
		}
	setTextColor(RED);
	wprintf(L"                                           Bạn đã nhập sai tài khoản hoặc mật khẩu\n");
	setTextColor(BLACK);
	dustIgnore();
	return;
}
void signOut(User& user)
{
	//reset id ve default
	user.id = -1;
}
void changePassword(AuthFile& credentials, User& user, bool force)
{
	//Doc va kiem tra file
	FILE* f = fopen(USER_CREDENTIALS_FILE, "r+b");
	if (f == NULL)
	{
		setTextColor(RED);
		wprintf(L"Không thể mở file dữ liệu thông tin tài khoản\n");
		wprintf(L"Không thể đổi mật khẩu!\n");
		setTextColor(BLACK);
		wprintf(L"Nhấn enter để quay lại menu");
		dustIgnore();
		return;
	}
	//Seek den vi tri cua user trong file
	fseek(f, user.id * sizeof(User) + sizeof(int), SEEK_SET);
	system("CLS");
	writeProgName();
	setTextColor(RED); 
	wprintf(L"                                                              --- Thay Đổi Mật Khẩu ---\n");
	setTextColor(BLACK);
	char password[MAX_PWD_LEN];
	//check quyen doi password co phai quyen cao nhat cua admin hay khong
	if (force == 0)
	{
		//neu khong thi can nhap mat khau cu de thay doi
		if (!securePassword(password, MAX_PWD_LEN, L"                                           Nhập mật khẩu cũ: ")) return;
		//check pwd
		if (hashGen(password) != user.hash)
		{
			for (int i = 2; i >= 1; i--)
			{
				wprintf(L"                                           Bạn đã nhập sai mật khẩu, còn %d lần thử\n", i);
				if (!securePassword(password, MAX_PWD_LEN, L"                                           Nhập lại mật khẩu cũ: ")) return;
				if (hashGen(password) == user.hash)
				{
					char newPass[MAX_PWD_LEN];
					if (!securePassword(newPass, MAX_PWD_LEN, L"                                           Nhập mật khẩu mới (tối đa 50 kí tự): ")) return;
					if (!securePassword(password, MAX_PWD_LEN, L"                                           Xác nhận mật khẩu mới: ")) return;
					if (hashGen(password) == hashGen(newPass))
					{
						user.hash = hashGen(newPass);
						if (fwrite(&user, sizeof(User), 1, f) == 1)
						{
							setTextColor(GREEN);
							accessCredData(user.id, hash) = user.hash;
							wprintf(L"                                           Thay đổi mật khẩu thành công!\n");
							setTextColor(BLACK);
						}
						else
						{
							setTextColor(RED);
							wprintf(L"                                           Không thể ghi mật khẩu mới vào file dữ liệu tài khoản\n");
							setTextColor(BLACK);
							wprintf(L"                                           Nhấn enter để trở về menu\n");
						}
						fclose(f);
						dustIgnore();
						return;
					}
					else
					{
						setTextColor(RED);
						wprintf(L"                                           Xác nhận mật khẩu không trùng khớp với mật khẩu mới\n");
						setTextColor(BLACK);
						wprintf(L"                                           Nhấn enter để trở về menu\n");
						dustIgnore();
						return;
					}
				}
			}
			setTextColor(RED);
			wprintf(L"                                           Bạn đã nhập sai mật khẩu 3 lần.\n                                           Bạn bị đăng xuất và khoá tài khoản, liên hệ admin để kích hoạt lại tài khoản\n");
			if (user.id != 0)
			{
				updateAccountStatus(user, S_BLOCKED);
				signOut(user);
			}
			setTextColor(BLACK);
			dustIgnore();
			return;
		}
		else
		{
			char newPass[MAX_PWD_LEN];
			if (!securePassword(newPass, MAX_PWD_LEN, L"                                           Nhập mật khẩu mới (tối đa 50 kí tự): ")) return;
			if (!securePassword(password, MAX_PWD_LEN, L"                                           Xác nhận mật khẩu mới: ")) return;
			if (hashGen(password) == hashGen(newPass))
			{
				user.hash = hashGen(newPass);
				if (fwrite(&user, sizeof(User), 1, f) == 1)
				{
					setTextColor(GREEN);
					accessCredData(user.id, hash) = user.hash;
					wprintf(L"                                           Thay đổi mật khẩu thành công!\n");
					setTextColor(BLACK);
					wprintf(L"                                           Nhấn enter để trở về menu\n");
				}
				else
				{
					setTextColor(RED);
					wprintf(L"                                           Không thể ghi mật khẩu mới vào file dữ liệu tài khoản\n");
					setTextColor(BLACK);
					wprintf(L"                                           Nhấn enter để trở về menu\n");
				}
				dustIgnore();
				fclose(f);
				return;
			}
			else
			{
				setTextColor(RED);
				wprintf(L"                                           Xác nhận mật khẩu không trùng khớp với mật khẩu mới\n");
				setTextColor(BLACK);
				wprintf(L"                                           Nhấn enter để trở về menu\n");
				dustIgnore();
				return;
			}
		}
	}
	else
	{
		char newPass[MAX_PWD_LEN];
		if (!securePassword(newPass, MAX_PWD_LEN, L"                                           Nhập mật khẩu mới (tối đa 50 kí tự): ")) return;
		if (!securePassword(password, MAX_PWD_LEN, L"                                           Xác nhận mật khẩu mới: ")) return;
		if (hashGen(password) == hashGen(password))
		{
			user.hash = hashGen(newPass);
			if (fwrite(&user, sizeof(User), 1, f) == 1)
			{
				setTextColor(GREEN);
				wprintf(L"                                           Thay đổi mật khẩu thành công!\n");
				setTextColor(BLACK);
			}
			else
			{
				setTextColor(RED);
				wprintf(L"Không thể ghi mật khẩu mới vào file dữ liệu tài khoản\n");
				setTextColor(BLACK);
				wprintf(L"Nhấn enter để trở về menu\n");
			}
			dustIgnore();
			fclose(f);
			return;
		}
		else
		{
			setTextColor(RED);
			wprintf(L"Xác nhận mật khẩu không trùng khớp với mật khẩu mới\n");
			setTextColor(BLACK);
			wprintf(L"Nhấn enter để trở về menu\n");
			dustIgnore();
			return;
		}
	}
}
void accountInfoUpdateChooser(AuthFile& credentials, User& user)
{
	int key, choice = 0;
	
	system("CLS");
	writeProgName();
	setTextColor(RED);
	wprintf(L"                                                             --- Cập Nhật Thông Tin Cá Nhân ---\n");
	setTextColor(BLACK);
	wprintf(L"Chọn người dùng bạn muốn cập nhật thông tin cá nhân: \n");
functionChooser:
	if (choice == 0) setTextColor(RED); else setTextColor(BLACK);
	wprintf(L"   >  Chính mình\n");
	if (choice == 1) setTextColor(RED); else setTextColor(BLACK);
	wprintf(L"   >  Người dùng khác\n");
	setTextColor(BLACK);
	key = _getch();
	switch (key)
	{
	case KEY_UP:
		if (choice == 0) choice = 1; else choice = 0;
		wprintf(L"\033[F\33[2K\033[F\33[2K");
		goto functionChooser;
		break;
	case KEY_DOWN:
		if (choice == 0) choice = 1; else choice = 0;
		wprintf(L"\033[F\33[2K\033[F\33[2K");
		goto functionChooser;
		break;
	case ESC_KEY:
		return; 
		break;
	case KEY_ENTER:
		if (choice == 0)
		{
			accountInfoUpdate(credentials, user);
			User* updateNode = (User*)accessCredNode(user.id)->data;
			strcpy(updateNode->fullName, user.fullName);
			strcpy(updateNode->address, user.address);
			strcpy(updateNode->idCard, user.idCard);
			updateNode->gender = user.gender;
			updateNode->birthday = user.birthday;
			return;
		}
		else goto accountChooser;
		break;
	default:
		wprintf(L"\033[F\33[2K\033[F\33[2K");
		goto functionChooser;
		break;
	}
	accountChooser:
	system("CLS");
	writeProgName();
	setTextColor(YELLOW);
	wprintf(L"                                                             --- Cập Nhật Thông Tin Cá Nhân ---\n");
	setTextColor(BLACK);
	if (credentials.size == 1)
	{
		setTextColor(RED);
		wprintf(L"Không có tài khoản nào để cập nhật thông tin cá nhân\n");
		setTextColor(BLACK);
		wprintf(L"Nhấn enter để quay lại menu\n");
		dustIgnore();
		return;
	}
	char chosenHandle[MAX_USR_LEN];
	if (!secureInp(chosenHandle, MAX_USR_LEN, L"Nhập tên tài khoản bạn muốn cập nhật thông tin cá nhân: ", ACCEPTED_CHARS_HANDLE)) return;
	Node* currentNode = accessCredNode(1);
	choice = -1;
	for (int i = 1; i < credentials.size; i++)
	{
		if (strcmp(((User*)currentNode->data)->handle, chosenHandle) == 0)
		{
			choice = ((User*)currentNode->data)->id;
			break;
		}
		currentNode = currentNode->next;
	}
	if (choice != -1)
	{
		User* updateNode = (User*)accessCredNode(choice)->data;
		accountInfoUpdate(credentials, *updateNode);
		if (choice == user.id)
		{
			strcpy(user.fullName, updateNode->fullName);
			strcpy(user.address, updateNode->address);
			strcpy(user.idCard, updateNode->idCard);
			user.gender = updateNode->gender;
			user.birthday = updateNode->birthday;
		}
	}
	else
	{
		setTextColor(RED);
		wprintf(L"Tài khoản bạn đã nhập không tồn tại\n");
		dustIgnore();
	}
}
void accountInfoUpdate(AuthFile& credentials, User& userBase)
{
	User user = userBase;
	FILE* f = fopen(USER_CREDENTIALS_FILE, "r+b");
	if (f == NULL)
	{
		setTextColor(RED);
		wprintf(L"Không thể mở file dữ liệu thông tin tài khoản\n");
		wprintf(L"Không thể cập nhật thông tin tài khoản!\n");
		setTextColor(BLACK);
		wprintf(L"Nhấn enter để trở về menu");
		dustIgnore();
		return;
	}
	//Seek con tro tep tin den vi tri chua user co id tuong ung
	fseek(f, user.id * sizeof(User) + sizeof(int), SEEK_SET);
	system("CLS");
	writeProgName();
	setTextColor(RED); 
	wprintf(L"                                                          --- Cập Nhật Thông Tin Tài Khoản ---\n");
	setTextColor(YELLOW);
	wprintf(L"Nhập thông tin mới: \n");
	setTextColor(BLACK);
	if (!secureInp(user.fullName, MAX_NAME_LEN, L"  Họ tên (tối đa 50 kí tự):  ", ACCEPTED_CHARS_NAME)) return;
	if (!secureInp(user.address, MAX_ADD_LEN, L"  Địa chỉ (tối đa 100 kí tự): ", ACCEPTED_CHARS_ADDRESS)) return;
	caesarEncrypt(user.address);
	char tmp[5];
	setTextColor(PURPLE);
	wprintf(L"  Ngày sinh:\n");
	if (!secureInp(tmp, 3, L"    Ngày: ", ACCEPTED_CHARS_NUMERIC)) return;
	user.birthday.day = atoi(tmp);
	if (!secureInp(tmp, 3, L"    Tháng: ", ACCEPTED_CHARS_NUMERIC)) return;
	user.birthday.month = atoi(tmp);
	if (!secureInp(tmp, 5, L"    Năm: ", ACCEPTED_CHARS_NUMERIC)) return;
	user.birthday.year = atoi(tmp);
	while (user.birthday.day < 1 ||
		user.birthday.day>31 ||
		user.birthday.year < 0 ||
		user.birthday.month < 1 ||
		user.birthday.month>12 ||
		(user.birthday.month == 2 && user.birthday.day > 28 && user.birthday.year % 4 != 0) ||
		(user.birthday.month == 2 && user.birthday.year % 4 == 0 && user.birthday.day > 29) ||
		(user.birthday.day > 30 && (user.birthday.month == 4 || user.birthday.month == 6 || user.birthday.month == 9 || user.birthday.month == 11)))
	{
		wprintf(L"\033[F\33[2K\033[F\33[2K\033[F\33[2K\033[F\33[2K");
		setTextColor(PURPLE);
		wprintf(L"\x1b[38;2;247;30;112mNgày sinh không hợp lệ! \x1b[38;2;0;0;0mNhập lại ngày sinh:\n");
		if (!secureInp(tmp, 3, L"    Ngày: ", ACCEPTED_CHARS_NUMERIC)) return;
		user.birthday.day = atoi(tmp);
		if (!secureInp(tmp, 3, L"    Tháng: ", ACCEPTED_CHARS_NUMERIC)) return;
		user.birthday.month = atoi(tmp);
		if (!secureInp(tmp, 5, L"    Năm: ", ACCEPTED_CHARS_NUMERIC)) return;
		user.birthday.year = atoi(tmp);
	}
	char prevId[MAX_IDCARD_LEN];
	strcpy(prevId, user.idCard);
	caesarDecrypt(prevId);
	if (!secureInp(user.idCard, MAX_IDCARD_LEN, L"CMND/CCCD (9 hoặc 12 chữ số): ", ACCEPTED_CHARS_NUMERIC)) return;
	if (strcmp(prevId, user.idCard) != 0)
	while (userIdCardCheck(credentials.users, user.idCard) != NULL || (strlen(user.idCard) != 9 && strlen(user.idCard) != 12))
	{
		wprintf(L"\033[F\33[2K\r");
		if (!secureInp(user.idCard, MAX_IDCARD_LEN, L"\x1b[38;2;247;30;112mCMND/CCCD bị trùng lặp hoặc không hợp lệ!\x1b[38;2;0;0;0m Nhập lại CMND/CCCD: ", ACCEPTED_CHARS_NUMERIC)) return;
		if (strcmp(prevId, user.idCard) == 0) break;
	}
	caesarEncrypt(user.idCard);
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
		return;
		break;
	case KEY_ENTER:
		break;
	default:
		wprintf(L"\033[F\033[F");
		goto genderChooser;
		break;
	}
	user.gender = choice;
	//Chon trang thai tai khoan
	choice = S_ACTIVATED;
	setTextColor(PURPLE);
	wprintf(L"Trạng thái tài khoản:\n");
statusChooser:
	if (choice == S_ACTIVATED) setTextColor(RED); else setTextColor(BLACK);
	wprintf(L"   > Activated\n");
	if (choice == S_BLOCKED) setTextColor(RED); else setTextColor(BLACK);
	wprintf(L"   > Blocked\n");
	setTextColor(BLACK);
	key = _getch();
	switch (key)
	{
	case KEY_UP:
		if (choice == 0) choice = 1; else choice = 0;
		wprintf(L"\033[F\033[F");
		goto statusChooser;
		break;
	case KEY_DOWN:
		if (choice == 0) choice = 1; else choice = 0;
		wprintf(L"\033[F\033[F");
		goto statusChooser;
		break;
	case ESC_KEY:
		return;
		break;
	case KEY_ENTER:
		break;
	default:
		wprintf(L"\033[F\033[F");
		goto statusChooser;
		break;
	}
	user.status = choice;
	if (!confBox()) return;
	//Ghi cau truc User vao file
	if (fwrite(&user, sizeof(User), 1, f) == 1)
	{
		setTextColor(GREEN);
		wprintf(L"Cập nhật thông tin tài khoản thành công!\n");
		setTextColor(BLACK);
		userBase = user;
		fclose(f);
		dustIgnore();
		return;
	}
	else
	{
		setTextColor(RED);
		wprintf(L"Không thể ghi thông tin tài khoản vào file dữ liệu\nKhông thể cập nhật thông tin tài khoản\n");
		setTextColor(BLACK);
		fclose(f);
		dustIgnore();
		return;
	}
}
void createAccount(AuthFile& credentials)
{
	int newRole = MANAGER_ROLE, key;
	system("CLS");
	User* newUser = new User;
	if (newUser == NULL)
	{
		setTextColor(RED);
		wprintf(L"\nKhông đủ vùng nhớ để chương trình hoạt động\n");
		exit(1);
	}
	writeProgName();
	setTextColor(RED);
	wprintf(L"                                                              --- Tạo Tài Khoản Mới ---\n");
	setTextColor(BLACK);
	wprintf(L"Chọn quyền cho tài khoản này: \n");
roleChooser:
	if (newRole == MANAGER_ROLE) setTextColor(RED); else setTextColor(BLACK);
	wprintf(L"   >  Quản Lý\n");
	if (newRole == SPECIALIST_ROLE) setTextColor(RED); else setTextColor(BLACK);
	wprintf(L"   >  Chuyên Viên\n");
	setTextColor(BLACK);
	key = _getch();
	switch (key)
	{
		case KEY_UP:
			if (newRole == MANAGER_ROLE) newRole = SPECIALIST_ROLE; else newRole = MANAGER_ROLE;
			wprintf(L"\033[F\033[F");
			goto roleChooser;
			break;
		case KEY_DOWN:
			if (newRole == MANAGER_ROLE) newRole = SPECIALIST_ROLE; else newRole = MANAGER_ROLE;
			wprintf(L"\033[F\033[F");
			goto roleChooser;
			break;
		case KEY_ENTER:
			goto createUser;
			break;
		case ESC_KEY:
			return; 
			break;
		default:
			wprintf(L"\033[F\033[F");
			goto roleChooser;
			break;
	}
createUser:
	userInfoInput(credentials, newUser, credentials.size, newRole);
	if (!confBox()) return;
	credentials.size++;
	FILE* f = fopen(USER_CREDENTIALS_FILE, "r+b");
	if (f == NULL)
	{
		setTextColor(RED);
		wprintf(L"Không thể mở file dữ liệu thông tin tài khoản\n");
		wprintf(L"Không thể thêm tài khoản\n");
		setTextColor(BLACK);
		wprintf(L"Nhấn enter để quay lại menu");
		dustIgnore();
		return;
	}
	if (fwrite(&credentials.size, sizeof(credentials.size), 1, f) != 1)
	{
		setTextColor(RED);
		wprintf(L"Không thể cập nhật file dữ liệu tài khoản\n");
		setTextColor(BLACK);
		wprintf(L"Nhấn enter để quay lại menu");
		fclose(f);
		dustIgnore();
		return;
	}
	fseek(f, 0, SEEK_END);
	if (fwrite(newUser, sizeof(User), 1, f) == 1)
	{
		if (appendList(credentials.users, newUser) == NULL) wprintf(L"Không đủ bộ nhớ để chương trình hoạt động\n");
		setTextColor(GREEN);
		wprintf(L"Thêm tài khoản thành công!\n");
		setTextColor(BLACK);
		fclose(f);
		wprintf(L"Nhấn enter để quay lại menu");
		dustIgnore();
		return;
	}
	else
	{
		setTextColor(RED);
		credentials.size--;
		fwrite(&credentials.size, sizeof(credentials.size), 1, f);
		wprintf(L"Không thể cập nhật file dữ liệu tài khoản\nThêm tài khoản không thành công\n");
		setTextColor(BLACK);
		fclose(f);
		wprintf(L"Nhấn enter để quay lại menu");
		dustIgnore();
		return;
	}
}
void updateRole(AuthFile& credentials)
{
	int choice = 1, key, newRole = MANAGER_ROLE;
	accountChooser:
	system("CLS");
	writeProgName();
	setTextColor(YELLOW); 
	wprintf(L"                                                           --- Cập Nhật Quyền Tài Khoản ---\n");
	setTextColor(BLACK);
	if (credentials.size == 1)
	{
		setTextColor(RED);
		wprintf(L"Không có tài khoản nào để cập nhật quyền\n");
		setTextColor(BLACK);
		wprintf(L"Nhấn enter để quay lại menu chính\n");
		dustIgnore();
		return;
	}
	char chosenHandle[MAX_USR_LEN];
	if (!secureInp(chosenHandle, MAX_USR_LEN, L"Nhập tên tài khoản bạn muốn cập nhật quyền: ", ACCEPTED_CHARS_HANDLE)) return;
	Node* cNode = accessCredNode(1);
	choice = -1;
	for (int i = 1; i < credentials.size; i++)
	{
		if (strcmp(((User*)cNode->data)->handle, chosenHandle) == 0)
		{
			choice = ((User*)cNode->data)->id;
			break;
		}
		cNode = cNode->next;
	}
	if (choice == -1)
	{
		setTextColor(RED);
		wprintf(L"Tài khoản bạn đã nhập không tồn tại\n");
		dustIgnore();
		return;
	}
	system("CLS");
	writeProgName();
	setTextColor(YELLOW); 
	wprintf(L"                                                           --- Cập Nhật Quyền Tài Khoản ---\n");
	setTextColor(BLACK);
	wprintf(L"Tài khoản đã chọn: ");
	setTextColor(YELLOW);
	User* currentNode = (User*)accessCredNode(choice)->data;
	if (currentNode->role == MANAGER_ROLE)
	{
		wprintf(L"Quản Lý - Tên tài khoản: %S", currentNode->handle);
	}
	else if (currentNode->role == SPECIALIST_ROLE)
	{
		wprintf(L"Chuyên Viên - Tên tài khoản: %S", currentNode->handle);
	}
	setTextColor(BLACK);
	wprintf(L"\nChọn quyền muốn cập nhật cho tài khoản đã chọn: \n");
	roleChooser:
	if (newRole == MANAGER_ROLE) setTextColor(RED); else setTextColor(BLACK);
	wprintf(L"   >  Quản Lý\n");
	if (newRole == SPECIALIST_ROLE) setTextColor(RED); else setTextColor(BLACK);
	wprintf(L"   >  Chuyên Viên\n");
	setTextColor(BLACK);
	key = _getch();
	switch (key)
	{
		case KEY_UP:
			if (newRole == MANAGER_ROLE) newRole = SPECIALIST_ROLE; else newRole = MANAGER_ROLE;
			wprintf(L"\033[F\33[2K\033[F\33[2K");
			goto roleChooser;
			break;
		case KEY_DOWN:
			if (newRole == MANAGER_ROLE) newRole = SPECIALIST_ROLE; else newRole = MANAGER_ROLE;
			wprintf(L"\033[F\33[2K\033[F\33[2K");
			goto roleChooser;
			break;
		case KEY_ENTER:
			goto roleUpdate;
			break;
		case ESC_KEY:
			return;
			break;
		default:
			wprintf(L"\033[F\33[2K\033[F\33[2K");
			goto roleChooser;
			break;
	}
	roleUpdate:
	//Cap nhat quyen cho tai khoan co ID tuong ung
	if (!confBox()) return;
	currentNode->role = newRole;
	FILE* f = fopen(USER_CREDENTIALS_FILE, "r+b");
	if (f == NULL)
	{
		setTextColor(RED);
		wprintf(L"Không thể mở file dữ liệu thông tin tài khoản\n");
		wprintf(L"Không thể cập nhật quyền tài khoản\n");
		setTextColor(BLACK);
		wprintf(L"Nhấn enter để quay lại menu");
		dustIgnore();
		return;
	}
	//Seek den vi tri cua user co ID tuong ung
	fseek(f, sizeof(credentials.size) + sizeof(User) * choice, SEEK_SET);
	if (fwrite(currentNode, sizeof(User), 1, f) == 1)
	{
		setTextColor(GREEN);
		wprintf(L"Cập nhật quyền thành công!\n");
		setTextColor(BLACK);
		wprintf(L"Nhấn enter để quay lại menu");
		fclose(f);
		dustIgnore();
		return;
	}
	else
	{
		setTextColor(RED);
		wprintf(L"Không thể cập nhật file dữ liệu thông tin tài khoản\nCập nhật quyền không thành công\n");
		setTextColor(BLACK);
		fclose(f);
		wprintf(L"Nhấn enter để quay lại menu");
		dustIgnore();
		return;
	}
}
void changePasswordChooser(AuthFile& credentials, User& user)
{
	int key, choice = 0;
	system("CLS");
	writeProgName();
	setTextColor(RED);
	wprintf(L"                                                              --- Thay Đổi Mật Khẩu ---\n");
	setTextColor(PURPLE);
	wprintf(L"Chọn người dùng bạn muốn thay đổi mật khẩu: \n");
functionChooser:
	if (choice == 0) setTextColor(RED); else setTextColor(BLACK);
	wprintf(L"   >  Chính mình\n");
	if (choice == 1) setTextColor(RED); else setTextColor(BLACK);
	wprintf(L"   >  Người dùng khác\n");
	setTextColor(BLACK);
	key = _getch();
	switch (key)
	{
	case KEY_UP:
		if (choice == 0) choice = 1; else choice = 0;
		wprintf(L"\033[F\33[2K\033[F\33[2K");
		goto functionChooser;
		break;
	case KEY_DOWN:
		if (choice == 0) choice = 1; else choice = 0;
		wprintf(L"\033[F\33[2K\033[F\33[2K");
		goto functionChooser;
		break;
	case KEY_ENTER:
		if (choice == 0)
		{
			changePassword(credentials, user, 0);
			return;
		}
		else goto accountChooser;
		break;
	case ESC_KEY:
		return;
		break;
	default:
		wprintf(L"\033[F\33[2K\033[F\33[2K");
		goto functionChooser;
		break;
	}
	accountChooser:
	system("CLS");
	writeProgName();
	setTextColor(RED);
	wprintf(L"                                                            --- Thay Đổi Mật Khẩu ---\n");
	setTextColor(BLACK);
	if (credentials.size == 1)
	{
		setTextColor(RED);
		wprintf(L"Không có tài khoản nào để thay đổi mật khẩu\n");
		setTextColor(BLACK);
		wprintf(L"Nhấn enter để quay lại menu\n");
		dustIgnore();
		return;
	}
	char chosenHandle[MAX_USR_LEN];
	if (!secureInp(chosenHandle, MAX_USR_LEN, L"Nhập tên đăng nhập của tài khoản bạn muốn thay đổi mật khẩu: ", ACCEPTED_CHARS_HANDLE)) return;
	Node* currentNode = accessCredNode(1);
	choice = -1;
	for (int i = 1; i < credentials.size; i++)
	{
		if (strcmp(((User*) currentNode->data)->handle, chosenHandle) == 0)
		{
			choice = ((User*)currentNode->data)->id;
			break;
		}
		currentNode = currentNode->next;
	}
	if (choice != -1) changePassword(credentials, *(User*)accessCredNode(choice)->data, 1);
	else
	{
		setTextColor(RED);
		wprintf(L"Tài khoản không tồn tại\n");
		dustIgnore();
	}
}
void accountManage(AuthFile& credentials, User& user)
{
	int choice = 0, key;
	while (1)
	{
		if (user.id < 0) return;
		system("CLS");
		writeProgName();
		setTextColor(RED);
		ShowConsoleCursor(false);
		wprintf(L"                                                               === QUẢN LÝ TÀI KHOẢN ===\n");
		setTextColor(YELLOW);
		wprintf(L"\n                                            Chọn chức năng bạn muốn thực hiện (sử dụng phím mũi tên lên/xuống)\n");
	authMenu:
		gotoxy(0, 15);
		if (choice == 0) setTextColor(RED); else setTextColor(BLACK);
		wprintf(L"                                                                <  Trở về menu chính  >\n");
		if (choice == 1) setTextColor(RED); else setTextColor(BLACK);
		wprintf(L"\n                                                                    <  Đăng xuất  >\n");
		if (choice == 2) setTextColor(RED); else setTextColor(BLACK);
		wprintf(L"\n                                                                <  Thay đổi mật khẩu  >\n");
		if (choice == 3) setTextColor(RED); else setTextColor(BLACK);
		wprintf(L"\n                                                            <  Cập nhật thông tin cá nhân  >\n");
		if (user.role == ADMIN_ROLE)
		{
			if (choice == 4) setTextColor(RED); else setTextColor(BLACK);
			wprintf(L"\n                                                                  <  Tạo người dùng  >\n");
			if (choice == 5) setTextColor(RED); else setTextColor(BLACK);
			wprintf(L"\n                                                               <  Phân quyền người dùng  >\n");
			if (choice == 6) setTextColor(RED); else setTextColor(BLACK);
			wprintf(L"\n                                                           <  Cập nhật trạng thái tài khoản >\n");
		}
		setTextColor(BLACK);
		key = _getch();
		switch (key)
		{
		case KEY_UP:
			if (choice == MENU_MIN_OPTION && user.role == ADMIN_ROLE) choice = AUTH_MENU_MAX_ADMIN_OPTION;
				else if (choice == MENU_MIN_OPTION && (user.role == MANAGER_ROLE || user.role == SPECIALIST_ROLE)) choice = AUTH_MENU_MAX_USER_OPTION; else choice--;
			goto authMenu;
			break;
		case KEY_DOWN:
			if (choice == AUTH_MENU_MAX_ADMIN_OPTION) choice = MENU_MIN_OPTION; 
				else if (choice == AUTH_MENU_MAX_USER_OPTION && (user.role == MANAGER_ROLE || user.role == SPECIALIST_ROLE)) choice = MENU_MIN_OPTION; else choice++;
			goto authMenu;
			break;
		case KEY_ENTER:
			goto authChooser;
			break;
		case ESC_KEY:
			return;
			break;
		default:
			goto authMenu;
			break;
		}
		
	authChooser:
		ShowConsoleCursor(true);
		switch (choice)
		{
		case 1:
			signOut(user);
			return;
			break;
		case 2:
			if (user.role != ADMIN_ROLE)
				changePassword(credentials, user, 0); else changePasswordChooser(credentials, user);
			break;
		case 3:
			if (user.role != ADMIN_ROLE)
				accountInfoUpdate(credentials, user); else accountInfoUpdateChooser(credentials, user);
			break;
		case 4:
			if (user.role == ADMIN_ROLE)
			{
				createAccount(credentials);
			}
			else wprintf(L"Chức năng không tồn tại hoặc bạn không có quyền sử dụng chức năng này!\n");
			break;
		case 5:
			if (user.role == ADMIN_ROLE)
			{
				updateRole(credentials);
			}
			else wprintf(L"Chức năng không tồn tại hoặc bạn không có quyền sử dụng chức năng này!\n");
			break;
		case 6:
			if (user.role == ADMIN_ROLE)
			{
				updateStatusChooser(credentials);
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