#include "../Misc/Library.h"
const int encryptKey = -27;
HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
DWORD prev_mode;
bool secureInp(char field[], int max, const wchar_t fieldName[], const char accepted[])
{
	//In ra ten field
	setTextColor(PURPLE);
	wprintf(L"%s", fieldName);
	setTextColor(BLACK);
	int count = 0;
	char buffer;
	do
	{
		buffer = _getch();
		if (buffer == -32)
		{
			//Ignore Arrow Keys
			_getch();
		}
		//Neu la esc thi return
		if (buffer == ESC_KEY) return false;
		//Neu la enter va do dai chuoi > 0 thi lay chuoi
		if (buffer == KEY_ENTER)
		{
			if (count > 0) break;
			else continue;
		}
		if (buffer == '\b' && count > 0) {
			field[--count] = '\0';
			wprintf(L"\b \b");
		}
		else if (strchr(accepted, buffer))
		{
			field[count] = buffer;
			count++;
			wprintf(L"%c", buffer);
		}
	} while (count < max - 1);
	wprintf(L"\n");
	field[count] = '\0';
	return true;
}
void caesarEncrypt(char field[])
{
	for (int i = 0; i < strlen(field); i++)
		field[i] += encryptKey;
}
void caesarDecrypt(char field[])
{
	for (int i = 0; i < strlen(field); i++)
		field[i] -= encryptKey;
}
bool securePassword(char field[], int max, const wchar_t fieldName[])
{
	setTextColor(PURPLE);
	wprintf(L"%s", fieldName);
	setTextColor(BLACK);
	int count = 0;
	char buffer;
	do
	{
		buffer = _getch();
		if (buffer == -32)
		{
			//Ignore Arrow Keys
			_getch();
		}
		if (buffer == ESC_KEY) return false;
		if (buffer == '\r')
		{
			if (count > 0) break;
			else continue;
		}
		if (buffer == '\b' && count > 0) {
			count--;
			field[count] = '\0';
			wprintf(L"\b \b");
		}
		else if (strchr(ACCEPTED_CHARS_PWD, buffer))
		{
			field[count] = buffer;
			//Mat na de in ra man hinh, giau password that
			count++;
			wprintf(L"*");
		}
	} while (count < max - 1);
	wprintf(L"\n");
	field[count] = '\0';
	return true;
}
Node* handleCheck(List list, char handle[])
{
	for (Node* p = list.head; p; p = p->next)
	{
		if (strcmp(((User*)(p->data))->handle, handle) == 0) return p;
	}
	return NULL;
}
Node* userIdCardCheck(List list, char idCard[])
{
	for (Node* p = list.head; p; p = p->next)
	{
		char thisNodeIdCard[MAX_IDCARD_LEN];
		strcpy(thisNodeIdCard, ((User*)(p->data))->idCard);
		caesarDecrypt(thisNodeIdCard);
		if (strcmp(thisNodeIdCard, idCard) == 0) return p;
	}
	return NULL;
}
Node* memberIdCardCheck(List list, char idCard[])
{
	for (Node* p = list.head; p; p = p->next)
	{
		char thisNodeIdCard[MAX_IDCARD_LEN];
		strcpy(thisNodeIdCard, ((Member*)(p->data))->idCard);
		caesarDecrypt(thisNodeIdCard);
		if (strcmp(thisNodeIdCard, idCard) == 0) return p;
	}
	return NULL;
}
void gotoxy(int x, int y)
{
	COORD c = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}
