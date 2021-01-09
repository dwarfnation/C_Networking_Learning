//Includes
#include<sys/socket.h>
#include<netdb.h>
#include<ifaddrs.h>
#include<stdio.h>
#include<stdlib.h>

int main() {
	//declare pointer addresses
	struct ifaddrs *addresses;

	//getifaddrs allocates the memory for addresses, and gives a linked list of addresses to addresses
	//Also checks if it works
	if (getifaddrs(&addresses) == -1) {
		printf("getifaddrs call faliled\n");
		return -1;
	}

	//declare new pointer address, and initalize it with addresses(used to cycle through)
	struct ifaddrs *address = addresses;
	//prints out the names of the sockets, and their addresess
	while (address) {
		//get address family
		int family = address->ifa_addr->sa_family;
		//check if it's not IPv4 or IPv6
		if (family == AF_INET || family == AF_INET6) {
			//print socket name
			printf("%s\t", address->ifa_name);
			//print address type
			printf("%s\t", family == AF_INET ? "IPv4" : "IPv6");

			//declare char array ap for storing the address
			char ap[100];

			//gets address type size
			const int family_size = family == AF_INET ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6);
			//gets the actual address and stores it in ap
			getnameinfo(address->ifa_addr, family_size, ap, sizeof(ap), 0, 0, NI_NUMERICHOST);
			//print address
			printf("\t%s\n", ap);
		}

		//cycle to the next socket
		address = address->ifa_next;
	}

	//End the program
	freeifaddrs(addresses);
	return 0;
}