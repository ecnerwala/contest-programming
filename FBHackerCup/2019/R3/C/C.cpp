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

using num = modnum<int(1e9)+7>;

const int MAXN = 7000;
int N;
int K;

int P[MAXN];

num go() {
	int A, B;
	cin >> N >> K >> A >> B;
	P[1] = 0;
	for (int i = 2; i <= N; i++) {
		cin >> P[i];
	}
	vector<int> Apath;
	for (int cur = A; cur != 0; cur = P[cur]) {
		Apath.push_back(cur);
	}
	reverse(Apath.begin(), Apath.end());
	vector<int> Bpath;
	for (int cur = B; cur != 0; cur = P[cur]) {
		Bpath.push_back(cur);
	}
	reverse(Bpath.begin(), Bpath.end());

	assert(Apath[0] == 1);
	assert(Bpath[0] == 1);

	int numEq = 0;
	while (numEq < int(Apath.size()) && numEq < int(Bpath.size()) && Apath[numEq] == Bpath[numEq]) {
		numEq++;
	}

	int lcaLen = numEq - 1;
	int aLen = int(Apath.size()) - numEq;
	int bLen = int(Bpath.size()) - numEq;

	num ans = 0;
	for (int i = 0; i < aLen + lcaLen; i++) {
		ans += pow(num(N-1-i), K);
	}
	for (int i = 0; i < bLen + lcaLen; i++) {
		ans += pow(num(N-1-i), K);
	}
	for (int i = 0; i < lcaLen; i++) {
		ans -= 2 * pow(num(N-1 - aLen - bLen - i), K);
	}
	return ans / pow(num(N-1), K);
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int T; cin >> T;
	for (int t = 1; t <= T; t++) {
		//cerr << "Case #" << t << '\n';
		num ans = go();
		cout << "Case #" << t << ": " << ans << '\n';
	}

	return 0;
}
