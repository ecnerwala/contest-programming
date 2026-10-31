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

const int MAXN = 2.1e5;
int N, K;
vector<pair<int, ll>> adj[MAXN];

const int MAXS = 1<<18;
int seg[MAXS*2];

ll depth[MAXN];

int preorder[MAXN];
int st[MAXN], en[MAXN];

ll solve() {
	auto checkAns = [&](int pt) -> ll {
		vector<ll> allVals;
		allVals.push_back(0); // self
		for (auto e : adj[pt]) {
			vector<ll> vals;
			std::y_combinator([&](auto self, int cur, int prv, ll dist) -> void {
				vals.emplace_back(dist);
				for (auto it : adj[cur]) {
					int nxt = it.first;
					if (nxt == prv) continue;
					self(nxt, cur, dist + it.second);
				}
			})(e.first, pt, e.second);
			sort(vals.begin(), vals.end());
			reverse(vals.begin(), vals.end());
			while (int(vals.size()) * 2 > K) {
				vals.pop_back();
			}
			allVals.insert(allVals.begin(), vals.begin(), vals.end());
		}
		if (int(allVals.size()) < K) return 0;

		ll ans = 0;
		sort(allVals.begin(), allVals.end());
		reverse(allVals.begin(), allVals.end());
		for (int i = 0; i < K; i++) ans += allVals[i];
		return ans * 2;
	};

	int ROOT = 1;
	while (adj[ROOT].size() != 1) ROOT++;

	set<pair<ll, int>> bigDownDists; set<pair<ll, int>, greater<pair<ll, int>>> smallDownDists;
	ll downOffset = 0;
	set<pair<ll, int>> bigUpDists; set<pair<ll, int>, greater<pair<ll, int>>> smallUpDists;
	ll upOffset = 0;

	//cerr << "ROOT" << ' ' << ROOT << '\n';
	int idx = 0;
	std::y_combinator([&](auto self, int cur, int prv) -> void {
		smallDownDists.emplace(depth[cur] * 2, cur);
		preorder[idx] = cur;
		st[cur] = idx;
		idx ++;
		for (auto it : adj[cur]) {
			int nxt = it.first;
			ll l = it.second;
			if (nxt == prv) continue;
			depth[nxt] = depth[cur] + l;
			self(nxt, cur);
		}
		en[cur] = idx;
	})(ROOT, 0);

	int S = 1; while (S < N) S *= 2;
	for (int i = 0; i < 2*S; i++) {
		seg[i] = 0;
	}

	auto segUpdate = [&](int node, int v) {
		for (int a = S + st[node]; a; a /= 2) {
			seg[a] += v;
		}
	};

	auto segQuery = [&](int node) -> int {
		int ans = 0;
		for (int a = S + st[node], b = S + en[node]; a < b; a /= 2, b /= 2) {
			if (a & 1) ans += seg[a++];
			if (b & 1) ans += seg[--b];
		}
		return ans;
	};

	auto fixDists = [&]() {
		//cerr << "starting to fix dists" << '\n';
		while (int(bigDownDists.size()) + int(bigUpDists.size()) < K) {
			// move from small to big
			assert(!smallDownDists.empty() || !smallUpDists.empty());
			// up being big is better, as they get bigger
			if (smallDownDists.empty() || (!smallUpDists.empty() && smallUpDists.begin()->first + upOffset >= smallDownDists.begin()->first + downOffset)) {
				bigUpDists.insert(*smallUpDists.begin());
				smallUpDists.erase(smallUpDists.begin());
			} else {
				segUpdate(smallDownDists.begin()->second, +1);
				bigDownDists.insert(*smallDownDists.begin());
				smallDownDists.erase(smallDownDists.begin());
			}
		}
		//cerr << "finished to fix dists" << '\n';
	};

	fixDists();

	auto movePt = [&](int n) {
		//cerr << "movePt " << n << '\n';
		pair<ll, int> curVal(depth[n] * 2, n);
		//cerr << curVal.first + downOffset << '\n';
		if (bigDownDists.count(curVal)) {
			bigDownDists.erase(curVal);
		} else if (smallDownDists.count(curVal)) {
			smallDownDists.erase(curVal);
		} else assert(false);
		smallUpDists.emplace(depth[n] * 2 + downOffset - upOffset, n);
		fixDists();
	};

	for (int cur = ROOT; true; ) {
		assert(upOffset == depth[cur] * 2);
		assert(downOffset == - depth[cur] * 2);

		movePt(cur);

		int nxtCur = -1;
		for (auto e : adj[cur]) {
			int nxt = e.first;
			if (st[nxt] < st[cur]) continue;
			int numCh = segQuery(nxt);
			if (numCh * 2 > K) {
				nxtCur = nxt;
			} else {
				// this guy is good, just move him to the top
				for (int z = st[nxt]; z < en[nxt]; z++) {
					movePt(preorder[z]);
				}
			}
		}

		if (nxtCur == -1) {
			return checkAns(cur);
		}
		ll nxtOffset = 2 * depth[nxtCur];
		//cerr << "nxtCur" << ' ' << nxtCur << '\n';
		while (true) {
			if (!bigDownDists.empty() && !smallUpDists.empty()) {
				ll curDown = bigDownDists.begin()->first + downOffset;
				ll curUp = smallUpDists.begin()->first + upOffset;
				assert(curDown >= curUp);
				ll cnd = upOffset + (curDown - curUp) / 2;
				if (cnd < nxtOffset) {
					//cerr << "upOffset to " << cnd << '\n';
					upOffset = cnd;
					downOffset = -upOffset;
					segUpdate(bigDownDists.begin()->second, -1);
					smallDownDists.insert(*bigDownDists.begin());
					bigDownDists.erase(bigDownDists.begin());
					fixDists();

					if (int(bigDownDists.size()) * 2 <= K) {
						return max(checkAns(cur), checkAns(nxtCur));
					}
					continue;
				}
			}
			upOffset = nxtOffset;
			downOffset = -upOffset;
			break;
		}
		cur = nxtCur;
	}
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int T; cin >> T;
	while (T--) {
		cin >> N >> K;
		for (int i = 0; i < N-1; i++) {
			int u, v; ll l; cin >> u >> v >> l;
			adj[u].emplace_back(v, l);
			adj[v].emplace_back(u, l);
		}

		ll ans = solve();
		cout << ans << '\n';

		for (int i = 1; i <= N; i++) {
			adj[i] = {};
		}
	}

	return 0;
}
