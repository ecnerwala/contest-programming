#include <bits/stdc++.h>

namespace kactl {
using namespace std;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

template <typename num> num det(vector<vector<num>>& a) {
	int n = sz(a); num res = 1;
	rep(i,0,n) {
		int b = i;
		//rep(j,i+1,n) if (fabs(a[j][i]) > fabs(a[b][i])) b = j;
		if (i != b) swap(a[i], a[b]), res *= -1;
		res *= a[i][i];
		if (res == 0) return 0;
		rep(j,i+1,n) {
			num v = a[j][i] / a[i][i];
			if (v != 0) rep(k,i+1,n) a[j][k] -= v * a[i][k];
		}
	}
	return res;
}

#undef sz
#undef all
#undef rep

}

template <typename T> T mod_inv_in_range(T a, T m) {
	// assert(0 <= a && a < m);
	T x = a, y = m;
	// coeff of a in x and y
	T vx = 1, vy = 0;
	while (x) {
		T k = y / x;
		y %= x;
		vy -= k * vx;
		std::swap(x, y);
		std::swap(vx, vy);
	}
	assert(y == 1);
	return vy < 0 ? m + vy : vy;
}

template <typename T> struct extended_gcd_result {
	T gcd;
	T coeff_a, coeff_b;
};
template <typename T> extended_gcd_result<T> extended_gcd(T a, T b) {
	T x = a, y = b;
	// coeff of a and b in x and y
	T ax = 1, ay = 0;
	T bx = 0, by = 1;
	while (x) {
		T k = y / x;
		y %= x;
		ay -= k * ax;
		by -= k * bx;
		std::swap(x, y);
		std::swap(ax, ay);
		std::swap(bx, by);
	}
	return {y, ay, by};
}

template <typename T> T mod_inv(T a, T m) {
	a %= m;
	a = a < 0 ? a + m : a;
	return mod_inv_in_range(a, m);
}

