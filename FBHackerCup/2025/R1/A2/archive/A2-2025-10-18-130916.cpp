#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		int N; cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) cin >> a;

		std::vector<int> par(N+1, -1);
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

		std::vector<std::pair<int, std::array<int, 2>>> edges; edges.reserve(N + (N-1));
		for (int i = 0; i < N; i++) {
			edges.push_back({A[i], {i, N}});
			if (i+1 < N) {
				edges.push_back({std::abs(A[i] - A[i+1]), {i, i+1}});
			}
		}
		std::sort(edges.begin(), edges.end());

		int ans = 0;
		int cnt = N+1;
		assert(cnt > 1);
		for (auto [w, vs] : edges) {
			ans = w;
			cnt -= merge(vs[0], vs[1]);
			if (cnt == 1) break;
		}
		assert(cnt == 1);

		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}
