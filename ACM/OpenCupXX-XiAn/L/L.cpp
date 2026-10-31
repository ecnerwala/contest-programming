#include<bits/stdc++.h>
using namespace std;

namespace kactl {

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define trav(a, x) for(auto& a : x)
#define all(x) x.begin(), x.end()
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;
vi val, comp, z, cont;
int Time, ncomps;
template<class G, class F> int dfs(int j, G& g, F f) {
	int low = val[j] = ++Time, x; z.push_back(j);
	trav(e,g[j]) if (comp[e] < 0)
		low = min(low, val[e] ? val[e] : dfs(e,g,f));

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

} // namespace kactl

template <int MOD_> struct modnum {
	static constexpr int MOD = MOD_;
	static_assert(MOD_ > 0, "MOD must be positive");

private:
	using ll = long long;

	int v;

	static int minv(int a, int m) {
		a %= m;
		assert(a);
		return a == 1 ? 1 : int(m - ll(minv(m, a)) * ll(m) / a);
	}

public:

	modnum() : v(0) {}
	modnum(ll v_) : v(int(v_ % MOD)) { if (v < 0) v += MOD; }
	explicit operator int() const { return v; }
	friend std::ostream& operator << (std::ostream& out, const modnum& n) { return out << int(n); }
	friend std::istream& operator >> (std::istream& in, modnum& n) { ll v_; in >> v_; n = modnum(v_); return in; }

	friend bool operator == (const modnum& a, const modnum& b) { return a.v == b.v; }
	friend bool operator != (const modnum& a, const modnum& b) { return a.v != b.v; }

	modnum inv() const {
		modnum res;
		res.v = minv(v, MOD);
		return res;
	}
	modnum neg() const {
		modnum res;
		res.v = v ? MOD-v : 0;
		return res;
	}

	modnum operator- () const {
		return neg();
	}
	modnum operator+ () const {
		return modnum(*this);
	}

	modnum& operator ++ () {
		v ++;
		if (v == MOD) v = 0;
		return *this;
	}
	modnum& operator -- () {
		if (v == 0) v = MOD;
		v --;
		return *this;
	}
	modnum& operator += (const modnum& o) {
		v += o.v;
		if (v >= MOD) v -= MOD;
		return *this;
	}
	modnum& operator -= (const modnum& o) {
		v -= o.v;
		if (v < 0) v += MOD;
		return *this;
	}
	modnum& operator *= (const modnum& o) {
		v = int(ll(v) * ll(o.v) % MOD);
		return *this;
	}
	modnum& operator /= (const modnum& o) {
		return *this *= o.inv();
	}

	friend modnum operator ++ (modnum& a, int) { modnum r = a; ++a; return r; }
	friend modnum operator -- (modnum& a, int) { modnum r = a; --a; return r; }
	friend modnum operator + (const modnum& a, const modnum& b) { return modnum(a) += b; }
	friend modnum operator - (const modnum& a, const modnum& b) { return modnum(a) -= b; }
	friend modnum operator * (const modnum& a, const modnum& b) { return modnum(a) *= b; }
	friend modnum operator / (const modnum& a, const modnum& b) { return modnum(a) /= b; }
};
using num = modnum<998244353>;

const int MAXN = 2100;
const int MAXM = 4100;
int N, M;
int K;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N >> M >> K;

	if (K == 0) {
		cout << 0 << '\n';
		exit(0);
	}

	assert(K >= 1);

	vector<vector<int>> adj(N+2);
	for (int i = 1; i <= N; i++) {
		adj[0].push_back(i);
		adj[i].push_back(N+1);
	}
	adj[0].push_back(N+1);

	for (int e = 0; e < M; e++) {
		int a, b; cin >> a >> b;
		adj[a].push_back(b);
	}

	vector<vector<int>> ccs;
	kactl::scc(adj, [&](vector<int> cc) { ccs.push_back(std::move(cc)); });

	reverse(ccs.begin(), ccs.end());
	assert(ccs.front() == vector<int>({0}));
	assert(ccs.back() == vector<int>({N+1}));

	int num_ccs = int(ccs.size());

	vector<int> cc_num(N+2);
	for (int z = 0; z < num_ccs; z++) {
		for (int v : ccs[z]) cc_num[v] = z;
	}

	vector<vector<num>> dp(N+2, vector<num>(N+2));
	for (int i = 1; i <= N+1; i++) {
		for (int j = 1; j <= N+1; j++) {
			if (min(cc_num[i], cc_num[j]) > 1) continue;
			dp[i][j] ++;
		}
	}

