#include <bits/stdc++.h>
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

vector<num> fact, ifact;

void init(){
	int N = 31000000;
	fact.resize(N);
	fact[0] = 1;
	for(int i = 1; i < N; i++) fact[i] = fact[i-1] * num(i);
	ifact.resize(N);
	ifact.back() = 1 / fact.back();
	for(int i = N - 1; i > 0; i--) ifact[i-1] = i * ifact[i];
}

num ncr(int n, int k){
	if(k < 0 || k > n) return 0;
	return fact[n] * ifact[k] * ifact[n-k];
}

int main(){
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	init();
	cerr << "done init" << '\n';

	int N, D, R;
	cin >> N >> D >> R;
	num ans = 0;

	int G = N + D;

	assert(R >= 1);

	auto get_asum = [&](int L){

		num F = ncr(L-2, R-1);
		if((L + R) & 1) F = -F;

		if (L == 1) F = 1;

		F *= ncr(N, L);
		return F;
	};
	vector<num> ncr_with_N(D+1, 0);
	for(int d = 0; d <= D; d++){
		ncr_with_N[d] = int(ncr(N + (D-d) - 1, (D-d)));
	}
	cerr << "done with precomp" << '\n';
	std::vector<num> sum_ncr(G+1);
	ans += get_asum(1) * ncr(N + D, D);
	for (int L = G; L >= 2; L--) {
		num asum = get_asum(L);
		num inside;
		if (L*2 > G) {
			inside = 0;
			for(int Y = 0; Y <= D / L; Y++){
				inside += ncr_with_N[L * Y];
			}
		} else {
			inside = sum_ncr[L*2];
			for(int Y = 1; Y <= D / L; Y += 2){
				inside += ncr_with_N[L * Y];
			}
		}
		sum_ncr[L] = inside;
		ans += asum * num(inside);
	}

	ans /= ncr(N + D - 1, D);
	cout << ans << '\n';
}