template <int MOD_> struct modnum {
	static constexpr int MOD = MOD_;
	static_assert(MOD_ > 0, "MOD must be positive");

private:
	int v;

public:

	modnum() : v(0) {}
	modnum(int64_t v_) : v(int(v_ % MOD)) { if (v < 0) v += MOD; }
	explicit operator int() const { return v; }
	friend std::ostream& operator << (std::ostream& out, const modnum& n) { return out << int(n); }
	friend std::istream& operator >> (std::istream& in, modnum& n) { int64_t v_; in >> v_; n = modnum(v_); return in; }

	friend bool operator == (const modnum& a, const modnum& b) { return a.v == b.v; }
	friend bool operator != (const modnum& a, const modnum& b) { return a.v != b.v; }

	modnum inv() const {
		modnum res;
		res.v = mod_inv_in_range(v, MOD);
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
		v = int(int64_t(v) * int64_t(o.v) % MOD);
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

template <typename num>
std::vector<num> linearRec(const std::vector<num>& S, const std::vector<num>& tr, long long k) {
	int n = int(tr.size());
	assert(S.size() >= tr.size());

	auto combine = [&](std::vector<num> a, std::vector<num> b) {
		std::vector<num> res(n * 2 + 1);
		for (int i = 0; i <= n; i++) for (int j = 0; j <= n; j++) res[i + j] += a[i] * b[j];
		for (int i = 2 * n; i > n; --i) for (int j = 0; j < n; j++)
			res[i - 1 - j] += res[i] * tr[j];
		res.resize(n + 1);
		return res;
	};

	std::vector<num> pol(n + 1), e(pol);
	pol[0] = e[1] = 1;

	for (++k; k; k /= 2) {
		if (k % 2) pol = combine(pol, e);
		e = combine(e, e);
	}

	pol.erase(pol.begin());
	return pol;

	/*
	num res = 0;
	for (int i = 0; i < n; i++) res += pol[i] * S[i];
	return res;
	*/
}

using num = modnum<int(1e9)+7>;

using std::cerr;

const int BUCKET = 10'000'000;
const int N_BUCKET = 100;
const std::vector<num> BUCKET_FACT = {1,682498929,491101308,76479948,723816384,67347853,27368307,625544428,199888908,888050723,927880474,281863274,661224977,623534362,970055531,261384175,195888993,66404266,547665832,109838563,933245637,724691727,368925948,268838846,136026497,112390913,135498044,217544623,419363534,500780548,668123525,128487469,30977140,522049725,309058615,386027524,189239124,148528617,940567523,917084264,429277690,996164327,358655417,568392357,780072518,462639908,275105629,909210595,99199382,703397904,733333339,97830135,608823837,256141983,141827977,696628828,637939935,811575797,848924691,131772368,724464507,272814771,326159309,456152084,903466878,92255682,769795511,373745190,606241871,825871994,957939114,435887178,852304035,663307737,375297772,217598709,624148346,671734977,624500515,748510389,203191898,423951674,629786193,672850561,814362881,823845496,116667533,256473217,627655552,245795606,586445753,172114298,193781724,778983779,83868974,315103615,965785236,492741665,377329025,847549272,698611116,};

void precomp() {
	std::vector<num> vals; vals.reserve(N_BUCKET+1);
	num f = num(1);
	vals.push_back(f);
	for (int i = 1; i <= N_BUCKET * BUCKET; i++) {
		f *= num(i);
		if (i % BUCKET == 0) {
			vals.push_back(f);
		}
	}
	assert(int(vals.size()) == N_BUCKET+1);
	cerr << '{';
	for (num v : vals) cerr << v << ',';
	cerr << '}' << '\n';
}

num fact(int M) {
	int st = std::min(N_BUCKET, M / BUCKET);
	num r = BUCKET_FACT[st];
	for (int i = st * BUCKET + 1; i <= M; i++) r *= num(i);
	return r;
}

num solve(std::vector<num> A, int M) {
	std::vector<num> vals(A.size());
	for (int i = 0; i < int(A.size()); i++) {
		for (int j = i; j < int(A.size()); j++) {
			vals[j-i] += A[i] * A[j];
		}
	}

	// min things down to M
	if (int(vals.size()) > M) vals.resize(M);
	while (!vals.empty() && vals.back() == 0) vals.pop_back();
	if (vals.empty()) {
		return M == 0 ? num(1) : num(0);
	}
	assert(int(vals.size()) <= M);
	assert(int(vals.size()) >= 1);

	num leading_coeff = vals.back();
	num inv_leading_coeff = inv(leading_coeff);

	std::vector<num> tr; tr.reserve(int(vals.size()) * 2 - 2);
	tr.insert(tr.end(), vals.rbegin() + 1, vals.rend());
	tr.insert(tr.end(), vals.begin() + 1, vals.end());
	for (auto& v : tr) v *= -inv_leading_coeff;
	assert(int(tr.size()) == 2 * int(vals.size()) - 2);

	//for (auto& v : vals) { cerr << v << ' '; } cerr << '\n';

	int n = int(tr.size());
	auto mod_tr = [&](std::vector<num>& r) -> void {
		assert(int(r.size()) >= n);
		for (int i = int(r.size())-1; i >= n; i--) {
			for (int j = 0; j < n; j++) {
				r[i-1-j] += r[i] * tr[j];
			}
		}
		r.resize(n);
	};
	auto mul = [&](std::vector<num> a, std::vector<num> b, bool e = false) -> std::vector<num> {
		std::vector<num> res(int(a.size()) + int(b.size()));
		for (int i = 0; i < int(a.size()); i++) {
			for (int j = 0; j < int(b.size()); j++) {
				res[i + j + e] += a[i] * b[j];
			}
		}
		mod_tr(res);
		return res;
	};

	std::vector<num> pol(std::max(n, 1)), x(std::max(n, 2));
	pol[0] = 1;
	x[1] = 1;
	mod_tr(pol);
	mod_tr(x);
	for (int i = 30; i >= 0; i--) {
		pol = mul(pol, pol, (M >> i) & 1);
	}
	std::vector<num> id({num(1)});
	std::vector<std::vector<num>> C; C.reserve(n);
	for (int i = 0; i < n; i++) {
		C.push_back(pol);
		pol = mul(pol, id, true);
	}
	//for (const auto& r : C) { for (auto v : r) { cerr << v << ' '; } cerr << '\n'; } cerr << '\n';
	C.erase(C.begin(), C.begin() + n/2);
	for (int i = 0; i < n/2; i++) {
		C[i].erase(C[i].begin(), C[i].begin() + n/2);
	}
	//for (const auto& r : C) { for (auto v : r) { cerr << v << ' '; } cerr << '\n'; } cerr << '\n';
	return kactl::det(C) * pow(leading_coeff, M) * pow(pow(num(-1), M), n/2) * fact(M);
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	//precomp();

	int N, M; cin >> N >> M;
	std::vector<num> A(N+1);
	for (auto& a : A) cin >> a;

	cout << solve(A, M) << '\n';

	return 0;
}
