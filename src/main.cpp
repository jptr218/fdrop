#include "fdrop.h"

int main(int argc, char* argv[]) {
	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		cout << "Failed to find ws2_32.dll" << endl;
		return 0;
	}
	
	int id = getId();

	if (string(argv[1]) == "start") {
		TCHAR t_fileName[MAX_PATH];
		GetModuleFileName(NULL, t_fileName, MAX_PATH);
		wstring w_fileName(&t_fileName[0]);
		string s_fileName(w_fileName.begin(), w_fileName.end());

		_popen((s_fileName + " bp").c_str(), "r");

		cout << "Fdrop has started." << endl << "Your user id is " << id << endl;
	}
	else if (string(argv[1]) == "stop") {
		_popen("taskkill /F /T /IM fdrop.exe", "r");
	}
	else if (string(argv[1]) == "send" && argc == 4) {
		send(id, argv[2], argv[3]);
	}
	else if (string(argv[1]) == "bp"){
		listen();
	}
	else {
		cout << "Usage:" << endl << "fdrop start" << endl << "fdrop stop" << endl << "fdrop send [id] [file]" << endl;
	}

	return 1;
}