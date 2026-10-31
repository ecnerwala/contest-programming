#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		int K; cin >> K;
		assert(K % 2 == 0);

		int N = 2;
		vector<pair<int, int>> edges;
		edges.push_back({1, 2});
		int cur_K = K;
		while (cur_K > 0) {
			int hub = N;
			int D1 = 1;
			assert(D1 * (D1-1) <= cur_K);
			while (D1 * (D1+1) <= cur_K) {
				edges.push_back({hub, ++N});
				D1++;
			}
			assert(D1 * (D1-1) <= cur_K);
			assert(D1 * (D1+1) > cur_K);
			assert(D1 > 1);
			cur_K -= D1 * (D1-1);
		}
		assert(cur_K == 0);
		assert(N <= 2000);
		assert(int(edges.size()) <= 2000);

		cout << "Case #" << case_num << ": " << N << ' ' << int(edges.size()) << '\n';
		for (auto [x, y] : edges) {
			cout << x << ' ' << y << '\n';
		}
	}

	return 0;
}

// hub with deg d_1 and hub with deg d_2 and they share x leaves
