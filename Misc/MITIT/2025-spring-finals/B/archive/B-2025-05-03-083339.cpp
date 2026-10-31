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
			//cerr << "solve " << z << '\n';
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

			if (false) {
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
			std::vector<int> max_reach(N+1, 0);
			for (int i = 0; i <= N; i++) max_reach[i] = i;
			for (int i = 0; i <= N; i++) {
				if (par[i] < 0) {
					auto [l, r] = bounds[i];
					min_range[l] = std::min(min_range[l], r);
					max_reach[l] = std::max(max_reach[l], r);
					//cerr << "bounds " << l << ' ' << r << '\n';
				}
			}
			for (int i = N; i > 0; i--) {
				min_range[i-1] = std::min(min_range[i-1], min_range[i]);
			}
			for (int i = 0; i < N; i++) {
				max_reach[i+1] = std::max(max_reach[i+1], max_reach[i]);
			}

			std::vector<int> n_ans_pref(N+1);
			for (int i = 0; i <= N; i++) {
				// now, anything that strictly includes bounds is bad
				n_ans_pref[i] ^= ans[i];
				int r = i+1 <= N ? min_range[i+1] : N;
				r = std::min(r, max_reach[i]);
				//cerr << i << " -> " << r << '\n';
				if (r < N) n_ans_pref[r+1] ^= ans[i];
			}

			int cur = 0;
			for (int i = 0; i <= N; i++) {
				cur ^= n_ans_pref[i];
				ans[i] = cur;
			}

			for (int i = 0; i <= N; i++) if (!can_right[i]) ans[i] = 0;
			//for (int i = 0; i <= N; i++) { cerr << can_left[i] << ' '; } cerr << '\n';
			//for (int i = 0; i <= N; i++) { cerr << can_right[i] << ' '; } cerr << '\n';
			//for (int i = 0; i <= N; i++) { cerr << ans[i] << ' '; } cerr << '\n';
			} else {
				std::vector<int> nans(N+1);
				for (int i = 0; i <= N; i++) {
					for (int j = i; j <= N; j++) {
						if (can_left[i] && can_right[j]) {
							std::vector<int> par(N+1, -1);
							std::vector<std::array<int, 2>> bounds(N+1);
							for (int k = 0; k <= N; k++) bounds[k] = {k, k};
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
								bounds[a][0] = std::min(bounds[a][0], bounds[b][0]);
								bounds[a][1] = std::max(bounds[a][1], bounds[b][1]);
								return true;
							};
							int cnt_edges = 0;
							int cnt_tree = 0;
							for (auto [u, v] : B) {
								assert(v != i);
								assert(u != j);
								if (v < i || u > j) continue;
								u = std::max(u, i);
								v = std::min(v, j);
								cnt_edges++;
								cnt_tree += merge(u, v);
							}
							assert(cnt_edges == j - i);
							if (cnt_tree == cnt_edges) {
								nans[j] ^= ans[i];
							} else {
								bool found = false;
								for (int k = 0; k <= N; k++) {
									if (par[k] < 0 && i < bounds[k][0] && bounds[k][1] < j) found = true;
								}
								assert(found == true);
							}
						}
					}
				}
				ans = nans;
			}
		}
		cout << ans[N] << '\n';
	}

	return 0;
}
