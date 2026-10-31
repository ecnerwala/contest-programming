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

template <typename T> T pow(T a, long long b) {
	assert(b >= 0);
	T r = 1; while (b) { if (b & 1) r *= a; b >>= 1; a *= a; } return r;
}
using num = modnum<998244353>;

using ll = long long;

const int MAXN = 3.1e5;
int N;
ll X;
vector<pair<ll, ll>> A;

const int MAXV = MAXN * 60;
int V;
int cnt[MAXV];
int ch[MAXV][2];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N >> X;
	//N = int(3e5), X = (1ll << (60 - 1));
	if (X == 0) {
		cout << pow(num(2), N) - 1 << '\n';
		return 0;
	}
	int L = (64 - __builtin_clzll(X));
	//cerr << L << '\n';
	for (int i = 0; i < N; i++) {
		ll a; cin >> a;
		A.emplace_back(a >> L, a & ((1ll << L) - 1));
	}
	sort(A.begin(), A.end());

	num ans = 1;
	for (int st = 0; st < int(A.size()); ) {
		ll pref = A[st].first;

		ll bucketVal = 1;

		cnt[0] = 0;
		ch[0][0] = ch[0][1] = 0;
		V = 1;

		for (; st < int(A.size()) && A[st].first == pref; st++) {
			ll a = A[st].second;
			bucketVal += 1; // take just yourself

			int cur = 0;
			for (int i = L-1; i >= 0; i--) {
				int dir = ((a >> i) ^ (X >> i)) & 1;
				if (!((X >> i) & 1)) {
					if (ch[cur][!dir]) bucketVal += cnt[ch[cur][!dir]];
				}
				if (!ch[cur][dir]) {
					break;
				}
				cur = ch[cur][dir];
				if (i == 0) {
					// equal case
					bucketVal += cnt[cur];
				}
			}

			cur = 0;
			for (int i = L-1; i >= 0; i--) {
				cnt[cur] ++;
				int dir = (a >> i) & 1;
				if (!ch[cur][dir]) {
					ch[cur][dir] = V;
					cnt[V] = ch[V][0] = ch[V][1] = 0;
					V++;
				}
				cur = ch[cur][dir];
			}
			cnt[cur] ++;
		}
		ans *= num(bucketVal);
	}
	ans -= 1;
	cout << ans << '\n';

	return 0;
}
