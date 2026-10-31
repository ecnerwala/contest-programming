#include <bits/stdc++.h>

std::optional<std::vector<int>> solve(int N, int K) {
	if (K % 2 == 0) return std::nullopt;
	if (N == K && K > 1) return std::nullopt;
	if (N == K && K == 1) return std::vector<int>({0, 1});

	std::vector<int> res; res.reserve(1 << N);
	int cur = 0;
	res.push_back(cur);
	for (int i = 0; i < (1 << (N-K-1)); i++) {
		for (int z = 1; z < (1 << K); z++) {
			cur ^= (1 << (K+1)) - 1 - 1;
			res.push_back(cur);
			cur ^= (1 << (K+1)) - 1 - (2 << __builtin_ctz(z));
			res.push_back(cur);
		}
		cur ^= (1 << (K+1)) - 1 - 1;
		res.push_back(cur);

		if (i+1 == (1 << (N-K-1))) {
			break;
		}
		cur ^= (1 << __builtin_ctz(i+1)) << (K+1);
		cur ^= (1 << (K-1)) - 1;
		res.push_back(cur);
	}
	return res;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	for (int N = 1; N <= 18; N++) {
		for (int K = 1; K <= N; K++) {
			auto res_opt = solve(N, K);
			if (res_opt) {
				auto res = *res_opt;
				assert(int(res.size()) == (1 << N));
				for (int i = 0; i < int(res.size()); i++) {
					assert(__builtin_popcount(res[i] ^ res[(i+1)%int(res.size())]) == K);
				}
				std::vector<bool> seen(1 << N);
				for (int v : res) {
					assert(!seen[v]);
					seen[v] = true;
				}
			}
		}
	}

	int N, K; cin >> N >> K;
	auto res_opt = solve(N, K);
	if (res_opt) {
		auto res = *res_opt;
		cout << "Yes" << '\n';
		for (int i = 0; i < int(res.size()); i++) {
			cout << res[i] << " \n"[i+1==int(res.size())];
		}
	} else {
		cout << "No" << '\n';
	}

	return 0;
}
