#include<bits/stdc++.h>
using namespace std;

const int MAXN = 300;
int N;
char A[MAXN];

int cnt[5];

int main() {

	scanf("%d %s", &N, A);

	memset(cnt, 0, sizeof(cnt));

	for(int i = 0; i < N; i++) {
		switch(A[i]) {
			case 'B': cnt[1]++;
					  break;
			case 'G': cnt[2]++;
					  break;
			case 'R': cnt[3]++;
					  break;
			default: assert(false);
		}
	}

	if(
			(cnt[1] >= 1 && cnt[2] >= 1 && cnt[3] >= 1) ||
			((cnt[1] >= 2) + (cnt[2] >= 2) + (cnt[3] >= 2) >= 2)
	  ) {
		printf("BGR\n");
	} else if ((cnt[1] >= 1) + (cnt[2] >= 1) + (cnt[3] >= 1) == 2) {
		// there are exactly two distinct values
		if(cnt[1] >= 2) {
			printf("GR\n");
		} else if(cnt[2] >= 2) {
			printf("BR\n");
		} else if(cnt[3] >= 2) {
			printf("BG\n");
		} else {
			assert(cnt[1] + cnt[2] + cnt[3] == 2);
			if(cnt[1] && cnt[2]) {
				printf("R\n");
			} else if(cnt[1] && cnt[3]) {
				printf("G\n");
			} else if(cnt[2] && cnt[3]) {
				printf("B\n");
			}
		}
	} else if ((cnt[1] >= 1) + (cnt[2] >= 1) + (cnt[3] >= 1) == 1) {
		// only one distinct value
		printf("%c\n", A[0]);
	} else assert(false);
}
