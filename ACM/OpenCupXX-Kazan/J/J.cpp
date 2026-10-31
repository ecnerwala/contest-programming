#include<bits/stdc++.h>
using namespace std;

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

using ll = long long;

const ll INF = 1e18;

const int MAXN = 2.1e5;
int N;
vector<pair<int, ll>> adj[MAXN];
vector<pair<int, ll>> ch[MAXN];

int sz[MAXN];
int st[MAXN];
int en[MAXN];
int heavyRoot[MAXN];
int par[MAXN];
ll parWeight[MAXN];

void buildHLD() {
	std::y_combinator([&](auto self, int cur, int prv, ll parW) -> void {
		par[cur] = prv;
		parWeight[cur] = parW;
		sz[cur] = 1;
		for (auto it : adj[cur]) {
			int nxt = it.first;
			if (nxt == prv) continue;
			ch[cur].push_back(it);
			self(nxt, cur, it.second);
			sz[cur] += sz[nxt];
		}
	}) (0, -1, 0);

	int ind = 0;
	std::y_combinator([&](auto self, int cur) -> void {
		if (par[cur] != -1 && cur == ch[par[cur]][0].first) {
			heavyRoot[cur] = heavyRoot[par[cur]];
		} else {
			heavyRoot[cur] = cur;
		}
		st[cur] = ind++;
		sort(ch[cur].begin(), ch[cur].end(), std::greater<>());
		for (auto it : ch[cur]) {
			self(it.first);
		}
		en[cur] = ind;
	}) (0);
}

const int S = 1 << 18;
struct seg_t {
	bool shouldFlip;
	struct path_data_t {
		pair<ll, pair<int, int>> bestPath;
		// i = parity of length, j = assuming flip?
		pair<ll, int> bestTop;
		pair<ll, int> bestBot;
	} path_data [2];
} seg[2 * S];

void flipNode(int i) {
	seg[i].shouldFlip ^= 1;
	swap(seg[i].path_data[0], seg[i].path_data[1]);
}

void updateNode(int i) {
	if (i >= S) {
		// compute the current edge
		int cur = i-S;
	} else {
		// compute the thing
	}

	if (seg[i].shouldFlip) {
	}
}
void downdateNode(int i) {
	if (i >= S) {
		return;
	}
	if (seg[i].shouldFlip) {
		flipNode(2*i), flipNode(2*i+1);
		seg[i].shouldFlip = false;
	}
}
void downdateAll(int i) {
	if (!i) return;
	downdateAll(i/2);
	downdateNode(i);
}
void updateAll(int i) {
	if (!i) return;
	updateNode(i);
	updateAll(i/2);
}

void initializeSeg() {
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N;
	for (int i = 0; i < N; i++) {
		int u, v; ll w; cin >> u >> v >> w; u--, v--;
		adj[u].emplace_back(v, w);
		adj[v].emplace_back(u, w);
	}

	buildHLD();
	initializeSeg();

	return 0;
}
