#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		cout << "Case #" << case_num << ": ";

		int N, C; cin >> N >> C;
		C -= N-1;

		if (!(0 <= C && C <= N*(N-1)/2)) {
			cout << "IMPOSSIBLE" << '\n';
			continue;
		}

		vector<int> L(N); iota(L.begin(), L.end(), 1);
		for (int i = N-2; i >= 0; i--) {
			int v = min(C, N-1-i);
			C -= v;
			reverse(L.begin()+i, L.begin()+i+v+1);
		}
		assert(C == 0);
		for (int i = 0; i < N; i++) {
			cout << L[i] << " \n"[i+1==N];
		}
	}

	return 0;
}
