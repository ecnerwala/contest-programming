#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		int N, A, B, C; cin >> N >> A >> B >> C;

		assert(1 <= C && C <= N);
		assert(C <= A && A <= N);
		assert(C <= B && B <= N);

		cout << "Case #" << case_num << ": ";
		if ((A + B - C) > N || (A + B - C == 1 && N >= 2)) {
			cout << "IMPOSSIBLE" << '\n';
		} else if (N == 1) {
			cout << "1" << '\n';
		} else if (N == 2) {
			if (C == 2) {
				cout << "1 1" << '\n';
			} else if (A == 2) {
				cout << "1 2" << '\n';
			} else if (B == 2) {
				cout << "2 1" << '\n';
			} else assert(false);
		} else {
			vector<int> res; res.reserve(N);
			for (int i = 0; i < A-C; i++) res.push_back(2);
			for (int i = 0; i < C; i++) res.push_back(3);
			for (int i = 0; i < B-C; i++) res.push_back(2);
			int extra = N - (A + B - C);
			if (extra > 0) {
				res.insert(res.begin()+1, extra, 1);
			}
			for (int i = 0; i < N; i++) {
				cout << res[i] << " \n"[i+1==N];
			}
		}
	}

	return 0;
}
