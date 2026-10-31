#include <stdio.h>
#include <stdlib.h>
#include "squad.h"

static int N, Q;
static std::vector<int> Att, Def, Pop;

static void my_assert(int TF, const char* message){
	if(!TF){ puts(message); exit(1); }
}

int main(){
	my_assert(scanf("%d", &N) == 1, "Error: Invalid Input");
	my_assert(2 <= N && N <= 1000000, "Error: Invalid Input");

	Att.resize(N); Def.resize(N); Pop.resize(N);
	for(int i = 0; i < N; i++){
		my_assert(scanf("%d%d%d", &Att[i], &Def[i], &Pop[i]) == 3, "Error: Invalid Input");
		my_assert(1 <= Att[i] && Att[i] <= 1000000000, "Error: Invalid Input");
		my_assert(1 <= Def[i] && Def[i] <= 1000000000, "Error: Invalid Input");
		my_assert(1 <= Pop[i] && Pop[i] <= 1000000000, "Error: Invalid Input");
	}
	Init(Att, Def, Pop);

	my_assert(scanf("%d", &Q) == 1, "Error: Invalid Input");
	my_assert(1 <= Q && Q <= 1000000, "Error: Invalid Input");

	for(int i = 0; i < Q; i++){
		int X, Y;
		my_assert(scanf("%d%d", &X, &Y) == 2, "Error: Invalid Input");
		my_assert(1 <= X && X <= 1000000000, "Error: Invalid Input");
		my_assert(1 <= Y && Y <= 1000000000, "Error: Invalid Input");
		printf("%lld\n", BestSquad(X, Y));
	}
	return 0;
}
