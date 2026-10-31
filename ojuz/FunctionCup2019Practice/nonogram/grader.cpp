#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "nonogram.h"

static void my_assert(int TF, const char* message){
	if(!TF){ puts(message); exit(0); }
}

int main(){
	int N, M;
	std::vector<std::vector<int>> Rclue, Cclue, gr;

	my_assert(scanf("%d%d", &N, &M) == 2, "Error: Invalid Input");
	my_assert(1 <= N && N <= 100 && 1 <= M && M <= 100, "Error: Invalid Input");

	Rclue.resize(N);
	for(int i = 0; i < N; i++){
		int sz;
		my_assert(scanf("%d", &sz) == 1, "Error: Invalid Input");
		Rclue[i].resize(sz);
		for(int j = 0; j < sz; j++){
			my_assert(scanf("%d", &Rclue[i][j]) == 1, "Error: Invalid Input");
			my_assert(Rclue[i][j], "Error: Invalid Input");
		}
	}

	Cclue.resize(M);
	for(int i = 0; i < M; i++){
		int sz;
		my_assert(scanf("%d", &sz) == 1, "Error: Invalid Input");
		Cclue[i].resize(sz);
		for(int j = 0; j < sz; j++){
			my_assert(scanf("%d", &Cclue[i][j]) == 1, "Error: Invalid Input");
			my_assert(Cclue[i][j], "Error: Invalid Input");
		}
	}

	gr = SolveNonogram(N, M, Rclue, Cclue);
	int rc = gr.size();
	for(int i = 0; i < rc; i++){
		int cc = gr[i].size();
		for(int j = 0; j < cc; j++) printf("%d ", gr[i][j]);
		puts("");
	}
	return 0;
}
