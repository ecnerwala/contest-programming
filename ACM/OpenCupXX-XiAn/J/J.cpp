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

using pq = priority_queue<int, vector<int>, greater<int>>;

const int MAXN = 5.1e5;
int N, C;
int P[MAXN];
int invP[MAXN];
int S;
int seg[1 << 20];

num ans;

// open-interval
void dfs(int l, int r, pq&& lq, pq&& rq, int lsz, int rsz) {
	if (l >= r) {
		int sz = C * (lsz + rsz) - (l - r + 1);
		if (sz < C) {
			ans *= 1; // can't actually permute anything
			return;
		}
		assert(lsz || rsz);
		assert(C * max(lsz - 1, 0) + C * max(rsz - 1, 0) <= sz);
		// max(0, lsz - 1) special things on the left, max(0, rsz - 1) special things on the right
		for (int i = 0; i < max(0, lsz - 1); i++) {
			ans *= num(i * (C-1) + C);
		}
		for (int i = 0; i < max(0, rsz - 1); i++) {
			ans *= num(i * (C-1) + C);
		}
		for (int i = 1; i <= sz - max(lsz - 1, 0) - max(rsz - 1, 0); i++) {
			ans *= num(i);
		}
		return;
	}
	assert(l < r);
	assert(!lq.empty());
	assert(!rq.empty());

	int best = min(lq.top(), rq.top());
	for (int a = l+S+1, b = r+S; a < b; a /= 2, b /= 2) {
		if (a & 1) {
			best = min(best, seg[a]);
			a++;
		}
		if (b & 1) {
			--b;
			best = min(best, seg[b]);
		}
	}

	if (best == lq.top()) {
		lq.pop();
		for (int z = l+1; z <= l+C && z < r; z++) {
			lq.push(P[z]);
		}
		dfs(l + C, r, std::move(lq), std::move(rq), lsz + 1, rsz);
	} else if (best == rq.top()) {
		rq.pop();
		for (int z = r-1; z >= r-C && z > l; z--) {
			rq.push(P[z]);
		}
		dfs(l, r - C, std::move(lq), std::move(rq), lsz, rsz + 1);
	} else {
		int x = invP[best];
		assert(l < x && x < r);
		pq xlq;
		for (int z = x-1; z >= x-C && z > l; z--) { xlq.push(P[z]); }
		dfs(l, x - C, std::move(lq), std::move(xlq), lsz, 1);
		pq xrq;
		for (int z = x+1; z <= x+C && z < r; z++) { xrq.push(P[z]); }
		dfs(x + C, r, std::move(xrq), std::move(rq), 1, rsz);
	}
}

num go() {
	cin >> N >> C;
	for (int i = 0; i < N; i++) {
		cin >> P[i];
		P[i] --;
		invP[P[i]] = i;
	}

	S = 1;
	while (N > S) { S *= 2; }
	for (int i = 0; i < N; i++) { seg[i+S] = P[i]; }
	for (int i = S-1; i > 0; i--) { seg[i] = min(seg[2*i], seg[2*i+1]); }

	ans = 1;
	pq lq; lq.push(N+1);
	pq rq; rq.push(N+1);
	dfs(-1, N, std::move(lq), std::move(rq), 0, 0);
	return ans;
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int T; cin >> T;
	while (T--) {
		num v = go();
		cout << v << '\n';
	}

	return 0;
}
