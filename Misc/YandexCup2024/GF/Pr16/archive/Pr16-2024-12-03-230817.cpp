#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, L; cin >> N >> L;
	std::vector<std::vector<int>> A(N);
	for (int i = 0; i < N; i++) {
		std::string S; cin >> S;
		assert(int(S.size()) == L);
		A[i].resize(L);
		for (int j = 0; j < L; j++) {
			A[i][j] = S[j] == 'A' ? 0 : S[j] == 'C' ? 1 : S[j] == 'G' ? 2 : S[j] == 'T' ? 3 : 4;
			assert(A[i][j] != 4);
		}
	}
	A.push_back(std::vector<int>(L, 0));
	N++;
	std::vector<int> par(N, -1);
	auto get_par = [&](int a) -> int {
		while (par[a] >= 0) {
			if (par[par[a]] >= 0) par[a] = par[par[a]];
			a = par[a];
		}
		return a;
	};
	auto merge = [&](int a, int b) -> bool {
		a = get_par(a), b = get_par(b);
		if (a == b) return false;
		if (par[a] > par[b]) std::swap(a, b);
		par[a] += par[b];
		par[b] = a;
		return true;
	};
	std::vector<std::pair<int, std::array<int, 2>>> edges; edges.reserve(N*N);
	for (int i = 0; i < N; i++) {
		for (int j = i+1; j < N; j++) {
			int a = 0;
			for (int k = 0; k < L; k++) {
				int d = std::abs(A[i][k] - A[j][k]);
				d = std::min(d, 4-d);
				a += d;
			}
			edges.push_back({a, {i, j}});
		}
	}
	std::sort(edges.begin(), edges.end());
	int64_t ans = 0;
	for (auto [v, ps] : edges) {
		if (merge(ps[0], ps[1])) ans += v;
	}
	cout << ans << '\n';

	return 0;
}
