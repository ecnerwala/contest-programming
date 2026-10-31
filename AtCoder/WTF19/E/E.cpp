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

using num = modnum<int(1e9)+7>;

const int MAXN = 1.1e3;
int N;
num ifact[MAXN*2+1];
num H[MAXN];
num den[MAXN];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N;

	ifact[0] = 1;
	for (int i = 1; i <= 2*N+1; i++) ifact[i] = ifact[i-1] * num(i);
	ifact[2*N+1] = inv(ifact[2*N+1]);
	for (int i = 2*N+1; i >= 1; i--) ifact[i-1] = ifact[i] * num(i);

	for (int i = 0; i <= N; i++) {
		den[i] = ifact[2*i] - (i > 0 ? ifact[2*i-1] : 0);
		H[i] = ifact[2*i] - ifact[2*i+1];
	}
	assert(den[0] == num(1));
	for (int i = 0; i <= N; i++) {
		for (int j = 1; i+j <= N; j++) {
			H[i+j] -= H[i] * den[j];
		}
	}

	string S; cin >> S;
	vector<int> breaks;
	for (int i = 0; i+1 < N; i++) {
		if (S[i] == 'X' && S[i+1] == 'X') {
			cout << 0 << ' ' << 0 << ' ' << 0 << '\n';
			exit(0);
		}
		if (S[i] == '-' && S[i+1] == '-') breaks.push_back(i);
	}

	if (breaks.empty()) {
		int numX = 0;
		for (int i = 0; i < N; i++) {
			numX += S[i] == 'X';
		}
		num valC = num(1);
		num ans = inv(num(2));
		for (int i = 0; i < numX; i++) {
			ans -= valC;
			valC -= H[i];
		}
		cout << inv(num(2)) << ' ' << 0 << ' ' << ans << '\n';
		exit(0);
	}

	num ans = 1;
	for (int z = 0; z+1 < int(breaks.size()); z++) {
		ans *= H[(breaks[z+1] - breaks[z]) / 2];
	}
	{
		num valC = num(1);
		for (int i = 0; i < (breaks.front()+1)/2; i++) {
			valC -= H[i];
		}
		ans *= valC;
	}
	{
		num valC = num(1);
		for (int i = 0; i < (N-1-breaks.back())/2; i++) {
			valC -= H[i];
		}
		ans *= valC;
	}
	cout << 0 << ' ' << 0 << ' ' << ans << '\n';

	return 0;
}

// Note that the constraint is satisfied iff all dashes are next to an earlier X
// This means that -- can't interact with each other, so we just want to solve
// (...-)-X-X-X-X-(-...)
//
// Let f be the g.f. of P(...X-X-X... is good) * y^{2 * #X's}
// Then, caseworking on min gives
// y f'(y) - (f(y) - 1) = y^2 f(y)
// f(y) = cosh(y) / (cosh(y) - y * sinh(y))
//
// Let g be the g.f. of P(...X-X-X-... is good) * y^{2 * #X's}
// y g'(y) = y^2 f(y) g(y)
// g(y) = 1 / (cosh(y) - y * sinh(y))
//
// Let h be the g.f. of P(...-X-X-X-... is good) * y^{2 * #X's} (>= 1 X's)
// h(y) = (cosh(y) - sinh(y)/y) / (cosh(y) - y * sinh(y))
//
// So we just need terms of h(y)
//
// Also, for a given location, P(...-|- stuff on the left ends in -) = 1 - 1/1 + 1/2! - 1/3! ... = 1/e
// (We're caseworking on length of increasing sequence to the -)
//
// Then, let c be the g.f. of e * P(...X-X-X-|-) * y^{2 * #X's} is
// c(y) = 1 / (1-y^2) - y^2 / (1-y^2) * h(y)
// = cosh(h) / (cosh(y) - y * sinh(y)) = f(y)
//
// Let d be the g.f. of e^2 * P(...X-X-X...) * y^{2 * #X's}
// We do inclusion-exclusion:
// y^2 d(y) - d(y) + (e^2 - 1) / 2 y^2 = (c(y) - 1) * y^2
// d(y) = (e^2+1)/2 / (1-y^2) - c(y) * y^2 / (1-y^2)
//
// We can use x = y^2 to simplify/speed up the calculations
