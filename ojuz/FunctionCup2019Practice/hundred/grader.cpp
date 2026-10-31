#include <stdio.h>
#include <stdlib.h>
#include "hundred.h"

static void my_assert(int TF, const char* message){
	if(!TF){ puts(message); exit(0); }
}

static int A = 0, B = 0, C = 0;
static std::string ans;
static int call_cnt = 0;
int Mark(std::string S){
	call_cnt++;
	my_assert(call_cnt <= 100, "Wrong: Too Much Calls");
	my_assert(S.size() == 100, "Wrong: Invalid S");

	int fin, As, Bs, Cs;
	fin = As = Bs = Cs = 0;
	for(int i = 0; i < 100; i++){
		my_assert(S[i] == 'A' || S[i] == 'B' || S[i] == 'C', "Wrong: Invalid S");
		if(S[i] == 'A') As++;
		if(S[i] == 'B') Bs++;
		if(S[i] == 'C') Cs++;
		if(S[i] == ans[i]) fin++;
	}
	my_assert(As == A && Bs == B && Cs == C, "Wrong: Invalid S");
	return fin;
}

int main(){
	char st[111];
	my_assert(scanf("%s", st) == 1, "Error: Invalid Input");
	ans = st;
	my_assert(ans.size() == 100, "Error: Invalid Input");

	for(int i = 0; i < 100; i++){
		if(ans[i] == 'A') A++;
		else if(ans[i] == 'B') B++;
		else if(ans[i] == 'C') C++;
		else my_assert(0, "Error: Invalid Input");
	}

	std::string you = GetHundredPoints(A, B, C);
	my_assert(ans == you, "Wrong: Wrong Answer");
	printf("Correct\n%d\n", call_cnt);
	return 0;
}
