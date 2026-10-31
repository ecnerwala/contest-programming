#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	std::array<std::array<std::vector<std::pair<int, int>>, 21>, 21> strings;
	for (int N = 1; N <= 20; N++) {
		std::vector<int> used(1 << N);
		for (int m = 0; m < (1 << N); m++) {
			if (used[m]) continue;
			int K = __builtin_popcount(m);
			int cur = m;
			int cnt = 0;
			for (; !used[cur]; cur = (cur >> 1) | ((cur & 1) << (N-1))) {
				used[cur] = true;
				cnt++;
			}
			assert(cur == m);
			strings[N][K].push_back({m, cnt});
		}
	}

	int T; cin >> T;
	while (T--) {
		int N, K, M; cin >> N >> K >> M;
		std::vector<int> s; s.reserve(M);
		for (auto [m, cnt] : strings[N][K]) {
			if (m == (1 << K) - 1) continue;
			if (int(s.size()) + cnt <= M) {
				int cur = m;
				for (int z = 0; z < cnt; z++) {
					s.push_back(cur);
					cur = (cur >> 1) | ((cur & 1) << (N-1));
				}
				assert(cur == m);
			}
		}
		assert(int(s.size()) <= M && M <= int(s.size()) + N);
		int g = std::gcd(K, N);
		int cur = (1 << K) - 1;
		for (int z = 0; int(s.size()) < M; z++) {
			s.push_back(cur);
			cur = (cur >> K) | ((cur & ((1 << K) - 1)) << (N-K));
			if ((z+1) % (N/g) == 0) {
				// rotate by 1 extra
				cur = (cur >> 1) | ((cur & 1) << (N-1));
			}
		}
		cout << (K * M + N - 1) / N << '\n';
		for (auto m : s) {
			for (int i = 0; i < N; i++) cout << ((m >> i) & 1);
			cout << '\n';
		}
	}

	return 0;
}
