#include<bits/stdc++.h>
using namespace std;

namespace kactl {
#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define trav(a, x) for (auto& a : x)
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

vi num, st;
vector<vector<pii>> ed;
int Time;
template<class F> int dfs(int at, int par, F f) {
	int me = num[at] = ++Time, e, y, top = me;
	trav(pa, ed[at]) if (pa.second != par) {
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
				//f(vi(st.begin()+si, st.end()));
				st.resize(si);
			}
			else if (up < me) st.push_back(e);
			else {
				/* e is a bridge */
				f(e);
			}
		}
	}
	return top;
}

template <class F> void bicomps(F f) {
	num.assign(sz(ed), 0);
	rep(i, 0, sz(ed)) if (!num[i]) dfs(i, -1, f);
}
} // namespace kactl

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);

	int N, M; cin >> N >> M;
	vector<pair<int, int>> edges(M);
	for (auto& it : edges) {
		int a, b; cin >> a >> b; a--, b--;
		it = {a,b};
	}
	vector<bool> alive(M, true);

	kactl::ed.resize(N);

	auto buildAdj = [&]() -> void {
		for (int i = 0; i < N; i++) {
			kactl::ed[i].clear();
		}
		for (int e = 0; e < M; e++) {
			if (alive[e]) {
				int a = edges[e].first, b = edges[e].second;
				kactl::ed[a].emplace_back(b, e);
				kactl::ed[b].emplace_back(a, e);
			}
		}
	};

	const int MOD = 998244353;
	const int64_t BASE1 = 260401;
	const int64_t BASE2 = 132607;
	vector<pair<int, int>> labels(N);

	auto incrLabel = [&](int i, int v) {
		labels[i].first = (labels[i].first * BASE1 + v) % MOD;
		labels[i].second = (labels[i].second * BASE2 + v) % MOD;
	};

	vector<bool> vis(N);
	vector<int> q; q.reserve(N);
	auto labelCCs = [&]() {
		buildAdj();
		fill(vis.begin(), vis.end(), false);
		for (int i = 0; i < N; i++) {
			if (!vis[i]) {
				vis[i] = true;
				q.push_back(i);
				for (int z = 0; z < int(q.size()); z++) {
					int cur = q[z];
					incrLabel(cur, i);
					for (auto it : kactl::ed[cur]) {
						int nxt = it.first;
						if (vis[nxt]) continue;
						vis[nxt] = true;
						q.push_back(nxt);
					}
				}
				q.clear();
			}
		}
	};

	auto getFreq = [&]() -> int {
		//for (int i = 0; i < N; i++) { cerr << labels[i].first << ' ' << labels[i].second << '\n'; }
		int res = 0;
		for (int i = 0; i < N; i++) {
			for (int j = i+1; j < N; j++) {
				res += labels[i] == labels[j];
			}
		}
		return res;
	};

	auto cutBridges = [&]() -> void {
		buildAdj();
		kactl::bicomps([&] (int e) {
			alive[e] = false;
		});
	};

	labelCCs();
	int ans1 = getFreq();

	cutBridges();
	labelCCs();
	int ans2 = getFreq();

	for (int e = 0; e < M; e++) {
		fill(alive.begin(), alive.end(), true);
		alive[e] = false;
		cutBridges();
		labelCCs();
	}
	int ans3 = getFreq();

	//cerr << ans1 << ' ' << ans2 << ' ' << ans3 << '\n';
	cout << ans1 + ans2 + ans3 << '\n';

	return 0;
}
