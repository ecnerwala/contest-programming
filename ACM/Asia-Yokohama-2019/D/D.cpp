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

template <typename T> int signum(T a) { return (a > 0) - (a < 0); }

using ll = long long;

ll sq(ll a) { return a * a; }

struct P3 {
	ll x, y, z;
	P3 () : x(0), y(0), z(0) {}
	P3(ll x_, ll y_, ll z_) : x(x_), y(y_), z(z_) {}

	friend std::ostream& operator << (std::ostream& o, const P3& p) { return o << "(" << p.x << "," << p.y << "," << p.z << ")"; }
	friend std::istream& operator >> (std::istream& i, P3& p) { return i >> p.x >> p.y >> p.z; }

	friend P3 operator + (P3 a, P3 b) { return P3(a.x + b.x, a.y + b.y, a.z + b.z); }
	friend P3 operator - (P3 a, P3 b) { return P3(a.x - b.x, a.y - b.y, a.z - b.z); }

	friend ll dot(P3 a, P3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
	friend P3 cross(P3 a, P3 b) { return P3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x); }

	friend ll norm(P3 p) { return dot(p, p); }
	friend ll dist2(P3 a, P3 b) { return norm(b-a); }
	friend ll area2(P3 a, P3 b, P3 c) {
		return norm(cross(b-a, c-a));
	}
	friend ll vol(P3 a, P3 b, P3 c, P3 d) {
		return dot(cross(b-a, c-a), d-a);
	}
};

struct tree {
	int N;
	vector<P3> pts;
	vector<pair<int, int>> edges;
	vector<vector<int>> adj;

	tree() {}
	tree(int N_) : N(N_), pts(N), edges(N-1), adj(N) {}
	friend std::istream& operator >> (std::istream& in, tree& t) {
		for (auto& p : t.pts) in >> p;
		for (auto& it : t.edges) {
			in >> it.first >> it.second;
			it.first--, it.second--;
			it.first %= t.N;
			it.second %= t.N;
			t.adj[it.first].push_back(it.second);
			t.adj[it.second].push_back(it.first);
		}
		return in;
	}

	vector<int> getSz(int root) {
		vector<int> sz(N);
		auto dfs_sz = y_combinator([&](auto self, int cur, int prv = -1) -> void {
			sz[cur] = 1;
			for (int nxt : adj[cur]) {
				if (nxt == prv) continue;
				self(nxt, cur);
				sz[cur] += sz[nxt];
			}
		});

		dfs_sz(root);
		return sz;
	}

	vector<int> getCentroid() {
		vector<int> sz = getSz(0);
		int cur = 0;
		while (true) {
			int nxt = -1;
			for (int cnd : adj[cur]) {
				if (sz[cnd] > sz[cur]) continue;
				if (nxt == -1 || sz[cnd] > sz[nxt]) nxt = cnd;
			}
			if (nxt == -1 || sz[nxt] * 2 < N) {
				return {cur};
			} else if (sz[nxt] * 2 == N) {
				return {cur, nxt};
			} else {
				cur = nxt;
			}
		}
	}

	ll getScale() {
		ll ans = 0;
		for (auto it : edges) {
			ans = max(ans, dist2(pts[it.first], pts[it.second]));
		}
		return ans;
	}

	vector<int> getOffLine(int i, int j) {
		vector<int> res;
		for (int k = 0; k < N; k++) {
			if (area2(pts[i], pts[j], pts[k])) {
				res.push_back(k);
			}
		}
		return res;
	}

	vector<pair<tuple<ll,ll,ll,int>, tuple<ll,ll,ll,int>>> getRel(int i, int j, int k, ll s) {
		vector<tuple<ll,ll,ll,int>> pRel;
		for (P3 p : pts) {
			pRel.push_back(tuple<ll,ll,ll,int>(dist2(pts[i], p) * s, dist2(pts[j], p) * s, dist2(pts[k], p) * s, signum(vol(pts[i], pts[j], pts[k], p))));
		}

		vector<pair<tuple<ll,ll,ll,int>,tuple<ll,ll,ll,int>>> res;
		for (auto e : edges) {
			auto l = pRel[e.first];
			auto r = pRel[e.second];
			if (l > r) swap(l,r);
			res.push_back({l,r});
		}
		sort(res.begin(),res.end());
		return res;
	}
};

int numEq(tree T1, tree T2) {
	if (T1.N != T2.N) return 0;
	int N = T1.N;
	if (N <= 1) return 1;
	if (N == 2) return 2;

	auto S1 = T1.getScale();
	auto S2 = T2.getScale();

	auto C1 = T1.getCentroid();
	auto C2 = T2.getCentroid();
	if (C1.size() != C2.size()) {
		return 0;
	}

	assert(C1.size() == C2.size());
	vector<pair<pair<int, int>, pair<int, int>>> rootPairs;
	if (C1.size() == 2) {
		rootPairs = {
			{{C1[0], C1[1]}, {C2[0], C2[1]}},
			{{C1[0], C1[1]}, {C2[1], C2[0]}},
		};
	} else if (C1.size() == 1) {
		assert(!T1.adj[C1[0]].empty());
		for (int v : T2.adj[C2[0]]) {
			rootPairs.push_back({{C1[0], T1.adj[C1[0]][0]}, {C2[0], v}});
		}
	} else assert(false);

	int ans = 0;
	for (auto it : rootPairs) {
		int i1 = it.first.first, j1 = it.first.second;
		int i2 = it.second.first, j2 = it.second.second;

		if (dist2(T1.pts[i1], T1.pts[j1]) * S2 != dist2(T2.pts[i2], T2.pts[j2]) * S1) {
			continue;
		}

		vector<int> k1s = T1.getOffLine(i1, j1);
		vector<int> k2s = T2.getOffLine(i2, j2);
		if (k1s.empty() != k2s.empty()) continue;

		if (k1s.empty()) {
			k1s.push_back(i1);
			k2s.push_back(i2);
		}
		int k1 = k1s.front();
		ll di = dist2(T1.pts[i1], T1.pts[k1]);
		ll dj = dist2(T1.pts[j1], T1.pts[k1]);
		for (int k2 : k2s) {
			if (di * S2 != dist2(T2.pts[i2], T2.pts[k2]) * S1) continue;
			if (dj * S2 != dist2(T2.pts[j2], T2.pts[k2]) * S1) continue;
			if (T1.getRel(i1, j1, k1, S2) == T2.getRel(i2, j2, k2, S1)) {
				ans++;
			}
		}
	}
	return ans;
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N; cin >> N;
	tree T1(N), T2(N);
	cin >> T1 >> T2;

	cout << numEq(std::move(T1), std::move(T2)) << '\n';
	return 0;
}
