//needed for newest versions
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif

//includes
#include<winsock2.h>
#include<iphlpapi.h>
#include<ws2tcpip.h>
#include<stdio.h>
#include<stdlib.h>

//Libraries
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")

int main() {
	//declare winsock data, declare and initalize 20,000 bytes to asize, and declare adapters
	WSADATA d;
	DWORD asize = 20000;
	PIP_ADAPTER_ADDRESSES adapters;

	//assign winsock version
	if (WSAStartup(MAKEWORD(2, 2), &d)) {
		printf("Failed to initialize WinSock2.2");
		return -1;
	}

	//Checks if asize if enough, or too little for adapters to hold all the adapters info it gets in GetAdapterAddress()
	do {
		//assign the size of adpaters
		adapters = (PIP_ADAPTER_ADDRESSES)malloc(asize);
		//check if can assign, and if can't end program
		if (!adapters) {
			printf("Couldn't allocate %ld bytes for adapters.\n", asize);
			WSACleanup();
			return -1;
		}
		//specifies to get all adapters and addresses, and reassigns asize a size 
		int r = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, 0, adapters, &asize);
		//checks if asize overflowed, and free adapters to reassign them new size
		if (r == ERROR_BUFFER_OVERFLOW) {
			printf("GetAdaptersAddresses wants %ld bytes.\n", asize);
			free(adapters);
		}
		//checks if the getAdaptersAddresses succeeds, and breaks out of loop if does
		else if (r == ERROR_SUCCESS) {
			break;
		}
		//nothing works end program
		else {
			printf("Error from GetAdaptersAddresses: %d\n", r);
			free(adapters);
			WSACleanup();
			return -1;
		}
	} while (!adapters);

	//declare adapter and initalize it to adapters
	PIP_ADAPTER_ADDRESSES adapter = adapters;
	//print all adapters, and their addresses
	while (adapter) {
		//print the adapter name
		printf("\nAdapter name: %S\n", adapter->FriendlyName);

		//assign the unicast addresses to address
		PIP_ADAPTER_UNICAST_ADDRESS address = adapter->FirstUnicastAddress;
		//cycle, and print addresses
		while (address) {
			//check what type of address it is, and print the type
			printf("\t%s", address->Address.lpSockaddr->sa_family == AF_INET ? "IPv4" : "IPv6");

			//declare a char array
			char ap[100];

			//assign the address's address to the char array
			getnameinfo(address->Address.lpSockaddr, address->Address.iSockaddrLength, ap, sizeof(ap), 0, 0, NI_NUMERICHOST);
			//print the address
			printf("\t%s\n", ap);

			//cycle to the next address
			address = address->Next;
		}

		//cycle to the next adapter
		adapter = adapter->Next;
	}

	//end the program
	free(adapters);
	WSACleanup();
	return 0;
}