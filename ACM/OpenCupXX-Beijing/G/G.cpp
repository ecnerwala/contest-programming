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
using num = modnum<int(1e9)+7>;

const int MAXN = 2.1e5;
int N;
vector<int> adj[MAXN];

struct linFn {
	num a = 1, b = 0;
	// apply g then f
	friend linFn operator * (const linFn& f, const linFn& g) {
		return linFn{f.a * g.a, f.a * g.b + f.b};
	}
	num operator () (num x) const {
		return a * x + b;
	}
};

using dp_vec = vector<pair<num, linFn>>;

void push(dp_vec& v, int i) {
	v[i].first = v[i].second(v[i].first);
	if (i > 0) {
		v[i-1].second = v[i].second * v[i-1].second;
	}
	v[i].second = linFn{};
}

void mul_eq(dp_vec& a, dp_vec& b) {
	if (b.size() > a.size()) swap(a,b);
	for (int i = int(b.size())-1, j = int(a.size())-1; i >= 0; i--, j--) {
		push(b, i);
		push(a, j);

		a[j].first *= b[i].first;
	}
	if (int(b.size()) < int(a.size())) {
		int j = int(a.size()) - int(b.size()) - 1;
		a[j].second = linFn{b.front().first, num(0)} * a[j].second;
	}
}

dp_vec dfs(int cur, int prv) {
	dp_vec ans;
	ans.emplace_back(num(1), linFn{});
	for (int nxt : adj[cur]) {
		if (nxt == prv) continue;
		dp_vec r = dfs(nxt, cur);
		mul_eq(ans, r);
	}
	ans.emplace_back(num(0), linFn{num(1), num(1)});
	return ans;
}

num go() {
	const int ROOT = 0;
	dp_vec all;
	all.emplace_back(num(1), linFn{});

	vector<num> facs(N+1);

	for (int ch : adj[ROOT]) {
		auto v = dfs(ch, ROOT);
		for (int i = int(v.size()) - 1; i >= 0; i--) {
			push(v, i);
			if (i+1 < int(v.size())) {
				facs[int(v.size())-1-(i+1)] += (v[i].first - v[i+1].first) / v[i+1].first;
			}
		}

		mul_eq(all, v);
	}

	num ans = 0;
	for (int i = int(all.size()) - 1; i >= 0; i--) {
		push(all, i);
		//cerr << all[i].first << '\n';
		ans += facs[int(all.size())-1-i] * all[i].first;
	}
	return all.front().first - ans;
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int T; cin >> T;
	for (int t = 1; t <= T; t++) {
		//cerr << "Case #" << t << '\n';

		cin >> N;
		for (int i = 0; i < N-1; i++) {
			int x, y; cin >> x >> y; x--, y--;
			adj[x].push_back(y);
			adj[y].push_back(x);
		}
		num ans = go();
		cout << "Case #" << t << ": " << ans << '\n';

		for (int i = 0; i < N; i++) {
			adj[i] = {};
		}
	}

	return 0;
}
