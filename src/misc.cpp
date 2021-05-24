#include "fdrop.h"

string extractBytes(string buf, DWORD bytes) {
	string o;
	for (int i = 0; i < bytes; i++) {
		o += buf[i];
	}
	return o;
}

int getId() {
    char host[256];
    gethostname(host, 256);
    hostent* h = gethostbyname(host);
    char* ip = inet_ntoa(*((in_addr*)h->h_addr_list[0]));

    string s_id;
    int ld = 0;
    for (int i = 0; i < string(ip).length(); i++) {
        if (string(ip).at(i) == '.') {
            ld = i;
        }
    }
    for (int i = ld + 1; i < string(ip).length(); i++) {
        s_id += string(ip).at(i);
    }

    return stoi(s_id);
}

LPCWSTR getWstr(string& str)
{
    int bufferlen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), NULL, 0);
    if (bufferlen == 0)
    {
        return 0;
    }
    LPWSTR widestr = new WCHAR[bufferlen + 1];
    ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), widestr, bufferlen);
    widestr[bufferlen] = 0;
    return widestr;
}

string getUsername() {
    TCHAR t_uname[256 + 1];
    DWORD size = 256 + 1;
    GetUserName((TCHAR*)t_uname, &size);
    wstring w_uname(&t_uname[0]);
    return string(w_uname.begin(), w_uname.end());
}

int alert(string message, int type) {
    MessageBeep(MB_ICONINFORMATION);
    return MessageBoxW(NULL, getWstr(message), L"Fdrop", type);
}

int fsize(FILE* f) {
    fseek(f, 0, SEEK_END);
    int s = ftell(f);
    rewind(f);
    return s;
}