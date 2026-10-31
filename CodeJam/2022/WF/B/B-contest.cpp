#include <bits/stdc++.h>

namespace kactl {
using namespace std;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

vi val, comp, z, cont;
int Time, ncomps;
template<class G, class F> int dfs(int j, G& g, F& f) {
	int low = val[j] = ++Time, x; z.push_back(j);
	for (auto e : g[j]) if (comp[e] < 0)
		low = min(low, val[e] ?: dfs(e,g,f));

	if (low == val[j]) {
		do {
			x = z.back(); z.pop_back();
			comp[x] = ncomps;
			cont.push_back(x);
		} while (x != j);
		f(cont); cont.clear();
		ncomps++;
	}
	return val[j] = low;
}
template<class G, class F> void scc(G& g, F f) {
	int n = sz(g);
	val.assign(n, 0); comp.assign(n, -1);
	Time = ncomps = 0;
	rep(i,0,n) if (comp[i] < 0) dfs(i, g, f);
}
}

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

struct pt_t {
	int x, y;
	friend std::istream& operator >> (std::istream& i, pt_t& p) { return i >> p.x >> p.y; }
};
struct time_pt_t {
	int t;
	pt_t p;
	friend bool operator < (time_pt_t a, time_pt_t b) { return a.t < b.t; }
};

bool feasible(time_pt_t a, time_pt_t b) {
	auto sq = [](int v) -> int64_t {
		return int64_t(v) * int64_t(v);
	};
	return sq(a.p.x - b.p.x) + sq(a.p.y - b.p.y) <= sq(a.t - b.t);
}

struct claim_t {
	time_pt_t tp;
	int idx;
	int claimer;
	friend bool operator < (claim_t a, claim_t b) { return a.tp.t < b.tp.t; }
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		int N, M, S; cin >> N >> M >> S;
		std::vector<time_pt_t> duck_meetings(M);
		for (auto& v : duck_meetings) cin >> v.p >> v.t;

		std::vector<std::vector<claim_t>> claims(N);
		for (int s = 0; s < S; s++) {
			int a, b; time_pt_t tp; cin >> a >> b >> tp.p >> tp.t; a--, b--;
			assert(a != b);
			claims[a].push_back({tp, s, a});
			claims[b].push_back({tp, s, a});
		}

		std::vector<bool> is_duck(N, false);
		std::vector<int> duck_queue; duck_queue.reserve(N);
		auto mark_duck = [&](int d) {
			if (!is_duck[d]) {
				is_duck[d] = true;
				duck_queue.push_back(d);
			}
		};

		std::vector<std::vector<int>> adj(N);

		for (int i = 0; i < N; i++) {
			if (claims[i].empty()) continue;

			std::sort(claims[i].begin(), claims[i].end());
			int L = -1;
			for (int z = 0; z < int(claims[i].size()); z++) {
				while (true) {
					if (L >= 0 && !feasible(claims[i][L].tp, claims[i][z].tp)) {
						if (claims[i][L].idx < claims[i][z].idx) {
							mark_duck(claims[i][L].claimer);
							L--;
							continue;
						} else {
							mark_duck(claims[i][z].claimer);
							// skip this z
							break;
						}
					} else {
						claims[i][++L] = claims[i][z];
						break;
					}
				}
			}
			claims[i].resize(L+1);

			adj[i].reserve(claims[i].size());
			for (const auto& claim : claims[i]) {
				if (claim.claimer == i) continue;
				auto it = std::lower_bound(duck_meetings.begin(), duck_meetings.end(), claim.tp);
				if (it != duck_meetings.end()) {
					if (!feasible(claim.tp, *it)) {
						adj[i].push_back(claim.claimer);
						continue;
					}
				}
				if (it != duck_meetings.begin()) {
					if (!feasible(claim.tp, *std::prev(it))) {
						adj[i].push_back(claim.claimer);
						continue;
					}
				}
			}
		}
		claims = {};

		int ans;
		if (duck_queue.empty()) {
			ans = N + 1;
			kactl::scc(adj, [](const auto&) {});
			std::vector<int> comp_sz(kactl::ncomps);
			for (int i = 0; i < N; i++) {
				comp_sz[kactl::comp[i]] ++;
			}
			for (int i = 0; i < N; i++) {
				for (int j : adj[i]) {
					if (kactl::comp[i] != kactl::comp[j]) comp_sz[kactl::comp[i]] = -1;
				}
			}
			for (int i = 0; i < kactl::ncomps; i++) {
				if (comp_sz[i] != -1) ans = min(ans, comp_sz[i]);
			}
		} else {
			for (int z = 0; z < int(duck_queue.size()); z++) {
				int i = duck_queue[z];
				for (int j : adj[i]) mark_duck(j);
			}
			ans = int(duck_queue.size());
		}

		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}
