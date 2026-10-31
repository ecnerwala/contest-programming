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
	vector<vector<int>> adj(N);
	for (int e = 0; e < N-1; e++) {
		int u, v; cin >> u >> v; u--, v--;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}
	auto FAIL = []() {
		cout << -1 << '\n';
		exit(0);
	};
	for (int i = 0; i < N; i++) {
		if (adj[i].size() >= 4) {
			FAIL();
		}
	}

	int ROOT = 0;
	while (ROOT < N && adj[ROOT].size() >= 3) ROOT++;
	if (ROOT == N) {
		FAIL();
	}

	vector<int> par(N);
	vector<optional<pair<int, int>>> dp_down(N);
	vector<optional<pair<int, int>>> dp_up(N);
	auto merge = [&](int cur, optional<pair<int, int>> a, optional<pair<int, int>> b) -> optional<pair<int, int>> {
		if (!a || !b) return std::nullopt;
		if (b->second < a->first) {
			if (a->first != cur) return std::nullopt;
			a->first = b->first;
			return a;
		} else if (b->first > a->second) {
			if (a->second != cur) return std::nullopt;
			a->second = b->second;
			return a;
		} else {
			return std::nullopt;
		}
	};
	std::y_combinator([&](auto self, int cur, int prv) -> void {
		par[cur] = prv;
		dp_down[cur] = {{cur, cur}};
		for (int nxt : adj[cur]) {
			if (nxt == prv) continue;
			self(nxt, cur);
			dp_down[cur] = merge(cur, dp_down[cur], dp_down[nxt]);
		}
	})(ROOT, -1);
	vector<bool> is_good(N);
	std::y_combinator([&](auto self, int cur) -> void {
		{
			optional<pair<int, int>> state{{cur, cur}};
			for (int nxt : adj[cur]) {
				state = merge(cur, state, (nxt == par[cur]) ? dp_up[cur] : dp_down[nxt]);
			}
			if (state) {
				is_good[cur] = true;
			}
		}

		for (int ch : adj[cur]) {
			if (ch == par[cur]) continue;
			dp_up[ch] = {{cur, cur}};
			for (int nxt : adj[cur]) {
				if (ch == nxt) continue;
				dp_up[ch] = merge(cur, dp_up[ch], (nxt == par[cur]) ? dp_up[cur] : dp_down[nxt]);
			}
			self(ch);
		}
	})(ROOT);
	/*
	for (int i = 0; i < N; i++) {
		cerr << i << ": ";
		if (dp_down[i]) cerr << dp_down[i]->first << '-' << dp_down[i]->second << '\n';
		else cerr << "bad" << '\n';
	}
	for (int i = 0; i < N; i++) {
		cerr << i << ": ";
		if (dp_up[i]) cerr << dp_up[i]->first << '-' << dp_up[i]->second << '\n';
		else cerr << "bad" << '\n';
	}
	*/

	vector<int> roots; roots.reserve(N);
	for (int i = 0; i < N; i++) if (is_good[i]) roots.push_back(i);
	if (roots.empty()) FAIL();
	for (int i = 0; i < int(roots.size()); i++) {
		cout << roots[i]+1 << ' ';
	}
	cout << '\n';

	return 0;
}
