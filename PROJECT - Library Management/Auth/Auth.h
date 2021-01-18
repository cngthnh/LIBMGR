#ifndef _PROJ_LIB_MNMT_AUTH_
#define _PROJ_LIB_MNMT_AUTH_
#define AUTH_MENU_MAX_ADMIN_OPTION 6
#define AUTH_MENU_MAX_USER_OPTION 3
#include "..\Misc\Library.h"
void getCredentials(AuthFile& credentials);
void signIn(AuthFile credentials, User& thisSessionUser);
void accountManage(AuthFile& credentials, User& thisSessionUser);
void signOut(User& thisSessionUser);
void changePassword(AuthFile& credentials, User& thisSessionUser, bool force);
void accountInfoUpdate(AuthFile& credentials, User& userBase);
void createAccount(AuthFile& credentials);
bool userInfoInput(AuthFile& credentials, User*& user, int id, int role);
void updateRole(AuthFile& credentials);
long long hashGen(char pwd[]);
void changePasswordChooser(AuthFile& credentials, User& user);
void accountManage(AuthFile& credentials, User& user);
void updateStatusChooser(AuthFile credentials);
void updateAccountStatus(User& user, int status);
#endif