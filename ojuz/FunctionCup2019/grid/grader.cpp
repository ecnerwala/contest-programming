#include <stdio.h>
#include <stdlib.h>
#include "grid.h"

static int N, size[1010], chk[1010];

static void my_assert(int TF, const char* message){
	if(!TF){ puts(message); exit(0); }
}

static int call_cnt = 0, rp_cnt = 0;
int PutDisks(const std::vector<int> Disks){
	call_cnt++;
	my_assert(call_cnt <= 2000, "Wrong: Too Much Calls");
	my_assert(!rp_cnt, "Wrong: Called After Report");
	my_assert((int)Disks.size() == N, "Wrong: Invalid Argument");

	int lv = 0;
	for(int i = 0; i < N; i++) chk[i] = 0;
	for(int i = 0; i < N; i++){
		my_assert(0 <= Disks[i] && Disks[i] < N && !chk[Disks[i]], "Wrong: Invalid Argument");
		chk[Disks[i]] = 1;

		int sz = size[Disks[i]];
		if(lv < sz) lv = sz;
		else lv++;
	}

	return lv;
}

int main(){
	my_assert(scanf("%d", &N) == 1, "Error: Invalid Input");
	my_assert(1 <= N && N <= 1000, "Error: Invalid Input");
	for(int i = 0; i < N; i++){
		my_assert(scanf("%d", &size[i]) == 1, "Error: Invalid Input");
		my_assert(1 <= size[i] && size[i] <= N && !chk[size[i]], "Error: Invalid Input");
		chk[size[i]] = 1;
	}

	std::vector<int> v = SortDisks(N);
	my_assert((int) v.size() == N, "Wrong: Wrong Answer");
	for(int i = 0; i < N; i++)
		my_assert(size[i] == v[i], "Wrong: Wrong Answer");

	printf("Correct\n%d\n", call_cnt);
	return 0;
}
