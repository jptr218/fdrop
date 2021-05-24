#pragma once

#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>

#pragma comment(lib, "Ws2_32.lib")
using namespace std;

struct fdrop_req_hdr {
	int hdrLen;
	int from;
	int fnLen;
	int dLen;
};

string extractBytes(string buf, DWORD bytes);
int getId();
string getUsername();

int fsize(FILE* f);

int alert(string message, int type);

void listen();
void send(int id, string address, string file);