#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

namespace kactl {
#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define all(x) begin(x), end(x)
template <typename T> int sz(const T& t) { return int(t.size()); }
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

vi num, st;
vector<vector<pii>> ed;
int Time;
template<class F>
int dfs(int at, int par, F& f) {
	int me = num[at] = ++Time, e, y, top = me;
	for (auto pa : ed[at]) if (pa.second != par) {
		tie(y, e) = pa;
		if (num[y]) {
			top = min(top, num[y]);
			if (num[y] < me)
				st.push_back(e);
		} else {
			int si = sz(st);
			int up = dfs(y, e, f);
			top = min(top, up);
			if (up == me) {
				st.push_back(e);
				//f(vi(st.begin() + si, st.end()));
				st.resize(si);
			}
			else if (up < me) st.push_back(e);
			else { f(e); }
		}
	}
	return top;
}

template<class F>
void bicomps(F f) {
	num.assign(sz(ed), 0);
	rep(i,0,sz(ed)) if (!num[i]) dfs(i, -1, f);
}
}

class AqaAsadiSaves {

public:

	long long minDamage(int N, int M, vector <int> PA, vector <int> PB, int Seed, int X, int Y) {
		vector<int> A = PA; A.reserve(M);
		vector<int> B = PB; B.reserve(M);
		for (int i = int(PA.size()); i <= M-1; i++) {
			A.push_back( Seed );
			Seed = int((int64_t(Seed) * X + Y) % N);
		}
		for (int i = int(PB.size()); i <= M-1; i++) {
			B.push_back( Seed );
			Seed = int((int64_t(Seed) * X + Y) % N);
		}

		using kactl::ed;
		ed.assign(N, {});
		for (int e = 0; e < M; e++) {
			ed[A[e]].push_back({B[e], e});
			ed[B[e]].push_back({A[e], e});
		}

		vector<bool> is_bridge(M, false);
		kactl::bicomps([&](int e) { is_bridge[e] = true; });

		vector<bool> vis(N);
		vector<pair<int, int>> par(N);
		vector<int> q; q.reserve(N);
		for (int st = 0; st < N; st++) {
			if (vis[st]) continue;
			vis[st] = true;
			par[st] = {-1, -1};
			q.push_back(st);
			for (int z = int(q.size())-1; z < int(q.size()); z++) {
				int cur = q[z];
				for (auto it : ed[cur]) {
					int nxt = it.first, e = it.second;
					if (vis[nxt]) continue;
					vis[nxt] = true;
					par[nxt] = {cur, e};
					q.push_back(nxt);
				}
			}
		}
		assert(int(q.size()) == N);

		vector<int> sz(N, 1);
		for (int z = N-1; z >= 0; z--) {
			int cur = q[z];
			if (par[cur].first == -1) continue;
			sz[par[cur].first] += sz[cur];
		}
		vector<int> sub_sz(N);
		for (int z = 0; z < N; z++) {
			int cur = q[z];
			if (par[cur].first == -1) {
				sub_sz[cur] = sz[cur];
			} else {
				sub_sz[cur] = sub_sz[par[cur].first];
			}
		}
		using ll = long long;
		vector<ll> val(N);
		for (int z = N-1; z >= 0; z--) {
			int cur = q[z];
			if (par[cur].first == -1) continue;
			if (is_bridge[par[cur].second]) {
				val[cur] = ll(sz[cur]) * ll(sub_sz[cur] - sz[cur]);
			} else {
				val[cur] = 0;
			}
		}

		auto is_good = [&](ll v) -> bool {
			vector<int> nleaves(N);
			// can i cover everything >= v
			bool hastree = false;
			for (int z = N-1; z >= 0; z--) {
				int cur = q[z];
				if (val[cur] >= v) {
					if (nleaves[cur] >= 2) return false;
					nleaves[cur] = 1;
				}
				if (nleaves[cur]) {
					if (nleaves[cur] > 2) return false;
					if (par[cur].first == -1) {
						if (hastree) return false;
						hastree = true;
					} else {
						nleaves[par[cur].first] += nleaves[cur];
					}
				}
			}
			return true;
		};

		ll mi = 0;
		ll ma = *max_element(val.begin(), val.end())+1;
		while (ma - mi > 1) {
			ll md = (mi + ma) / 2;
			if (is_good(md)) {
				ma = md;
			} else {
				mi = md;
			}
		}
		return mi;
	}
};

void my_tests() {
	// FILL IN TESTS
	cerr << AqaAsadiSaves().minDamage(88136, 44068, {}, {}, 55018, 68526, 14959) << '\n';
}

}  // namespace ecnerwala

using ecnerwala::AqaAsadiSaves;

// vim:ft=cpp
