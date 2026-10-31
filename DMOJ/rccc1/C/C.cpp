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

	int N, M; cin >> N >> M;
	vector<int> A(N); for (auto& a : A) { cin >> a; }

	vector<int> par(N, -1);
	auto get_par = [&](int a) -> int {
		while (par[a] >= 0) {
			if (par[par[a]] >= 0) par[a] = par[par[a]];
			a = par[a];
		}
		return a;
	};
	auto merge = [&](int a, int b) -> bool {
		a = get_par(a);
		b = get_par(b);
		if (a == b) return false;
		if (par[a] >= par[b]) swap(a, b);
		par[a] += par[b];
		par[b] = a;
		return true;
	};

	vector<vector<int>> adj(N);
	for (int e = 0; e < M; e++) {
		int u, v; cin >> u >> v; u--, v--;
		if (merge(u, v)) {
			adj[u].push_back(v);
			adj[v].push_back(u);
		}
	}

	vector<int> B(N, -1);
	for (int i = 0; i < N; i++) {
		if (get_par(i) != i) continue;
		vector<int> cur_cc;
		vector<int> vals;
		for (int j = 0; j < N; j++) {
			if (get_par(j) == i) {
				cur_cc.push_back(j);
				vals.push_back(A[j]);
			}
		}
		sort(vals.begin(), vals.end());
		for (int z = 0; z < int(cur_cc.size()); z++) {
			B[cur_cc[z]] = vals[z];
		}
	}

	vector<pair<int, int>> ops; ops.reserve(2*N);

	vector<int> deg(N);
	vector<int> q; q.reserve(N);
	for (int i = 0; i < N; i++) {
		deg[i] = int(adj[i].size());
		if (deg[i] == 1) q.push_back(i);
	}

	for (int z = 0; z < int(q.size()); z++) {
		int rt = q[z];
		int tgt = B[rt];
		std::y_combinator([&](auto self, int cur, int prv) -> void {
			if (A[cur] == tgt) return;
			for (int nxt : adj[cur]) {
				if (nxt == prv) continue;
				self(nxt, cur);
				if (A[nxt] == tgt) {
					ops.push_back({cur, nxt});
					swap(A[cur], A[nxt]);
					return;
				}
			}
		})(rt, -1);
		assert(A[rt] == tgt);
		for (int nxt : adj[rt]) {
			if (--deg[nxt] == 1) q.push_back(nxt);
		}
	}

	cout << ops.size() << '\n';
	for (int i = 0; i < N; i++) {
		cout << A[i] << " \n"[i+1==N];
	}
	for (auto it : ops) {
		cout << it.first+1 << ' ' << it.second+1 << '\n';
	}

	return 0;
}