void drawFavicon()
{
	//wprintf(L"\x1b[38;2;251;98;98m");
	wprintf(L"                                                                        \x1b[38;2;215;101;144m▄\x1b[38;2;186;107;168m▄▄\n");
	wprintf(L"                                                                        \x1b[38;2;205;103;152m█\x1b[38;2;143;115;202m██\n");
	wprintf(L"                                                                        \x1b[38;2;152;113;195m█\x1b[38;2;100;124;236m██\n");
	wprintf(L"                                                                        \x1b[38;2;106;123;231m█\x1b[38;2;76;134;255m██\n");
	wprintf(L"                                                                        \x1b[38;2;76;134;255m█\x1b[38;2;85;139;247m██\n");
	wprintf(L"                                                                        \x1b[38;2;76;134;255m█\x1b[38;2;85;139;247m██\n");
	wprintf(L"                                                                        \x1b[38;2;76;134;255m███\x1b[38;2;122;145;217m█\x1b[38;2;154;149;191m██\x1b[38;2;209;155;146m█\x1b[38;2;237;158;123m██\n");
	wprintf(L"                                                                        \x1b[38;2;76;134;255m▀▀▀\x1b[38;2;122;145;217m▀\x1b[38;2;169;151;179m▀▀\x1b[38;2;226;156;133m▀\x1b[38;2;247;159;115m▀▀\n");
}
void drawLeftLogo()
{
	gotoxy(0, 0);
	wprintf(L"\n\n                                            \x1b[38;2;215;101;144m▄\x1b[38;2;186;107;168m▄▄\n");
	wprintf(L"                                            \x1b[38;2;205;103;152m█\x1b[38;2;143;115;202m██\n");
	wprintf(L"                                            \x1b[38;2;152;113;195m█\x1b[38;2;100;124;236m██\n");
	wprintf(L"                                            \x1b[38;2;106;123;231m█\x1b[38;2;76;134;255m██\n");
	wprintf(L"                                            \x1b[38;2;76;134;255m█\x1b[38;2;85;139;247m██\n");
	wprintf(L"                                            \x1b[38;2;76;134;255m█\x1b[38;2;85;139;247m██\n");
	wprintf(L"                                            \x1b[38;2;76;134;255m███\x1b[38;2;122;145;217m█\x1b[38;2;154;149;191m██\x1b[38;2;209;155;146m█\x1b[38;2;237;158;123m██\n");
	wprintf(L"                                            \x1b[38;2;76;134;255m▀▀▀\x1b[38;2;122;145;217m▀\x1b[38;2;169;151;179m▀▀\x1b[38;2;226;156;133m▀\x1b[38;2;247;159;115m▀▀\n");
}
void writeProgName()
{
	gotoxy(0, 4);

	wprintf(L"                                                                                         \x1b[38;2;200;200;200m▀■▄\n");
	wprintf(L"                                                                                         \x1b[38;2;200;200;200m▄■▀\n");
	gotoxy(0, 1);
	drawFavicon();
	gotoxy(0, 4);
	wprintf(L"                                                           \x1b[38;2;200;200;200m▄■▀\n");
	wprintf(L"                                                           \x1b[38;2;200;200;200m▀■▄\n");
	gotoxy(0, 9);
	wprintf(L"\n");
}
void drawUserIcon()
{
	setTextColor(BLACK);
	wprintf(L" ▄▄▄  \n");
	wprintf(L"█\x1b[38;2;254;231;194m███\x1b[38;2;0;0;0m█\n");
	wprintf(L" \x1b[38;2;254;231;194m███\n");
	wprintf(L" \x1b[38;2;228;218;175m▀▀▀\n");
	wprintf(L"");
	wprintf(L"");
	wprintf(L"");
}
void setTextColor(const wchar_t* color)
{
	wprintf(color);
}
void ShowConsoleCursor(bool showFlag)
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO     cursorInfo;

	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag; 
	SetConsoleCursorInfo(out, &cursorInfo);
}
void consoleInit(bool quickEdit)
{
	if (quickEdit)
	{
		SetConsoleMode(hInput, ENABLE_QUICK_EDIT_MODE | ENABLE_EXTENDED_FLAGS);
		return;
	}
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof cfi;
	cfi.nFont = 0;
	cfi.dwFontSize.X = 0;
	cfi.dwFontSize.Y = 16;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy(cfi.FaceName, L"Consolas");
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r); 
	SetWindowLong(console, GWL_STYLE, GetWindowLong(console, GWL_STYLE) & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX);
	MoveWindow(console, r.left, r.top, 1280, 720, TRUE);
	system("COLOR F0");
	SetConsoleMode(hInput, ENABLE_EXTENDED_FLAGS | (prev_mode & ~ENABLE_QUICK_EDIT_MODE) | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
	_setmode(_fileno(stdout), _O_U8TEXT);
}
void drawFirstLineIcon(int choice)
{
	gotoxy(0, 13);
	if (choice == -1)
	{
		wprintf(L"                                                 \x1b[38;2;58;58;58m▄▄▄▄                    \x1b[38;2;58;58;58m▄▄▄▄\n");
		wprintf(L"                                                \x1b[38;2;58;58;58m█\x1b[38;2;100;100;100m████\x1b[38;2;58;58;58m█                  \x1b[38;2;58;58;58m██\x1b[38;2;170;170;170m██\x1b[38;2;58;58;58m██                      \x1b[38;2;50;50;50m▄\x1b[38;2;100;100;100m▄█████\n");
		wprintf(L"                                                 \x1b[38;2;100;100;100m████                   \x1b[38;2;58;58;58m█\x1b[38;2;170;170;170m████\x1b[38;2;58;58;58m█                      \x1b[38;2;50;50;50m█\x1b[38;2;100;100;100m██████\n");
		wprintf(L"                                                 \x1b[38;2;30;30;30m ▀▀                     \x1b[38;2;170;170;170m ▀▀                        \x1b[38;2;50;50;50m█\x1b[38;2;100;100;100m██████\n");
		wprintf(L"                                               \x1b[38;2;151;151;151m▄██\x1b[38;2;100;100;100m▄▄\x1b[38;2;151;151;151m██▄                \x1b[38;2;170;170;170m████████                     \x1b[38;2;50;50;50m█\x1b[38;2;100;100;100m██████\n");
		wprintf(L"                                              \x1b[38;2;151;151;151m████████\x1b[38;2;171;171;171m■▀■             \x1b[38;2;100;100;100m████\x1b[38;2;170;170;170m██\x1b[38;2;100;100;100m████                    \x1b[38;2;70;70;70m█\x1b[38;2;150;150;150m██████\n");
		wprintf(L"                                              \x1b[38;2;101;101;101m ████████\x1b[38;2;171;171;171m▀               \x1b[38;2;0;0;0m████████\n");
	}
	if (choice == 1)
	{
		wprintf(L"                                                 \x1b[38;2;140;98;56m▄▄▄▄                    \x1b[38;2;58;58;58m▄▄▄▄\n");
		wprintf(L"                                                \x1b[38;2;140;98;56m█\x1b[38;2;250;212;175m████\x1b[38;2;140;98;56m█                  \x1b[38;2;58;58;58m██\x1b[38;2;170;170;170m██\x1b[38;2;58;58;58m██                      \x1b[38;2;50;50;50m▄\x1b[38;2;100;100;100m▄█████\n");
		wprintf(L"                                                 \x1b[38;2;250;212;175m████                   \x1b[38;2;58;58;58m█\x1b[38;2;170;170;170m████\x1b[38;2;58;58;58m█                      \x1b[38;2;50;50;50m█\x1b[38;2;100;100;100m██████\n");
		wprintf(L"                                                 \x1b[38;2;199;177;153m ▀▀                     \x1b[38;2;170;170;170m ▀▀                        \x1b[38;2;50;50;50m█\x1b[38;2;100;100;100m██████\n");
		wprintf(L"                                               \x1b[38;2;41;170;227m▄██\x1b[38;2;250;212;175m▄▄\x1b[38;2;41;170;227m██▄                \x1b[38;2;170;170;170m████████                     \x1b[38;2;50;50;50m█\x1b[38;2;100;100;100m██████\n");
		wprintf(L"                                              \x1b[38;2;41;170;227m████████\x1b[38;2;248;20;66m■▀■             \x1b[38;2;100;100;100m████\x1b[38;2;170;170;170m██\x1b[38;2;100;100;100m████                    \x1b[38;2;70;70;70m█\x1b[38;2;150;150;150m██████\n");
		wprintf(L"                                              \x1b[38;2;26;141;191m ████████\x1b[38;2;248;20;66m▀               \x1b[38;2;0;0;0m████████\n");
	}
	if (choice == 2)
	{
		wprintf(L"                                                 \x1b[38;2;58;58;58m▄▄▄▄                    \x1b[38;2;140;98;56m▄▄▄▄\n");
		wprintf(L"                                                \x1b[38;2;58;58;58m█\x1b[38;2;100;100;100m████\x1b[38;2;58;58;58m█                  \x1b[38;2;140;98;56m██\x1b[38;2;245;223;197m██\x1b[38;2;140;98;56m██                      \x1b[38;2;50;50;50m▄\x1b[38;2;100;100;100m▄█████\n");
		wprintf(L"                                                 \x1b[38;2;100;100;100m████                   \x1b[38;2;140;98;56m█\x1b[38;2;245;223;197m████\x1b[38;2;140;98;56m█                      \x1b[38;2;50;50;50m█\x1b[38;2;100;100;100m██████\n");
		wprintf(L"                                                 \x1b[38;2;30;30;30m ▀▀                     \x1b[38;2;245;223;197m ▀▀                        \x1b[38;2;50;50;50m█\x1b[38;2;100;100;100m██████\n");
		wprintf(L"                                               \x1b[38;2;151;151;151m▄██\x1b[38;2;100;100;100m▄▄\x1b[38;2;151;151;151m██▄                \x1b[38;2;245;223;197m████████                     \x1b[38;2;50;50;50m█\x1b[38;2;100;100;100m██████\n");
		wprintf(L"                                              \x1b[38;2;151;151;151m████████\x1b[38;2;171;171;171m■▀■             \x1b[38;2;242;113;193m████\x1b[38;2;245;223;197m██\x1b[38;2;242;113;193m████                    \x1b[38;2;70;70;70m█\x1b[38;2;150;150;150m██████\n");
		wprintf(L"                                              \x1b[38;2;101;101;101m ████████\x1b[38;2;171;171;171m▀               \x1b[38;2;238;68;174m████████\n");
	}
	if (choice == 3)
	{
		wprintf(L"                                                 \x1b[38;2;58;58;58m▄▄▄▄                    \x1b[38;2;58;58;58m▄▄▄▄\n");
		wprintf(L"                                                \x1b[38;2;58;58;58m█\x1b[38;2;100;100;100m████\x1b[38;2;58;58;58m█                  \x1b[38;2;58;58;58m██\x1b[38;2;170;170;170m██\x1b[38;2;58;58;58m██                      \x1b[38;2;163;73;14m▄\x1b[38;2;273;124;48m▄█████\n");
		wprintf(L"                                                 \x1b[38;2;100;100;100m████                   \x1b[38;2;58;58;58m█\x1b[38;2;170;170;170m████\x1b[38;2;58;58;58m█                      \x1b[38;2;163;73;14m█\x1b[38;2;273;124;48m██████\n");
		wprintf(L"                                                 \x1b[38;2;30;30;30m ▀▀                     \x1b[38;2;170;170;170m ▀▀                        \x1b[38;2;163;73;14m█\x1b[38;2;273;124;48m██████\n");
		wprintf(L"                                               \x1b[38;2;151;151;151m▄██\x1b[38;2;100;100;100m▄▄\x1b[38;2;151;151;151m██▄                \x1b[38;2;170;170;170m████████                     \x1b[38;2;163;73;14m█\x1b[38;2;273;124;48m██████\n");
		wprintf(L"                                              \x1b[38;2;151;151;151m████████\x1b[38;2;171;171;171m■▀■             \x1b[38;2;100;100;100m████\x1b[38;2;170;170;170m██\x1b[38;2;100;100;100m████                    \x1b[38;2;205;126;39m█\x1b[38;2;251;220;200m██████\n");
		wprintf(L"                                              \x1b[38;2;101;101;101m ████████\x1b[38;2;171;171;171m▀               \x1b[38;2;0;0;0m████████\n");
	}
	if (choice == 1) setTextColor(RED); else setTextColor(BLACK);
	wprintf(L"\n                                           Quản lý tài khoản");
	if (choice == 2) setTextColor(RED); else setTextColor(BLACK);
	wprintf(L"        Quản lý độc giả");
	if (choice == 3) setTextColor(RED); else setTextColor(BLACK);
	wprintf(L"               Quản lý sách");
}
void drawSecondLineIcon(int choice)
{
	gotoxy(0, 24);
	if (choice == -1)
	{
		wprintf(L"                                               \x1b[38;2;101;101;101m▄■█████                  \x1b[38;2;102;102;102m▄■█████                   \x1b[38;2;70;70;70m\n");
		wprintf(L"                                               \x1b[38;2;101;101;101m███████                 \x1b[38;2;102;102;102m ███████                   \x1b[38;2;70;70;70m█\n");
		wprintf(L"                                               \x1b[38;2;101;101;101m███████                 \x1b[38;2;102;102;102m ███████                   \x1b[38;2;70;70;70m█   \n");
		wprintf(L"                                               \x1b[38;2;151;151;151m███████                  \x1b[38;2;151;151;151m███████                   \x1b[38;2;70;70;70m█   \x1b[38;2;121;121;121m▄■▄  ▄■   \n");
		wprintf(L"                                                                                                  \x1b[38;2;70;70;70m█ \x1b[38;2;121;121;121m▄■   ▀■▀\n");
		wprintf(L"                                            \x1b[38;2;101;101;101m       ▄▀             \x1b[38;2;102;102;102m        ■▄■                     \x1b[38;2;70;70;70m█\x1b[38;2;121;121;121m■      \n");
		wprintf(L"                                            \x1b[38;2;101;101;101m    ▀▄▀               \x1b[38;2;102;102;102m        ▄▀▄                     \x1b[38;2;70;70;70m▀▀▀▀▀▀▀▀▀▀▀\n");
	}
	if (choice == 4)
	{
		wprintf(L"                                               \x1b[38;2;11;206;128m▄■█████                  \x1b[38;2;102;102;102m▄■█████                   \x1b[38;2;70;70;70m\n");
		wprintf(L"                                               \x1b[38;2;11;206;128m███████                 \x1b[38;2;102;102;102m ███████                   \x1b[38;2;70;70;70m█\n");
		wprintf(L"                                               \x1b[38;2;11;206;128m███████                 \x1b[38;2;102;102;102m ███████                   \x1b[38;2;70;70;70m█   \n");
		wprintf(L"                                               \x1b[38;2;250;212;175m███████                  \x1b[38;2;151;151;151m███████                   \x1b[38;2;70;70;70m█   \x1b[38;2;121;121;121m▄■▄  ▄■   \n");
		wprintf(L"                                                                                                  \x1b[38;2;70;70;70m█ \x1b[38;2;121;121;121m▄■   ▀■▀\n");
		wprintf(L"                                            \x1b[38;2;11;206;128m       ▄▀             \x1b[38;2;102;102;102m        ■▄■                     \x1b[38;2;70;70;70m█\x1b[38;2;121;121;121m■      \n");
		wprintf(L"                                            \x1b[38;2;11;206;128m    ▀▄▀               \x1b[38;2;102;102;102m        ▄▀▄                     \x1b[38;2;70;70;70m▀▀▀▀▀▀▀▀▀▀▀\n");
	}
	if (choice == 5)
	{
		wprintf(L"                                               \x1b[38;2;101;101;101m▄■█████                  \x1b[38;2;251;98;98m▄■█████                   \x1b[38;2;70;70;70m\n");
		wprintf(L"                                               \x1b[38;2;101;101;101m███████                 \x1b[38;2;251;98;98m ███████                   \x1b[38;2;70;70;70m█\n");
		wprintf(L"                                               \x1b[38;2;101;101;101m███████                 \x1b[38;2;251;98;98m ███████                   \x1b[38;2;70;70;70m█   \n");
		wprintf(L"                                               \x1b[38;2;151;151;151m███████                  \x1b[38;2;250;212;175m███████                   \x1b[38;2;70;70;70m█   \x1b[38;2;121;121;121m▄■▄  ▄■   \n");
		wprintf(L"                                                                                                  \x1b[38;2;70;70;70m█ \x1b[38;2;121;121;121m▄■   ▀■▀\n");
		wprintf(L"                                            \x1b[38;2;101;101;101m       ▄▀             \x1b[38;2;251;98;98m        ■▄■                     \x1b[38;2;70;70;70m█\x1b[38;2;121;121;121m■      \n");
		wprintf(L"                                            \x1b[38;2;101;101;101m    ▀▄▀               \x1b[38;2;251;98;98m        ▄▀▄                     \x1b[38;2;70;70;70m▀▀▀▀▀▀▀▀▀▀▀\n");
	}
	if (choice == 6)
	{
		wprintf(L"                                               \x1b[38;2;101;101;101m▄■█████                  \x1b[38;2;102;102;102m▄■█████                   \n");
		wprintf(L"                                               \x1b[38;2;101;101;101m███████                 \x1b[38;2;102;102;102m ███████                   \x1b[38;2;65;148;152m█\n");
		wprintf(L"                                               \x1b[38;2;101;101;101m███████                 \x1b[38;2;102;102;102m ███████                   \x1b[38;2;30;30;30m█   \n");
		wprintf(L"                                               \x1b[38;2;151;151;151m███████                  \x1b[38;2;151;151;151m███████                   \x1b[38;2;30;30;30m█   \x1b[38;2;100;24;192m▄■▄  ▄■   \n");
		wprintf(L"                                                                                                  \x1b[38;2;30;30;30m█ \x1b[38;2;100;24;192m▄■   ▀■▀\n");
		wprintf(L"                                            \x1b[38;2;101;101;101m       ▄▀             \x1b[38;2;102;102;102m        ■▄■                     \x1b[38;2;30;30;30m█\x1b[38;2;100;24;192m■      \n");
		wprintf(L"                                            \x1b[38;2;101;101;101m    ▀▄▀               \x1b[38;2;102;102;102m        ▄▀▄                     \x1b[38;2;30;30;30m▀▀▀▀▀▀▀▀▀▀\x1b[38;2;159;57;57m▀\n");
	}
	if (choice == 4) setTextColor(RED); else setTextColor(BLACK);
	wprintf(L"\n                                         Lập phiếu mượn sách");
	if (choice == 5) setTextColor(RED); else setTextColor(BLACK);
	wprintf(L"       Lập phiếu trả sách");
	if (choice == 6) setTextColor(RED); else setTextColor(BLACK);
	wprintf(L"               Thống kê");
}
void drawExitIcon(int choice)
{
	gotoxy(0, 34);
	if (choice == 0)
	{
		wprintf(L"\n\n");
		wprintf(L"                                                                          \x1b[38;2;251;98;98m▄▄   \n");
		wprintf(L"                                                                          ▀▀   \n");
		wprintf(L"\n\n");
	}
	if (choice == -1)
	{
		wprintf(L"\n\n");
		wprintf(L"                                                                          \x1b[38;2;50;50;50m▄▄   \n");
		wprintf(L"                                                                          ▀▀   \n");
		wprintf(L"\n\n");
	}
	if (choice == 0) setTextColor(RED); else setTextColor(BLACK);
	wprintf(L"                                                                  Thoát chương trình");
}
void dustIgnore()
{
	char buffer;
	while (1)
	{
		buffer = _getch();
		if (buffer != '\r' && buffer != ESC_KEY) continue; else break;
	}
}
bool confBox()
{
	setTextColor(YELLOW);
	wprintf(L"                                                       Bạn có muốn lưu các thay đổi này không?\n");
	int option = 1, key;
optionChooser:
	if (option == 1) setTextColor(RED); else setTextColor(BLACK);
	wprintf(L"                                                            >  Có             ");
	if (option == 0) setTextColor(RED); else setTextColor(BLACK);
	wprintf(L"   >  Không\n");
	setTextColor(BLACK);
	key = _getch();
	switch (key)
	{
	case KEY_LEFT:
		if (option == 1) option = 0; else option = 1;
		wprintf(L"\033[F\33[2K");
		goto optionChooser;
		break;
	case KEY_RIGHT:
		if (option == 1) option = 0; else option = 1;
		wprintf(L"\033[F\33[2K");
		goto optionChooser;
		break;
	case KEY_ENTER:
		return option;
		break;
	case ESC_KEY:
		return false;
		break;
	default:
		wprintf(L"\033[F\33[2K");
		goto optionChooser;
		break;
	}
}