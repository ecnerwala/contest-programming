#include <bits/stdc++.h>

using std::cerr;

const int C_LIM = 500;

std::vector<int64_t> simulate_full(std::vector<int64_t> V, int64_t K) {
	int64_t K_orig = K;
	std::sort(V.begin(), V.end());
	{
		int a = 0;
		while (a < int(V.size()) && V[a] == 0) a++;
		V.erase(V.begin(), V.begin() + a);
	}
	std::vector<int64_t> NV; NV.reserve(V.size());
	int num_one_round = 0;
	auto one_round = [&]() -> void {
		num_one_round++;
		NV.clear();
		for (int i = 0; i+1 < int(V.size()); i++) {
			V[i+1] -= V[i];
		}
		for (int i = 0, j = 1; i < int(V.size()) || j < int(V.size()); ) {
			if (i < int(V.size()) && (j >= int(V.size()) || V[i] < V[j])) {
				if (V[i]) NV.push_back(V[i]);
				i += 2;
			} else {
				if (V[j]) NV.push_back(V[j]);
				j += 2;
			}
		}
		std::swap(V, NV);
		K--;
	};
	while (K > 0 && int(V.size()) >= C_LIM) {
		one_round();
	}
	cerr << "num_one_round " << num_one_round << '\n';
	while (K > 0) {
		if (int(V.size()) <= 1) break;

		std::vector<std::vector<int64_t>> contribs;
		contribs.emplace_back(int(V.size()), int64_t(0));
		contribs[0][0] = 1;
		contribs[0][1] = 1;

		// set V = sim(V, 1 << k) and return true if it stays increasing
		auto maybe_jump = [&](int k) -> bool {
			if ((int64_t(1) << k) >= K) return false;
			NV.clear();
			for (int i = 0; i < int(V.size()); i++) {
				__int128_t cur = V[i];
				for (int d = 1; d <= i && d <= (int64_t(1) << k); d++) {
					cur -= __int128_t(contribs[k][d]) * NV[i-d];
					if (cur < 0) {
						return false;
					}
				}
				NV.push_back(int64_t(cur));
				if (i && NV[i-1] > NV[i]) return false;
			}
			V = std::move(NV);
			K -= int64_t(1) << k;
			assert(K >= 1);
			return true;
		};

		int k = 0;
		if (maybe_jump(0)) {
			while (true) {
				assert(k <= 100);
				if (!maybe_jump(k)) {
					for (--k; k >= 0; --k) {
						maybe_jump(k);
					}
					break;
				} else {
					// calculate contribs[k+1]
					contribs.emplace_back(int(V.size()), int64_t(0));
					for (int i = 0; i < int(V.size()) && i <= (int64_t(1) << k); i++) {
						for (int j = 0; i+j < int(V.size()) && j <= (int64_t(1) << k); j++) {
							contribs[k+1][i+j] += contribs[k][i] * contribs[k][j];
						}
					}
					continue;
				}
				assert(false);
			}
		}
		assert(K >= 1);
		one_round();
		cerr << K_orig - K << ' ' << int(V.size()) << '\n';
		for (auto v : V) { cerr << v << '\n'; }
	}
	return V;
}

void test() {
	std::mt19937_64 mt(48);
	for (int cases = 0; cases < 100; cases++) {
		int N = 100;
		std::vector<int64_t> V(N);
		for (auto& v : V) {
			int b = std::uniform_int_distribution<int>(0, 60)(mt);
			v = std::uniform_int_distribution<int64_t>(1, int64_t(1) << b)(mt);
		}
		cerr << "go " << N << '\n';
		auto r = simulate_full(V, 1e18);
		cerr << r.size() << '\n';
	}
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	test();

	int N; int64_t K; cin >> N >> K;
	std::vector<int64_t> V(N);
	for (auto& v : V) cin >> v;
	V = simulate_full(V, K-1);
	V.insert(V.begin(), N - int(V.size()), int64_t(0));
	for (int i = 0; i+1 < N; i++) {
		V[i+1] -= V[i];
	}
	for (int i = 0; i < N; i++) {
		cout << V[i] << " \n"[i+1==N];
	}

	return 0;
}
