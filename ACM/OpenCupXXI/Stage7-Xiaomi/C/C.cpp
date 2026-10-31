#include <bits/stdc++.h>

typedef unsigned long long ull;
struct FastMod {
private:
	ull b, m;
public:
	FastMod(ull b_) : b(b_), m(-1ULL / b) {}
	ull reduce(ull a) { // a % b + (0 or b)
		return a - (ull)((__uint128_t(m) * a) >> 64) * b;
	}
};

template <int& MOD_> struct modnum {
	static constexpr int& MOD = MOD_;
	//static_assert(MOD_ > 0, "MOD must be positive");
	static FastMod fm;

	static void setmod(int m) {
		fm = FastMod(m);
		MOD_ = m;
	}

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
		v -= MOD-o.v;
		v = (v < 0) ? v + MOD : v;
		return *this;
	}
	modnum& operator -= (const modnum& o) {
		v -= o.v;
		v = (v < 0) ? v + MOD : v;
		return *this;
	}
	modnum& operator *= (const modnum& o) {
		v = int(fm.reduce(ull(v) * ull(o.v)));
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

int MOD;
using num = modnum<MOD>;
template <> FastMod num::fm = {1};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;

	int mod_; cin >> mod_;
	num::setmod(mod_);

	vector<vector<int>> facs(N+1);

	vector<num> weight(N+1);
	for (int i = 1; i <= N; i++) {
		for (int j = 2*i; j <= N; j += i) {
			facs[j].push_back(i);
			weight[j]++;
		}
	}
	weight[1] = 1;
	for (int i = 1; i <= N; i++) {
		weight[i] = inv(weight[i]);
	}

	vector<num> root_path(N+1);
	for (int i = 1; i <= N; i++) {
		root_path[i] ++; // path ending at yourself
		for (int j = 2*i; j <= N; j += i) {
			root_path[j] += root_path[i] * weight[j];
		}
	}


	vector<vector<num>> num_paths(N+1);
	for (int i = N; i >= 1; i--) {
		num_paths[i].resize(N/i+1);
		num_paths[i][1] = 1;
		for (int j = 2; j <= N/i; j++) {
			for (int k = 1; k <= N/i/j; k++) {
				num_paths[i][j*k] += weight[i*j] * num_paths[i*j][k];
			}
		}
	}

	vector<num> all_paths(N+1);
	for (int i = N; i >= 1; i--) {
		all_paths[i] = 1;
		for (int j = 2*i; j <= N; j += i) {
			all_paths[i] += weight[j] * all_paths[j];
		}
	}

	num ans = 0;
	for (int i = 1; i <= N; i++) {
		ans += num(N) * root_path[i];
	}
	for (int i = N; i >= 1; i--) {
		all_paths[i] *= all_paths[i];
		for (int j = 2; j <= N/i; j++) {
			all_paths[i] -= num_paths[i][j] * num_paths[i][j] * all_paths[i*j];
		}

		ans -= all_paths[i] * root_path[i];
	}

	ans *= 2;
	cout << ans << '\n';

	return 0;
}
