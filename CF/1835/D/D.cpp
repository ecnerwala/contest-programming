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
	int64_t K; cin >> K;

	std::vector<std::vector<int>> adj(N);
	for (int e = 0; e < M; e++) {
		int u, v; cin >> u >> v; u--, v--;
		adj[u].push_back(v);
	}

	std::vector<int> cc_id(N, -1);
	std::vector<int> depth(N, -1);
	{
		std::vector<int> idx(N, -1);
		std::vector<int> lowval(N, -1);
		std::vector<int> in_stack(N, false);
		std::vector<int> stk; stk.reserve(N);
		int nxt_idx = 0;
		for (int rt = 0; rt < N; rt++) {
			if (idx[rt] == -1) {
				depth[rt] = 0;
				std::y_combinator([&](auto self, int cur) -> void {
					assert(depth[cur] != -1);
					idx[cur] = nxt_idx++;
					lowval[cur] = idx[cur];
					stk.push_back(cur);
					in_stack[cur] = true;
					for (int nxt : adj[cur]) {
						if (idx[nxt] == -1) {
							depth[nxt] = depth[cur] + 1;
							self(nxt);
							lowval[cur] = std::min(lowval[cur], lowval[nxt]);
						} else if (in_stack[nxt]) {
							lowval[cur] = std::min(lowval[cur], idx[nxt]);
						}
					}
					assert(lowval[cur] <= idx[cur]);
					if (lowval[cur] == idx[cur]) {
						while (true) {
							int v = stk.back();
							stk.pop_back();
							assert(idx[v] >= idx[cur]);
							assert(in_stack[v]);
							in_stack[v] = false;
							assert(cc_id[v] == -1);
							cc_id[v] = cur;
							if (v == cur) break;
						}
					}
				})(rt);
			}
		}
	}

	std::vector<int> vals(N, 0);
	std::vector<int> cc_sz(N, 0);
	for (int i = 0; i < N; i++) {
		cc_sz[cc_id[i]]++;
		for (int z = 0; z < int(adj[i].size()); z++) {
			int j = adj[i][z];
			if (cc_id[j] == cc_id[i]) {
				int d = std::abs(depth[j] - depth[i] - 1);
				if (d) vals[cc_id[i]] = std::gcd(vals[cc_id[i]], d);
			}
		}
	}

	std::vector<std::vector<int>> cnts(N);
	for (int rt = 0; rt < N; rt++) {
		if (cc_id[rt] != rt) continue;
		if (vals[rt] == 0) {
			assert(cc_sz[rt] == 1);
			continue;
		}
		cnts[rt].resize(vals[rt]);
	}

	for (int i = 0; i < N; i++) {
		if (vals[cc_id[i]] == 0) continue;
		cnts[cc_id[i]][depth[i] % vals[cc_id[i]]]++;
	}

	int64_t ans = 0;
	for (int rt = 0; rt < N; rt++) {
		if (cc_id[rt] != rt) continue;
		if (vals[rt] == 0) continue;

		int d = int(K % vals[rt]);
		if (d == 0) {
			for (int v = 0; v < vals[rt]; v++) {
				int a = cnts[rt][v];
				ans += int64_t(a) * int64_t(a + 1) / 2;
			}
		} else if (d * 2 == vals[rt]) {
			for (int v = 0; v < vals[rt] / 2; v++) {
				int u = (v + d) % vals[rt];
				ans += int64_t(cnts[rt][v]) * int64_t(cnts[rt][u]);
			}
		}
	}
	cout << ans << '\n';

	return 0;
}
