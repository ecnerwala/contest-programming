#include<bits/stdc++.h>

namespace kactl {
using namespace std;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

struct PushRelabel {
	struct Edge {
		int dest, back;
		ll f, c;
	};
	vector<vector<Edge>> g;
	vector<ll> ec;
	vector<Edge*> cur;
	vector<vi> hs; vi H;
	PushRelabel(int n) : g(n), ec(n), cur(n), hs(2*n), H(n) {}

	void addEdge(int s, int t, ll cap, ll rcap=0) {
		if (s == t) return;
		g[s].push_back({t, sz(g[t]), 0, cap});
		g[t].push_back({s, sz(g[s])-1, 0, rcap});
	}

	void addFlow(Edge& e, ll f) {
		Edge &back = g[e.dest][e.back];
		if (!ec[e.dest] && f) hs[H[e.dest]].push_back(e.dest);
		e.f += f; e.c -= f; ec[e.dest] += f;
		back.f -= f; back.c += f; ec[back.dest] -= f;
	}
	ll calc(int s, int t) {
		int v = sz(g); H[s] = v; ec[t] = 1;
		vi co(2*v); co[0] = v-1;
		rep(i,0,v) cur[i] = g[i].data();
		for (Edge& e : g[s]) addFlow(e, e.c);

		for (int hi = 0;;) {
			while (hs[hi].empty()) if (!hi--) return -ec[s];
			int u = hs[hi].back(); hs[hi].pop_back();
			while (ec[u] > 0)  // discharge u
				if (cur[u] == g[u].data() + sz(g[u])) {
					H[u] = 1e9;
					for (Edge& e : g[u]) if (e.c && H[u] > H[e.dest]+1)
						H[u] = H[e.dest]+1, cur[u] = &e;
					if (++co[H[u]], !--co[hi] && hi < v)
						rep(i,0,v) if (hi < H[i] && H[i] < v)
							--co[H[i]], H[i] = v + 1;
					hi = H[u];
				} else if (cur[u]->c && H[u] == H[cur[u]->dest]+1)
					addFlow(*cur[u], min(ec[u], cur[u]->c));
				else ++cur[u];
		}
	}
	bool leftOfMinCut(int a) { return H[a] >= sz(g); }
};

#undef rep
#undef all
#undef sz
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	struct job {
		int p, t;
		vector<int> deps;
	};
	vector<job> jobs(N);
	for (auto& j : jobs) {
		cin >> j.p >> j.t;
		int d; cin >> d;
		j.deps.resize(d);
		for (int& a : j.deps) cin >> a, a--;
	}

	// We can test if best_avg > d which means
	// sum(p - t * d) > 0

	auto is_good = [&](int d) -> bool {
		kactl::PushRelabel flower(N+2);
		const int S = N;
		const int T = N+1;
		int64_t tot_positive = 0;
		for (int i = 0; i < N; i++) {
			int val = jobs[i].p - jobs[i].t * d;
			if (val >= 0) {
				flower.addEdge(S, i, val);
				tot_positive += val;
			} else {
				flower.addEdge(i, T, -val);
			}

			for (int j : jobs[i].deps) {
				flower.addEdge(i, j, int64_t(1e10));
			}
		}
		int64_t best_val = tot_positive - flower.calc(S, T);
		return best_val > 0;
	};

	int mi = 0;
	int ma = 1024;

	while (ma - mi > 1) {
		int md = (mi + ma) / 2;
		if (is_good(md)) {
			mi = md;
		} else {
			ma = md;
		}
	}

	auto is_exactly_good = [&](int d) -> bool {
		for (int special = 0; special < N; special++) {
			kactl::PushRelabel flower(N+2);

			const int S = N;
			const int T = N+1;
			int64_t tot_positive = 0;
			for (int i = 0; i < N; i++) {
				int val = jobs[i].p - jobs[i].t * d;
				if (val >= 0) {
					flower.addEdge(S, i, val);
					tot_positive += val;
				} else {
					flower.addEdge(i, T, -val);
				}

				for (int j : jobs[i].deps) {
					flower.addEdge(i, j, int64_t(1e10));
				}
			}

			// add a single extra
			flower.addEdge(S, special, 1);
			// If the single extra doesn't change the flow, then there's a cut with special on the LHS, i.e. taken, so we're good

			int64_t best_val = tot_positive - flower.calc(S, T);

			assert(best_val == -1 || best_val == 0);

			if (best_val == 0) return true;
		}
		return false;
	};

	if (is_exactly_good(ma)) {
		cout << ma << '\n';
	} else {
		cout << mi << '\n';
	}

	return 0;
}
