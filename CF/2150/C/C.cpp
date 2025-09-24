#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int64_t> V(N); for (auto& v : V) cin >> v;
		std::vector<int> A(N); for (auto& a : A) { cin >> a; a--; }
		std::vector<int> B(N); for (auto& b : B) { cin >> b; b--; }

		std::vector<int> invB(N);
		for (int i = 0; i < N; i++) invB[B[i]] = i;

		int64_t tot = 0;
		std::map<int, int64_t> deltas;
		for (int z = 0; z < N; z++) {
			int cur = A[z];
			int64_t v = V[cur];
			int b = invB[cur];
			tot += v;
			deltas[b] -= v;
			auto it = deltas.find(b);
			while (it->second < 0) {
				auto nit = std::next(it);
				if (nit == deltas.end()) {
					deltas.erase(it);
					break;
				}
				int64_t d = std::min(-it->second, nit->second);
				it->second += d;
				nit->second -= d;
				if (nit->second == 0) {
					deltas.erase(nit);
				}
				if (it->second == 0) {
					deltas.erase(it);
					break;
				}
			}
		}
		for (auto [k, v] : deltas) tot += v;
		cout << tot << '\n';
	}

	return 0;
}
