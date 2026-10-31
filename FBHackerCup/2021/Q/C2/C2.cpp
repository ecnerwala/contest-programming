#include<bits/stdc++.h>

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

template <typename T> void setmax(T& a, T b) { if (b > a) a = b; }

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		int N, K; cin >> N >> K;
		std::vector<int> C(N);
		for (int i = 0; i < N; i++) {
			cin >> C[i];
		}
		std::vector<std::vector<int>> adj(N);
		for (int e = 0; e < N-1; e++) {
			int u, v; cin >> u >> v; u--, v--;
			adj[u].push_back(v);
			adj[v].push_back(u);
		}

		//const int INF = std::accumulate(C.begin(), C.end(), 100);
		const int INF = int(1e9) + 20;

		auto conv = [&](const std::vector<int>& a, const std::vector<int>& b) -> std::vector<int> {
			if (a.empty() || b.empty()) return {};
			std::vector<int> c(a.size()+b.size()-1, -INF);
			for (int i = 0; i < int(a.size()); i++) {
				for (int j = 0; j < int(b.size()); j++) {
					if (a[i] < 0 || b[j] < 0) continue;
					setmax(c[i+j], a[i]+b[j]);
				}
			}
			return c;
		};
		auto sum = [&](const std::vector<int>& a, const std::vector<int>& b) -> std::vector<int> {
			std::vector<int> r;
			if (a.size() < b.size()) {
				r = b;
				for (int i = 0; i < int(a.size()); i++) setmax(r[i], a[i]);
			} else {
				r = a;
				for (int i = 0; i < int(b.size()); i++) setmax(r[i], b[i]);
			}
			return r;
		};

		struct dp_t {
			std::vector<int> unused;
			std::vector<int> used_odd;
			std::vector<int> used_even;
		};
		auto root_resp = std::y_combinator([&](auto self, int cur, int prv) -> dp_t {
			dp_t resp;
			resp.unused = std::vector<int>({0});
			resp.used_odd = std::vector<int>({-INF, 0});
			for (int nxt : adj[cur]) {
				if (nxt == prv) continue;
				auto ch = self(nxt, cur);
				auto no_edge = sum(ch.unused, sum(ch.used_even, ch.used_odd));
				ch.used_even.insert(ch.used_even.begin(), -INF);
				ch.used_odd.erase(ch.used_odd.begin());
				auto with_edge = sum(ch.used_odd, ch.used_even);

				resp = dp_t {
					conv(resp.unused, no_edge),
					sum(conv(resp.used_odd, no_edge), conv(sum(resp.unused, resp.used_even), with_edge)),
					sum(conv(resp.used_even, no_edge), conv(resp.used_odd, with_edge)),
				};
			}

			resp.used_odd.insert(resp.used_odd.begin(), -INF);
			for (auto& v : resp.used_odd) if (v >= 0) v += C[cur];
			for (auto& v : resp.used_even) if (v >= 0) v += C[cur];
			return resp;
		})(0, -1);

		std::vector<int> root_used = sum(root_resp.used_odd, root_resp.used_even);
		int ans = C[0];
		for (int i = 0; i <= 2*K && i < int(root_used.size()); i++) {
			setmax(ans, root_used[i]);
		}

		cout << "Case #" << case_num << ": " << ans << '\n';;
	}

	return 0;
}
