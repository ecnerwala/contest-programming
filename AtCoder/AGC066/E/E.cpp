#include<bits/stdc++.h>
using namespace std;

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
	friend modnum inv(const modnum& m) { return m.inv(); }
	modnum neg() const {
		modnum res;
		res.v = v ? MOD-v : 0;
		return res;
	}
	friend modnum neg(const modnum& m) { return m.neg(); }

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

template <typename T> T pow(T a, long long b) {
	assert(b >= 0);
	T r = 1; while (b) { if (b & 1) r *= a; b >>= 1; a *= a; } return r;
}

using num = modnum<998244353>;

int main() {
int T; cin >> T; while (T--) {
	int N; cin >> N;
	vector<vector<int>> adj(N);
	for (int i = 0; i < N-1; i++) {
		int u, v; cin >> u >> v; u--, v--;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}

	if (N == 1) {
		cout << 1 << '\n';
		continue;
	}

	assert(N >= 2);

	vector<num> fact(N+1);
	fact[0] = 1;
	for (int i = 1; i <= N; i++) fact[i] = fact[i-1] * num(i);
	vector<num> ifact(N+1);
	ifact[N] = inv(fact[N]);
	for (int i = N; i >= 1; i--) ifact[i-1] = ifact[i] * num(i);

	int ROOT = 0;
	while (adj[ROOT].size() != 1) ROOT++;

	vector<int> preorder; preorder.reserve(N);
	vector<vector<pair<int, int>>> merges(N+3);

	std::function<void(int, int, pair<int, int>)> dfs = [&](int cur, int prv, pair<int, int> curMerge) -> void {
		preorder.push_back(cur);
		if (curMerge.first != -1) {
			merges[curMerge.second].emplace_back(curMerge.first, cur);
			curMerge.second++;
		}
		if (adj[cur].size() >= 3) {
			int nxt = adj[cur][0]; if (nxt == prv) nxt = adj[cur][1];
			if (curMerge.first != -1) {
				merges[curMerge.second].emplace_back(curMerge.first, nxt);
			}
			curMerge = {cur + N, 2};
		}
		for (int nxt : adj[cur]) {
			if (nxt == prv) continue;
			dfs(nxt, cur, curMerge);
		}
	};
	dfs(ROOT, -1, pair<int, int>(-1, -1));

	vector<int> par(2*N, -1);
	vector<int> sz(2*N);
	for (int i = 0; i < N; i++) sz[i] = 1;
	num curCnt = fact[N];

	auto getPar = [&](int a) -> int {
		stack<int> stk;
		while (par[a] >= 0) {
			stk.push(a);
			a = par[a];
		}
		while (!stk.empty()) {
			par[stk.top()] = a;
			stk.pop();
		}
		return a;
	};
	auto merge = [&](int a, int b) -> bool {
		a = getPar(a), b = getPar(b);
		if (a == b) return false;
		if (par[b] < par[a]) swap(a, b);
		par[a] += par[b];
		par[b] = a;
		curCnt *= fact[sz[a]];
		curCnt *= fact[sz[b]];
		sz[a] += sz[b];
		curCnt *= ifact[sz[a]];
		sz[b] = 0;
		return true;
	};

	vector<num> ans(N+1);
	for (int i = 0; i < N; i++) {
		ans[N-i] = curCnt;

		// delete this one
		//cerr << "delete " << preorder[i] << '\n';
		curCnt *= ifact[N-i];
		curCnt *= fact[N-i-1];
		int p = getPar(preorder[i]);
		curCnt *= fact[sz[p]];
		sz[p]--;
		curCnt *= ifact[sz[p]];

		for (auto it : merges[i+1]) {
			//cerr << "merge " << it.first << ' ' << it.second << '\n';
			merge(it.first, it.second);
		}

		//for (int v = 0; v < 2*N; v++) { if (par[v] < 0) cerr << sz[v] << ' '; } cerr << '\n';
	}

	for (int i = 1; i <= N; i++) {
		cout << fact[N] * ifact[N-i] / ans[i] << " \n"[i==N];
	}
}

	return 0;
}
