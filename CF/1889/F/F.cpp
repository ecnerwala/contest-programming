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

struct frac_t {
	int64_t num, den;
	friend auto operator == (const frac_t& a, const frac_t& b) {
		return a.num * b.den == b.num * a.den;
	}
	friend auto operator <=> (const frac_t& a, const frac_t& b) {
		return a.num * b.den <=> b.num * a.den;
	}
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, R, K; cin >> N >> R >> K; R--;
		std::vector<int> W(N);
		for (auto& w : W) cin >> w;
		std::vector<std::vector<int>> adj(N);
		for (int e = 0; e < N-1; e++) {
			int u, v; cin >> u >> v; u--, v--;
			adj[u].push_back(v);
			adj[v].push_back(u);
		}

		std::vector<std::vector<int>> ch(N);
		std::vector<int> par(N);
		std::vector<frac_t> vals(N);
		std::y_combinator([&](auto self, int cur, int prv) -> frac_t {
			par[cur] = prv;
			frac_t v = {W[cur], 1};
			for (int nxt : adj[cur]) {
				if (nxt == prv) continue;
				auto c = self(nxt, cur);
				v.num += c.num;
				v.den += c.den;
				ch[cur].push_back(nxt);
			}
			return vals[cur] = v;
		})(R, -1);

		std::vector<bool> cant_use(N);
		std::vector<bool> dead(N);
		std::vector<bool> has_must(N);
		int cur_K = 0;


		const frac_t INF = {int64_t(1e9), 1};

		for (int st = 0; st < N; st++) {
			// go up the tree til you get to a has_must
			if (dead[st]) continue;
			frac_t w_own = {W[st], 1};
			frac_t v_best = INF;
			frac_t v_must = INF;
			int cnt_must = 0;
			for (int cur = st; cur != -1; cur = par[cur]) {
				if (has_must[cur]) {
					assert(!cant_use[cur]);
					if (v_must == INF) {
						v_must = vals[cur];
					} else {
						assert(v_must == vals[cur]);
					}
					assert(!ch[cur].empty());
					for (int nxt : ch[cur]) {
						// TODO: We should subtract one for the current child?
						if (has_must[nxt]) {
							cnt_must++;
						}
					}
				} else if (cant_use[cur]) {
					assert(ch[cur].empty());
					assert(cur == st);
				} else {
					v_best = std::min(v_best, vals[cur]);
				}
			}

			if (cnt_must > 0) {
				assert(v_must != INF);
				if (w_own != v_must && cnt_must - 1 + cur_K > K) w_own = INF;
				if (v_best != v_must && cnt_must + cur_K > K) v_best = INF;
			}

			if (v_must <= v_best && v_must <= w_own) {
				// easiest case, just use it
				int cur = st;
				if (w_own != v_must) {
					// the first guy equal to v_must becomes a has_must, and its entire subtree dies
					while (vals[cur] != v_must) {
						cur = par[cur];
					}
					has_must[cur] = true;
					std::y_combinator([&](auto self, int a) -> void {
						for (int nxt : ch[a]) {
							self(nxt);
							dead[nxt] = true;
						}
						ch[a].clear();
					})(cur);
				} else {
					// don't change anything, just delete guys along the path which are worse than v_must
				}
				while (!has_must[cur]) {
				}
				if (vals[cur] < v_must) {
					// cur becomes a cant_use
				}
			}
			if (w_own <= v_best && w_own < v_must && cur_K + cnt_must <= K) {
			}
		}
	}

	return 0;
}
