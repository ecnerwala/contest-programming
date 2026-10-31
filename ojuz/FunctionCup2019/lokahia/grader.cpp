#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "lokahia.h"

static void my_assert(int TF, const char* message){
	if(!TF){ puts(message); exit(0); }
}

static int N, reach[202][202], call_cnt = 0;
int CollectRelics(int R1, int R2){
	call_cnt++;
	my_assert(call_cnt <= 300, "Wrong : Too Much Calls");
	my_assert(0 <= R1 && 0 <= R2 && R1 < N && R2 < N && R1 != R2, "Wrong : Invalid Argument");

	for(int i=0; i<N; i++){
		if(reach[i][R1] && reach[i][R2]) return i;
	}
	return -1;
}

int main(){
	my_assert(scanf("%d", &N) == 1, "Error: Invalid Input");
	my_assert(1 <= N && N <= 200, "Error: Invalid Input");

	int con[202][202] = {0,};
	for(int i = 1; i < N; i++){
		int s, e, m;
		my_assert(scanf("%d%d%d", &s, &e, &m) == 3, "Error: Invalid Input");
		my_assert(0 <= s && s < N && 0 <= e && e < N && s != e && 1 <= m && m <= N, "Error: Invalid Input");
		con[s][e] = con[e][s] = m;
	}

	int near[202];
	for(int i = 0; i < N; i++){
		memset(near, 0, sizeof(near));
		reach[i][i] = 1, near[i] = -1;
		for(int j = 0; j < N; j++){
			if(con[i][j] > 0 && near[j] == 0) near[j] = con[i][j];
		}
		for(int vis = 1; vis < N; vis++){
			int nxt = -1;
			for(int j = 0; j < N; j++){
				if(near[j] > 0 && near[j] <= vis){
					nxt = j;
					break;
				}
			}
			if(nxt < 0) break;
			reach[i][nxt] = 1, near[nxt] = -1;
			for(int j = 0; j < N; j++){
				if(con[nxt][j] > 0 && near[j] == 0) near[j] = con[nxt][j];
			}
		}
	}

	int base = FindBase(N);
	if(base == -1){
		for(int i = 0; i < N; i++){
			int cnt = 0;
			for(int j = 0; j < N; j++) cnt += reach[i][j];
			my_assert(cnt <= N / 2, "Wrong: Wrong Answer");
		}
	}
	else if(0 <= base && base < N){
		int cnt = 0;
		for(int j = 0; j < N; j++) cnt += reach[base][j];
		my_assert(cnt > N / 2, "Wrong: Wrong Answer");
	}
	else my_assert(0, "Wrong: Wrong Answer");
	printf("Correct\n%d\n", call_cnt);
	return 0;
}
