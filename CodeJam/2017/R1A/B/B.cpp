#include<bits/stdc++.h>
using namespace std;

const int MAXN = 100;
const int MAXP = 100;
int N;
int P;
int R[MAXN];
vector<int> Q[MAXN];

int mi(int q, int r) {
	return (10 * q + 11 * r - 1) / (11 * r);
}
int ma(int q, int r) {
	return (10 * q) / (9 * r);
}

int go() {
	int res = 0;
	while(true) {
		int val = 1;
		for (int i = 0; i < N; i++) {
			if (Q[i].empty()) return res;
			val = max(val, mi(Q[i].back(), R[i]));
		}
		bool is_good = true;
		for (int i = 0; i < N; i++) {
			while (true) {
				if (Q[i].empty()) return res;
				if (ma(Q[i].back(), R[i]) < val) {
					Q[i].pop_back();
					continue;
				}
				if (mi(Q[i].back(), R[i]) <= val) {
					Q[i].pop_back();
				} else {
					is_good = false;
				}
				break;
			}
		}
		if (is_good) res++;
	}
	assert(false);
	return res;
}

int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;

	for(int case_num = 1; case_num <= T; case_num ++) {
		cin >> N >> P;
		assert(N < MAXN);
		assert(P < MAXP);

		for (int i = 0; i < N; i++) cin >> R[i];
		for (int i = 0; i < N; i++) {
			Q[i].resize(size_t(P));
			for (int j = 0; j < P; j++) {
				cin >> Q[i][size_t(j)];
			}
			sort(Q[i].begin(), Q[i].end());
			reverse(Q[i].begin(), Q[i].end());
		}

		int res = go();

		cout << "Case #" << case_num << ": " << res << '\n';
	}

	return 0;
}
