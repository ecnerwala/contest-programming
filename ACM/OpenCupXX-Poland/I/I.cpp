#include<bits/stdc++.h>
using namespace std;

const int MAXN = 1.1e6;
int N;
int P[MAXN];
int Q[MAXN];
char S[MAXN];

void go() {
	cin >> N;
	for (int i = 0; i < N; i++) {
		cin >> P[i];
	}
	S[N] = '\0';

	S[0] = S[1] = '0';
	for (int i = 1; i < N-1; i++) {
		if (P[i] == 0) {
			S[i+1] = S[i-1] ^ 1;
		} else {
			S[i+1] = S[i-1];
		}
	}

	for (int i = 0, l = 0, r = 0; i < N; i++) {
		int t = r - i;
		if (i < r) t = min(t, Q[l + t]);
		Q[i] = t;
		int L = i - Q[i], R = i + Q[i];
		while (L >= 1 && R + 1 < N && S[L-1] == S[R+1]) {
			Q[i] ++, L--, R++;
		}
		if (R > r) l = L, r = R;
		if (Q[i] != P[i]) {
			cout << 0 << '\n';
			return;
		}
	}
	cout << 4 << '\n';
	cout << S << '\n';
	for (int i = 1; i < N; i += 2) S[i] ^= 1;
	cout << S << '\n';
	for (int i = 0; i < N; i ++) S[i] ^= 1;
	cout << S << '\n';
	for (int i = 1; i < N; i += 2) S[i] ^= 1;
	cout << S << '\n';
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int Z; cin >> Z;
	while (Z--) {
		go();
	}

	return 0;
}
