#include <stdio.h>
#include <stdlib.h>
#include "island.h"

static void my_assert(int TF, const char* message){
	if(!TF){ puts(message); exit(0); }
}

int main(){
	int N, M, K, Q;
	std::vector<int> F, S, E;

	my_assert(scanf("%d%d%d%d", &N, &M, &K, &Q) == 4, "Error: Invalid Input");
	my_assert(1 <= K && K <= N && N <= 300000, "Error: Invalid Input");
	my_assert(N - 1 <= M && M <= 1000000, "Error: Invalid Input");
	my_assert(1 <= Q && Q <= 300000, "Error: Invalid Input");

	for(int i = 0; i < N; i++){
		int a;
		my_assert(scanf("%d", &a) == 1, "Error: Invalid Input");
		my_assert(0 <= a && a < K, "Error: Invalid Input");
		F.push_back(a);
	}
	for(int i = 0; i < M; i++){
		int a, b;
		my_assert(scanf("%d%d", &a, &b) == 2, "Error: Invalid Input");
		my_assert(0 <= a && a < N && 0 <= b && b < N && a != b, "Error: Invalid Input");
		S.push_back(a), E.push_back(b);
	}
	Init(K, F, S, E);

	for(int i = 0; i < Q; i++){
		int a, b;
		my_assert(scanf("%d%d", &a, &b) == 2, "Error: Invalid Input");
		my_assert(0 <= a && a < K && 0 <= b && b < K && a != b, "Error: Invalid Input");
		printf("%d\n", Separate(a, b));
	}
	return 0;
}
