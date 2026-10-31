#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, Q; cin >> N >> Q;
		std::vector<int> par(N);
		par[0] = -1;
		for (int i = 1; i < N; i++) {
			cin >> par[i]; par[i]--;
		}
		std::vector<int> sz(N, 1);
		for (int i = N-1; i > 0; i--) sz[par[i]] += sz[i];

		// index to vertex
		std::vector<int> P(N);
		for (auto& a : P) { cin >> a; a--; }
		// vertex to index
		std::vector<int> label(N);
		for (int i = 0; i < N; i++) label[P[i]] = i;

		std::vector<std::set<std::tuple<int, int, int>>> ch(N);
		for (int i = 1; i < N; i++) {
			ch[par[i]].insert({label[i], label[i] + sz[i], i});
		}

		int num_good = 0;
		num_good += (label[0] == 0);
		for (int i = 0; i < N; i++) {
			int cur = label[i] + 1;
			for (auto [l1, l2, v] : ch[i]) {
				num_good += cur == l1;
				cur = l2;
			}
		}

		auto check_it = [&](int p, auto it) -> bool {
			if (it == ch[p].end()) return 0;
			return std::get<0>(*it) == (it == ch[p].begin() ? label[p] + 1 : std::get<1>(*std::prev(it)));
		};
		auto set_label = [&](int i, int old_label, int new_label) {
			assert(old_label == label[i]);
			label[i] = new_label;
			if (i > 0) {
				{
					auto it = ch[par[i]].find({old_label, old_label + sz[i], i});
					assert(it != ch[par[i]].end());
					num_good -= check_it(par[i], it);
					num_good -= check_it(par[i], std::next(it));
					it = ch[par[i]].erase(it);
					num_good += check_it(par[i], it);
				}
				{
					auto it = ch[par[i]].lower_bound({new_label, new_label + sz[i], i});
					num_good -= check_it(par[i], it);
					it = ch[par[i]].insert({new_label, new_label + sz[i], i}).first;
					num_good += check_it(par[i], it);
					num_good += check_it(par[i], std::next(it));
				}
			} else {
				num_good -= (old_label == 0);
				num_good += (new_label == 0);
			}
			// check your children
			if (!ch[i].empty()) {
				num_good -= (old_label + 1 == std::get<0>(*ch[i].begin()));
				num_good += (new_label + 1 == std::get<0>(*ch[i].begin()));
			}
		};

		for (int q = 0; q < Q; q++) {
			int x, y; cin >> x >> y; x--, y--;
			int orig_x = P[x];
			int orig_y = P[y];
			std::swap(P[x], P[y]);
			set_label(orig_x, x, y);
			set_label(orig_y, y, x);
			cout << (num_good == N ? "YES" : "NO") << '\n';
		}
	}

	return 0;
}