	for (int a = 1; a < num_ccs - 1; a++) {
		vector<int> verts;
		verts.push_back(ccs[a].front());
		while (verts.size() < ccs[a].size()) {
			int cur = verts.back();
			for (int nxt : adj[cur]) {
				assert(cc_num[nxt] >= a);
				if (cc_num[nxt] == a) {
					verts.push_back(nxt);
					break;
				}
			}
			assert(verts.back() != cur);
		}

		int V = int(verts.size());
		for (int b = a+1; b < num_ccs; b++) {
			for (int ov : ccs[b]) {
				num cnt = 0;
				for (int v : verts) {
					assert(v != ov);
					cnt += dp[v][ov] + dp[ov][v];
				}
				// K ways to go in the circle
				if (V > 1) {
					cnt *= num(K-1);
				} else {
					cnt = 0; // just count the exact-circle
				}
				for (int i = 0; i < V; i++) {
					int v = verts[i];
					for (int u : adj[v]) {
						if (cc_num[u] == a) continue;
						if (min(b, cc_num[u]) > a+1) {
							continue;
						}
						dp[u][ov] += cnt + dp[verts[(i+1)%V]][ov] + dp[ov][verts[(i+1)%V]];
					}
				}
			}
		}

		vector<vector<num>> cycPref(V+1, vector<num>(V+1));
		vector<vector<num>> cycOut(V, vector<num>(V));

		auto addRect = [&](int x1, int x2, int y1, int y2, num v) {
			//cerr << " addRect  " << x1 << '-' << x2 << ' ' << y1 << '-' << y2 << ' ' << v << '\n';
			assert(0 <= x1 && x1 <= x2 && x2 <= V);
			assert(0 <= y1 && y1 <= y2 && y2 <= V);
			cycPref[x1][y1] += v;
			cycPref[x2][y1] -= v;
			cycPref[x1][y2] -= v;
			cycPref[x2][y2] += v;
		};
		if (V == 1) {
			if (K >= 2) {
				cycOut[0][0] = dp[verts[0]][verts[0]];
			}
		} else {
			assert(V >= 2);
			for (int i = 0; i < V; i++) {
				if (K >= 2) {
					num v;
					//cerr << i << ' ' << dp[verts[i]][verts[i]] << '\n';
					v = dp[verts[i]][verts[i]] * num(K) * num(K-1) / num(2);

					addRect(0, V, 0, V, v);

					// subtract off things which don't hit everything, i.e. don't hit i-1
					if (i == 0) {
						addRect(0, V-1, 0, V-1, -dp[verts[i]][verts[i]]);
					} else {
						addRect(0, i-1, 0, i-1, -dp[verts[i]][verts[i]]);
						addRect(0, i-1, i, V, -dp[verts[i]][verts[i]]);
						addRect(i, V, 0, i-1, -dp[verts[i]][verts[i]]);
						addRect(i, V, i, V, -dp[verts[i]][verts[i]]);
					}
				}

				for (int j = i+1; j < V; j++) {
					num curVal = dp[verts[i]][verts[j]] + dp[verts[j]][verts[i]];
					//cerr << i << ' ' << j << ' ' << curVal << '\n';
					assert(V >= 2);

					if (K == 1) {
						cycOut[(i+V-1)%V][(j+V-1)%V] += curVal;
					} else {

						// everything can cycle <= K-2 times
						{
							num v = curVal * num(K) * num(K-1) / num(2);
							addRect(0, V, 0, V, v);
						}

						// things matching to i..j and j..V/0..i count an extra K
						{
							num v = curVal * num(K);
							addRect(i, j, j, V, v);
							addRect(i, j, 0, i, v);
						}

						// subtract off everything which doesn't hit everything
						if (j-1 != i) {
							addRect(i, j-1, j, V, -curVal);
							addRect(i, j-1, 0, j-1, -curVal);
						}

						if ((i+V-1) % V != j) {
							if (i == 0) {
								addRect(0, V-1, j, V-1, -curVal);
							} else {
								addRect(i, V, j, V, -curVal);
								addRect(0, i-1, j, V, -curVal);
								addRect(i, V, 0, i-1, -curVal);
								addRect(0, i-1, 0, i-1, -curVal);
							}
						}

						// add these back in
						if (j-1 != i && (i+V-1) % V != j) {
							if (i == 0) {
								addRect(i, j-1, j, V-1, +curVal);
							} else {
								addRect(i, j-1, j, V, +curVal);
								addRect(i, j-1, 0, i-1, +curVal);
							}
						}
					}
				}
			}
		}

		for (int i = 0; i <= V; i++) {
			for (int j = 0; j <= V; j++) {
				if (i > 0) { cycPref[i][j] += cycPref[i-1][j]; }
				if (j > 0) { cycPref[i][j] += cycPref[i][j-1]; }
				if (i > 0 && j > 0) { cycPref[i][j] -= cycPref[i-1][j-1]; }
			}
		}
		for (int i = 0; i < V; i++) {
			for (int j = 0; j < V; j++) {
				cycOut[i][j] += cycPref[i][j];
			}
		}
		//for (int i = 0; i < V; i++) { for (int j = 0; j < V; j++) { cerr << cycOut[i][j] << ' '; } cerr << '\n'; }

		for (int i = 0; i < V; i++) {
			for (int j = 0; j < V; j++) {
				for (int x : adj[verts[i]]) {
					if (cc_num[x] == a) continue;
					for (int y : adj[verts[j]]) {
						if (cc_num[y] == a) continue;
						if (min(cc_num[x], cc_num[y]) > a+1) {
							continue;
						}
						dp[x][y] += cycOut[i][j];
					}
				}
			}
		}

	}

	//for (int i = 1; i <= N+1; i++) { for (int j = 1; j <= N+1; j++) { cerr << dp[i][j] << ' '; } cerr << '\n'; }
	cout << dp[N+1][N+1] << '\n';

	return 0;
}
