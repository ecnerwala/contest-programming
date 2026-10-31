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

	int E, V, B; cin >> E >> V >> B; B--;

	std::vector<std::vector<std::pair<int, int>>> adj(V);
	std::vector<int> in_deg(V);

	for (int e = 0; e < E; e++) {
		string S; cin >> S;
		int st = 0;
		while (S[st] != '1') st++;
		for (int i = 0; i < int(S.size()); i++) {
			if (S[i] - '0' != (i % V == st)) {
				cout << -1 << '\n';
				exit(0);
			}
		}
		int en = (V-1) - (int(S.size()) - 1 - st) % V;
		adj[st].push_back({en, e});
		in_deg[en] ++;
	}

	for (int i = 0; i < V; i++) {
		if (int(adj[i].size()) != in_deg[i]) {
			cout << -1 << '\n';
			exit(0);
		}
		std::reverse(adj[i].begin(), adj[i].end());
	}

	std::vector<int> ans; ans.reserve(E);
	std::y_combinator([&](auto self, int cur) -> void {
		while (!adj[cur].empty()) {
			auto [nxt, e] = adj[cur].back(); adj[cur].pop_back();
			self(nxt);
			ans.push_back(e);
		}
	})(B);
	if (int(ans.size()) < E) {
		cout << -1 << '\n';
		exit(0);
	}
	std::reverse(ans.begin(), ans.end());
	for (int e = 0; e < E; e++) {
		cout << ans[e]+1 << " \n"[e+1==E];
	}
	return 0;
}
