#include "zeus.h"
#include <stdio.h>
#include <string.h>

#define MAX_LEN 256

int main() {
	int my_id = ZEUS(MyNodeId)();
	int nof_nodes = ZEUS(NumberOfNodes)();
	printf("Nodeow jest %d, a ja mam numer %d.\n", nof_nodes, my_id);
	if (my_id < nof_nodes - 1) {
		char msg[MAX_LEN];
		snprintf(msg, sizeof(msg), "Hello from %d!", my_id);
		msg[MAX_LEN-1] = '\0';
		printf("Wysylam wiadomosc do %d.\n", my_id + 1);
		ZEUS(Send)(my_id + 1, msg, strlen(msg));
	}
	if (my_id > 0) {
		printf("Odbieram wiadomosc od %d.\n", my_id - 1);
		char msg[MAX_LEN];
		fflush(stdout);
		ZEUS(MessageInfo) mi = ZEUS(Receive)(my_id - 1, msg, sizeof(msg) - 1);
		msg[mi.length] = '\0';
		printf("Odebralem: %s\n", msg);
	}
	return 0;
}

