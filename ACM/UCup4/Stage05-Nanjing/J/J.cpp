#include <bits/stdc++.h>

namespace kactl {
using namespace std;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

vi num, st;
vector<vector<pii>> ed;
int Time;
template<class F>
int dfs(int at, int par, F& f) {
	int me = num[at] = ++Time, top = me;
	for (auto [y, e] : ed[at]) if (e != par) {
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
				f(vi(st.begin() + si, st.end()));
				st.resize(si);
			}
			else if (up < me) st.push_back(e);
			else { f(std::vector<int>({e})); }
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

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;
		std::vector<std::array<int, 2>> edges(M);
		kactl::ed.clear();
		kactl::ed.resize(N);
		for (int e = 0; e < M; e++) {
			int u, v; cin >> u >> v; u--, v--;
			edges[e] = {u, v};
			kactl::ed[u].push_back({v, e});
			kactl::ed[v].push_back({u, e});
		}
		int tot_comps = 0;
		std::vector<int> last_comp(N, -1);
		std::vector<int> num_comps(N, 0);
		std::vector<bool> has_bridge(N, false);
		kactl::bicomps([&](auto es) -> void {
			int cur_comp = tot_comps++;
			cerr << "comp " << cur_comp << '\n';
			for (auto e : es) {
				cerr << "edge " << e << '\n';
				for (auto v : edges[e]) {
					cerr << "vert " << v << '\n';
					if (last_comp[v] == cur_comp) continue;
					last_comp[v] = cur_comp;
					num_comps[v]++;
					if (es.size() == 1) has_bridge[v] = true;
				}
			}
		});
		std::vector<int> ans; ans.reserve(N);
		for (int i = 0; i < N; i++) {
			if (num_comps[i] >= 2 && !has_bridge[i]) ans.push_back(i);
		}
		if (!ans.empty()) {
			for (int z = 0; z < int(ans.size()); z++) {
				cout << ans[z]+1 << " \n"[z+1==int(ans.size())];
			}
		} else {
			cout << "Empty" << '\n';
		}
	}

	return 0;
}
