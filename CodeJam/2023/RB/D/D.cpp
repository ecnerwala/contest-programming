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

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		int N, L; cin >> N >> L;
		int V = N+L;
		std::vector<std::vector<int>> adj(V);
		for (int i = 0; i < L; i++) {
			int k; cin >> k;
			adj[N+i].reserve(k);
			for (int j = 0; j < k; j++) {
				int s; cin >> s; s--;
				adj[N+i].push_back(s);
				adj[s].push_back(N+i);
			}
		}

		int ans = 0;
		std::vector<int> depth(V, -1);
		std::vector<int> lowval(V);
		for (int st = 0; st < N; st++) {
			if (depth[st] != -1) continue;
			depth[st] = 0;
			std::y_combinator([&](auto self, int cur, int prv) -> void {
				lowval[cur] = depth[cur];
				int num_bccs = (prv != -1);
				for (int nxt : adj[cur]) {
					if (nxt == prv) continue;
					if (depth[nxt] == -1) {
						depth[nxt] = depth[cur] + 1;
						self(nxt, cur);
						lowval[cur] = std::min(lowval[cur], lowval[nxt]);
						if (lowval[nxt] >= depth[cur]) {
							num_bccs++;
						}
					} else if (depth[nxt] < depth[cur]) {
						lowval[cur] = std::min(lowval[cur], depth[nxt]);
					}
				}
				if (num_bccs >= 2) {
					if (cur >= N) ans++;
				}
			})(st, -1);
		}

		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}
