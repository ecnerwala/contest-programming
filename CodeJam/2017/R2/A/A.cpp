#include<bits/stdc++.h>
using namespace std;

const int MAXN = 200;
const int MAXP = 10;
int N, P;

int G[MAXN];
int C[MAXP];

int slow4() {
	assert(P == 4);
	memset(C, 0, sizeof(C));
	for (int i = 0; i < N; i++) {
		C[G[i] % P]++;
	}
	int res = C[0];
	while (C[1] && C[3]) {
		C[1] --, C[3] --;
		res++;
	}
	if (C[1] < C[3]) swap(C[1], C[3]);
	assert(C[1] >= C[3]);
	while (C[1] >= 2 && C[2] >= 1) {
		C[1] -= 2, C[2] -= 1;
		res++;
	}
	while (C[1] >= 4) {
		C[1] -= 4;
		res++;
	}
	while (C[2] >= 2) {
		C[2] -= 2;
		res++;
	}
	return res + (C[1] || C[2] || C[3]);
}

int go() {
	assert(2 <= P && P <= 4);
	memset(C, 0, sizeof(C));
	for (int i = 0; i < N; i++) {
		C[G[i] % P]++;
	}
	if (P == 2) {
		// 0 or 1 + 1
		int res = C[0] + C[1] / 2 + (C[1] % 2);
		return res;
	} else if (P == 3) {
		// 0 or 1 + 2 or 1 + 1 + 1 or 2 + 2 + 2
		int res = C[0];
		res += min(C[1], C[2]);
		res += (abs(C[1] - C[2]) + 2) / 3;
		return res;
	} else if (P == 4) {
		// 0 or 1 + 3 or 2 + 2 or 1 + 1 + 2 or 3 + 3 + 2 or 1 * 4 or 3 * 4
		// 1 + 1 + 2, 3 + 3 + 2 => 1 + 3, 1 + 3, 2 + 2
		// 1 + 1 + 2, 3 * 4 => 1 + 3, 1 + 3, 3 + 3 + 2
		// 1 * 4, 3 * 4 => (1 + 3) * 4
		// only 1 + 1 + 2's and 1 * 4's OR 3 + 3 + 2's and 3 * 4's
		int res = C[0];
		res += min(C[1], C[3]);
		int rem = abs(C[1] - C[3]);
		bool extra = rem % 2 == 1;
		rem /= 2;
		res += min(rem, C[2]);
		rem = abs(rem - C[2]);
		extra = extra || (rem % 2 == 1);
		res += rem / 2;
		res += extra;
		assert(res == slow4());
		return res;
	} else assert(false);
}

int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;

	for(int case_num = 1; case_num <= T; case_num ++) {
		cin >> N >> P;
		assert(2 <= P && P <= 4);
		for (int i = 0; i < N; i++) {
			cin >> G[i];
		}
		
		int res = go();
		cout << "Case #" << case_num << ": " << res << '\n';
	}

	return 0;
}
