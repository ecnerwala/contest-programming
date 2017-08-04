#include<bits/stdc++.h>
using namespace std;

const int MAXN = int(1e6) + 200;
const int MAXL = int(1e6) + 200;

int N;
int L[MAXN];
int S[MAXL];
int T[MAXL];

int main() {
	scanf("%d", &N);
	for (int i = 0; i < N; i++) {
		scanf("%d", &L[i]);
	}
	memset(S, -1, sizeof(S));
	memset(T, -1, sizeof(T));
	for (int i = 0; i < N; i++) {
		if (S[L[i]] == -1) S[L[i]] = i;
		T[L[i]] = i;
	}
	int res = 0;
	int k = 0;
	for (int i = 0; i < N; i++) {
		if (S[L[i]] < k) {
			continue;
		}
		if (T[L[i]] == i) {
			res ++;
			k = i+1;
		}
	}
	cout << res << '\n';

	return 0;
}
