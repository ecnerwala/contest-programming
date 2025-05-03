#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<std::array<int, 6>> V(N);
		for (int i = 0; i < N; i++) {
			V[i][0] = 0;
			for (int z = 0; z < 5; z++) {
				int a; cin >> a;
				V[i][z+1] = V[i][z] + a;
			}
			assert(V[i][5] == N);
		}

		std::vector<int> ans(N+1);
		ans[0] = 1;
		for (int z = 4; z >= 0; z--) {
			cerr << "solve " << z << '\n';
			std::vector<std::array<int, 2>> B(N);
			std::vector<int> lefts(N+1);
			std::vector<int> rights(N+1);
			for (int i = 0; i < N; i++) {
				B[i] = {V[i][z], V[i][z+1]};
				lefts[i] = V[i][z];
				rights[i] = V[i][z+1];
			}
			lefts[N] = N+1;
			rights[N] = -1;
			std::sort(lefts.begin(), lefts.end());
			std::sort(rights.begin(), rights.end());
			std::vector<bool> can_right(N+1);
			for (int i = 0; i <= N; i++) can_right[i] = lefts[i] > i && (i == 0 || lefts[i-1] < i);
			std::vector<bool> can_left(N+1);
			for (int i = 0; i <= N; i++) can_left[i] = rights[i] < i && (i == N || rights[i+1] > i);

			std::vector<int> par(N+1, -1);
			std::vector<std::array<int, 2>> bounds(N+1);
			for (int i = 0; i <= N; i++) bounds[i] = {i, i};
			auto get_par = [&](int a) -> int {
				while (par[a] >= 0) {
					if (par[par[a]] >= 0) par[a] = par[par[a]];
					a = par[a];
				}
				return a;
			};
			for (auto [u, v] : B) {
				u = get_par(u);
				v = get_par(v);
				if (u == v) continue;
				if (par[u] > par[v]) std::swap(u, v);
				par[u] += par[v];
				par[v] = u;
				bounds[u][0] = std::min(bounds[u][0], bounds[v][0]);
				bounds[u][1] = std::max(bounds[u][1], bounds[v][1]);
			}

			for (int i = 0; i <= N; i++) if (!can_left[i]) ans[i] = 0;

			std::vector<int> min_range(N+1, N);
			for (int i = 0; i <= N; i++) {
				if (par[i] < 0) {
					auto [l, r] = bounds[i];
					min_range[l] = std::min(min_range[l], r);
					cerr << "bounds " << l << ' ' << r << '\n';
				}
			}
			for (int i = N; i > 0; i--) {
				min_range[i-1] = std::min(min_range[i-1], min_range[i]);
			}

			std::vector<int> n_ans_pref(N+1);
			for (int i = 0; i <= N; i++) {
				// now, anything that strictly includes bounds is bad
				n_ans_pref[i] ^= ans[i];
				int r = i+1 <= N ? min_range[i+1] : N;
				cerr << i << " -> " << r << '\n';
				if (r < N) n_ans_pref[r+1] ^= ans[i];
			}

			int cur = 0;
			for (int i = 0; i <= N; i++) {
				cur ^= n_ans_pref[i];
				ans[i] = cur;
			}

			for (int i = 0; i <= N; i++) if (!can_right[i]) ans[i] = 0;
			for (int i = 0; i <= N; i++) { cerr << can_left[i] << ' '; } cerr << '\n';
			for (int i = 0; i <= N; i++) { cerr << can_right[i] << ' '; } cerr << '\n';
			for (int i = 0; i <= N; i++) { cerr << ans[i] << ' '; } cerr << '\n';
		}
		cout << ans[N] << '\n';
	}

	return 0;
}
