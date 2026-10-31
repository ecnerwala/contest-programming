#include <bits/stdc++.h>

namespace std {

template<class Fun>
class y_combinator_result {
	Fun fun_;
public:
	template<class T>
	explicit y_combinator_result(T &&fun): fun_(std::forward<T>(fun)) {}

	template<class ...Args>
	decltype(auto) operator()(Args &&...args) {
		return fun_(std::ref(*this), std::forward<Args>(args)...);
	}
};

template<class Fun>
decltype(auto) y_combinator(Fun &&fun) {
	return y_combinator_result<std::decay_t<Fun>>(std::forward<Fun>(fun));
}

} // namespace std

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<std::pair<int, int>> A(N);
	std::vector<std::array<int, 2>> ch(N);
	std::vector<int> par(N, -1);
	for (int i = 0; i < N; i++) {
		cin >> A[i].first; A[i].second = i;
		cin >> ch[i][0] >> ch[i][1]; ch[i][0]--, ch[i][1]--;
		if (ch[i][0] != -1) par[ch[i][0]] = i;
		if (ch[i][1] != -1) par[ch[i][1]] = i;
	}

	std::sort(A.begin(), A.end());
	std::vector<int> V(N);
	for (int i = 0; i < N; i++) V[A[i].second] = i;

	int root = 0;
	while (par[root] != -1) root++;
	std::vector<int> idx(N);
	std::vector<int> ordering(N);
	{
		int nxt_idx = 0;
		std::y_combinator([&](auto self, int cur) -> void {
			if (cur == -1) return;
			self(ch[cur][0]);

			ordering[nxt_idx] = cur;
			idx[cur] = nxt_idx;
			nxt_idx++;

			self(ch[cur][1]);
		})(root);
		assert(nxt_idx == N);
	}

	auto d = [&](int a) -> bool {
		return a == ch[par[a]][1];
	};

	std::vector<std::pair<bool, int>> ops;

	auto rot = [&](int a) -> void {
		ops.push_back({false, a});
		assert(par[a] != -1);
		int p = par[a];
		int gp = par[p];

		int x = d(a);
		int c = ch[a][!x];

		par[a] = gp;
		if (gp != -1) ch[gp][d(p)] = a;

		if (c != -1) par[c] = p;
		ch[p][x] = c;

		par[p] = a;
		ch[a][!x] = p;
	};

	auto splay_to = [&](int a, int top) -> void {
		while (par[a] != top) {
			if (par[par[a]] != top) {
				if (d(a) == d(par[a])) rot(par[a]);
				else rot(a);
			}
			rot(a);
		}
	};

	for (int i = 0; i < N; i++) {
		if (V[i] != idx[i]) {
			splay_to(i, -1);
			while (V[i] != idx[i]) {
				int v = ordering[V[i]];
				splay_to(v, i);
				assert(par[v] == i);
				assert(V[i] == idx[v]);
				ops.push_back({true, v});
				std::swap(V[v], V[i]);
			}
		}
	}

	cout << ops.size() << '\n';
	for (auto [t, i] : ops) {
		cout << (t ? "swap" : "rotate") << ' ' << i+1 << '\n';
	}

	return 0;
}
