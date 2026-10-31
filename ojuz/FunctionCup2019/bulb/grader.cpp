#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bulb.h"

static void my_assert(int TF, const char* message){
	if(!TF){ puts(message); exit(0); }
}

int main(){
	int N, T;
	std::vector<int> L, R;

	my_assert(scanf("%d%d", &N, &T) == 2, "Error: Invalid Input");
	my_assert(1 <= N && N <= 300000, "Error: Invalid Input");
	my_assert(1 <= T && T <= 300000, "Error: Invalid Input");

	L.resize(N), R.resize(N);
	for(int i = 0; i < N; i++){
		my_assert(scanf("%d%d", &L[i], &R[i]) == 2, "Error: Invalid Input");
		my_assert(-2 <= L[i] && L[i] <= N-1, "Error: Invalid Input");
		my_assert(-2 <= R[i] && R[i] <= N-1, "Error: Invalid Input");
	}

	int res = FindWinner(T, L, R);
	my_assert(res == 0 || res == 1, "Wrong: Wrong Answer");
	puts(res ? "Minhyung" : "Sunyoul");
	return 0;
}
