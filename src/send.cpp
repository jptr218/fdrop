#include "fdrop.h"

void send(int id, string address, string file) {
	SOCKET sock;
	addrinfo* dest;
	addrinfo hints;
	addrinfo* ptr;
	FILE* f = fopen(file.c_str(), "rb");

	if (ferror(f)) {
		cout << "The specified file does not exist." << endl;
		return;
	}

	DWORD bytes;
	char buf[500];
	char fbuf[500];

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	if (getaddrinfo(("192.168.1." + address).c_str(), "4321", &hints, &dest) != 0) {
		cout << "Couldn't find that user. Are you sure you've typed their ID correctly?" << endl;
		return;
	}
	for (ptr = dest; ptr != NULL; ptr = ptr->ai_next) {

		sock = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (sock == INVALID_SOCKET) {
			cout << "Error sending, failed to create TCP messager." << endl;
			return;
		}

		if (connect(sock, ptr->ai_addr, (int)ptr->ai_addrlen) == SOCKET_ERROR) {
			closesocket(sock);
			sock = INVALID_SOCKET;
			continue;
		}
		break;
	}
	freeaddrinfo(dest);

	if (sock == INVALID_SOCKET) {
		cout << "Couldn't find that user. Are you sure you've typed their ID correctly?" << endl;
		return;
	}

	fdrop_req_hdr* hdr = (fdrop_req_hdr*)buf;
	hdr->hdrLen = 0;
	hdr->from = id;
	hdr->fnLen = file.length();
	hdr->dLen = fsize(f);
	hdr->hdrLen = sizeof(hdr) * sizeof(int);

	memcpy(&buf[hdr->hdrLen], file.c_str(), file.length());

	if (send(sock, buf, 500, NULL) == SOCKET_ERROR) {
		cout << "Error sending file, error code " << WSAGetLastError() << endl;
		return;
	}

	if ((bytes = recv(sock, buf, 500, NULL)) == SOCKET_ERROR) {
		cout << "Error sending file, error code " << WSAGetLastError() << endl;
		return;
	}

	if (extractBytes(string(buf), bytes) == "ACCEPT") {
		while ((bytes = fread(fbuf, 1, 500, f)) > 0) {
			if (send(sock, fbuf, bytes, NULL) == SOCKET_ERROR) {
				cout << "Error sending file, error code " << WSAGetLastError() << endl;
				return;
			}
		}
	}
	else {
		cout << "Your file was denied by the recipitent." << endl;
		return;
	}

	fclose(f);
}