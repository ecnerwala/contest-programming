#include<bits/stdc++.h>
using namespace std;

const int MAXL = 2e3;
int L;
string S;
int K;
int V[MAXL];

int go() {
	L = int(S.size()) + 1;
	S = '+' + S + '+';
	for (int i = 0; i < L; i++) {
		V[i] = (S[i] != S[i+1]);
	}
	int res = 0;
	for (int i = 0; i < L; i++) {
		if (V[i] != 0) {
			if (i + K >= L) return -1;
			V[i + K] ^= 1;
			res ++;
		}
	}
	return res;
}

int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;

	for(int case_num = 1; case_num <= T; case_num ++) {
		cin >> S >> K;
		int res = go();
		cout << "Case #" << case_num << ": ";
		if (res == -1) cout << "IMPOSSIBLE";
		else cout << res;
		cout << endl;
	}

	return 0;
}
