#include "fdrop.h"

void onRecieve(SOCKET sock) {
	char buf[500];
	DWORD bytes;

	fdrop_req_hdr* hdr = nullptr;
	string fname;
	int fsize;
	
	FILE* f = nullptr;
	DWORD bRd = 0;

	bytes = recv(sock, buf, 500, NULL);
	if (bytes < 0) {
		alert("Failed to recieve data from sender!", MB_ICONERROR);
		return;
	}

	hdr = (fdrop_req_hdr*)buf;
	fname = extractBytes(string(&buf[hdr->hdrLen]), hdr->fnLen);
	fsize = hdr->dLen;

	if (alert("Would you like to recieve " + fname + " from user " + to_string(hdr->from) + "?", MB_YESNO | MB_ICONINFORMATION) != IDYES) {
		send(sock, "DENY", 4, NULL);
		return;
	}

	if (send(sock, "ACCEPT", 6, NULL) == SOCKET_ERROR) {
		alert("Failed to recieve data from sender!", MB_ICONERROR);
		return;
	}

	f = fopen(("C:\\Users\\" + getUsername() + "\\Downloads\\" + fname).c_str(), "w");
		
	while (bRd < fsize) {
		bytes = recv(sock, buf, 500, NULL);
		if (bytes < 0) {
			alert("Failed to recieve " + fname + "!", MB_ICONERROR);
			return;
		}

		cout << bytes << endl;
		fwrite(buf, 1, bytes, f);

		bRd += bytes;
	}

	alert(fname + " saved to your downloads folder.", MB_ICONINFORMATION);

	fclose(f);
}

void listen() {
	addrinfo hints;
	addrinfo* result = NULL;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	if (getaddrinfo(NULL, "4321", &hints, &result) != 0) {
		cout << "Error starting Fdrop, failed to resolve local address" << endl;
		return;
	}

	SOCKET lSock = INVALID_SOCKET;
	lSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (lSock == INVALID_SOCKET) {
		cout << "Error starting Fdrop, failed to create TCP listener" << endl;
		return;
	}

	if (bind(lSock, result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR) {
		cout << "Error starting Fdrop, failed to create TCP listener" << endl;
		return;
	}
	freeaddrinfo(result);

	if (listen(lSock, SOMAXCONN) == SOCKET_ERROR) {
		cout << "Error starting Fdrop, failed to create TCP listener" << endl;
		return;
	}
	
	SOCKET s;
	while (s = accept(lSock, NULL, NULL)) {
		onRecieve(s);
	}
}