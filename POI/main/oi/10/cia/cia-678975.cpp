#include<bits/stdc++.h>
using namespace std;

const int MAXN = 2e7;
int N;
char S[MAXN];
const char perm[4][3] = {
	{'a', 'b', 'c'},
	{'b', 'c', 'a'},
	{'c', 'a', 'b'},
	{'a', 'c', 'b'},
};

bool check() {
	for(int i = 0; i < N; i++) {
		for(int j = i + 1; j + (j - i) <= N; j++) {
			bool good = true;
			for(int k = i; k < j; k++) {
				if(S[j + (k - i)] != S[k]) {
					good = false;
					break;
				}
			}
			if(good) return false;
		}
	}
	return true;
}

int main() {
	memcpy(S, "abacbabcabacbab", 16);
	scanf("%d", &N);
	if(N <= 1) printf("1\n");
	else if(N <= 3) printf("2\n");
	else printf("3\n");
	for(int i = 0; i < N; i++) {
		if((i & 15) == 15) S[i] = 'd';
		else {
			S[i] = perm[S[i / 16] - 'a'][S[i & 15] - 'a'];
		}
	}

	S[N] = 0;
	//assert(check());
	printf("%s\n", S);
}
