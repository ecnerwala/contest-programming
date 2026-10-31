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
	vector<vector<int>> adj(N);
	for (int e = 0; e < M; e++) {
		int u, v; cin >> u >> v; u--, v--;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}

	vector<int8_t> vis(N, false);

	vector<int> root_msks(N);
	vector<int> root_id(N);
	vector<int> msk(N);
	vector<int> depth(N);
	for (int i = 0; i < N; i++) {
		if (vis[i]) continue;

		int root_msk = 0;
		bool has_extra = false;

		msk[i] = 0;
		depth[i] = 0;

		std::y_combinator([&](auto self, int cur) -> void {
			assert(!vis[cur]);
			vis[cur] = true;
			root_id[cur] = i;
			for (int nxt : adj[cur]) {
				if (vis[nxt]) {
					if (!has_extra && !((depth[cur] ^ depth[nxt]) & 1)) {
						has_extra = true;
						cout << "? " << cur+1 << " & " << nxt+1 << '\n' << flush;
						int resp; cin >> resp;
						if (resp == -1) exit(0);
						root_msk |= (resp ^ msk[cur]) & ~(msk[cur] ^ msk[nxt]);
					}
					continue;
				}

				cout << "? " << cur+1 << " & " << nxt+1 << '\n' << flush;
				int va; cin >> va;
				if (va == -1) exit(0);
				cout << "? " << cur+1 << " | " << nxt+1 << '\n' << flush;
				int vb; cin >> vb;
				if (vb == -1) exit(0);

				root_msk |= (va ^ msk[cur]) & (~(va ^ vb));

				msk[nxt] = msk[cur] ^ va ^ vb;
				depth[nxt] = depth[cur] ^ 1;
				self(nxt);
			}
		})(i);

		assert(has_extra);

		root_msks[i] = root_msk;
	}

	cout << "! ";
	for (int i = 0; i < N; i++) {
		cout << (msk[i] ^ root_msks[root_id[i]]) << " \n"[i+1==N];
	}
	cout << flush;

	return 0;
}
