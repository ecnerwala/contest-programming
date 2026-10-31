#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		int K; cin >> K;
		assert(K % 2 == 0);

		int D1 = 1;
		while (D1 * (D1+1) <= K) D1++;
		int K2 = K - D1 * (D1-1);
		int D2 = 1;
		while (D2 * (D2+1) <= K2) D2++;
		int K3 = K2 - D2 * (D2-1);

		assert(K3 % 2 == 0);
		int overlap = K3 / 2;
		assert(overlap <= D2-1 && D2 <= D1);

		cerr << D1 << ' ' << D2 << '\n';

		int N = 2 + (D1-1) + (D2-1) - overlap;
		int M = 1 + (D1-1) + (D2-1);
		vector<pair<int, int>> edges; edges.reserve(M);
		edges.push_back({1, 2});
		for (int i = 0; i < D1-1; i++) {
			edges.push_back({1, 3+i});
		}
		for (int i = 0; i < D2-1; i++) {
			edges.push_back({2, N-i});
		}

		assert(int(edges.size()) == M);
		assert(N <= 2000);
		assert(M <= 2000);
		cout << "Case #" << case_num << ": " << N << ' ' << int(edges.size()) << '\n';
		for (auto [x, y] : edges) {
			cout << x << ' ' << y << '\n';
		}
	}

	return 0;
}

// hub with deg d_1 and hub with deg d_2 and they share x leaves
