#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "make.h"
#include "guess.h"

static void my_assert(int TF, const char* message){
	if(!TF){ puts(message); exit(0); }
}

int main(){
	int N, M, Dist[111];
	char st[17];
	std::vector<std::string> Dict;

	my_assert(scanf("%d%d", &N, &M) == 2, "Error: Invalid Input");
	my_assert(1 <= N && N <= 100 && 1 <= M && M <= 10, "Error: Invalid Input");

	Dict.resize(N);
	for(int i = 0; i < N; i++){
		my_assert(scanf("\n%s", st) == 1, "Error: Invalid Input");
		my_assert((int) strlen(st) == M, "Error: Invalid Input");
		for(int j = 0; j < M; j++) my_assert('a' <= st[j] && st[j] <= 'z', "Error: Invalid Input");
		Dict[i] = st;
		for(int j = 0; j < i; j++) my_assert(Dict[i] != Dict[j], "Error: Invalid Input");
	}

	std::string sh1, yh, sh2;
	sh1 = MakeWord(N, M, Dict);
	my_assert((int) sh1.size() == M, "Wrong[1]: Invalid Word");
	for(int j = 0; j < M; j++) my_assert('a' <= sh1[j] && sh1[j] <= 'z', "Wrong[1]: Invalid Word");

	int mxd = 0, mcn = 0;
	for(int i = 0; i < N; i++){
		int dif = 0;
		for(int j = 0; j < M; j++){
			if(sh1[j] != Dict[i][j]) dif++;
		}
		Dist[i] = dif;
		if(mxd < dif) mxd = dif, mcn = 0;
		if(mxd == dif) mcn++;
	}

	mcn = rand() % mcn;
	for(int i = 0; i < N; i++){
		if(mxd == Dist[i]){
			if(!mcn){
				yh = Dict[i];
				break;
			}
			else mcn--;
		}
	}

	sh2 = GuessWord(M, yh);
	my_assert(sh1 == sh2, "Wrong[2] : Wrong Answer");
	puts("Correct");
	return 0;
}
