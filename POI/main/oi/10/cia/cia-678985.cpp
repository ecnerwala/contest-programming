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
	scanf("%d", &N);
	if(N <= 1) putchar('1');
	else if(N <= 3) putchar('2');
	else putchar('3');
	putchar('\n');
	for(int i = 0; i < N; i++) {
		putchar('b' + __builtin_popcount(i + 1) - __builtin_popcount(i));
	}
	putchar('\n');
}
