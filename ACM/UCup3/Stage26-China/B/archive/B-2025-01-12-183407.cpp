#include <bits/stdc++.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wsign-compare"
namespace indy256 {

using namespace std;

// Fast Fourier transform
// https://cp-algorithms.com/algebra/fft.html
// https://drive.google.com/file/d/1B9BIfATnI_qL6rYiE5hY9bh20SMVmHZ7/view

using cpx = complex<double>;
const double PI = acos(-1);
vector<cpx> roots = {{0, 0}, {1, 0}};

void ensure_capacity(int min_capacity) {
	for (int len = roots.size(); len < min_capacity; len *= 2) {
		for (int i = len >> 1; i < len; i++) {
			roots.emplace_back(roots[i]);
			double angle = 2 * PI * (2 * i + 1 - len) / (len * 2);
			roots.emplace_back(cos(angle), sin(angle));
		}
	}
}

void fft(vector<cpx> &z, bool inverse) {
	int n = z.size();
	assert((n & (n - 1)) == 0);
	ensure_capacity(n);
	for (unsigned i = 1, j = 0; i < n; i++) {
		int bit = n >> 1;
		for (; j >= bit; bit >>= 1)
			j -= bit;
		j += bit;
		if (i < j)
			swap(z[i], z[j]);
	}
	for (int len = 1; len < n; len <<= 1) {
		for (int i = 0; i < n; i += len * 2) {
			for (int j = 0; j < len; j++) {
				cpx root = inverse ? conj(roots[j + len]) : roots[j + len];
				cpx u = z[i + j];
				cpx v = z[i + j + len] * root;
				z[i + j] = u + v;
				z[i + j + len] = u - v;
			}
		}
	}
	if (inverse)
		for (int i = 0; i < n; i++)
			z[i] /= n;
}

vector<int> multiply_bigint(const vector<int> &a, const vector<int> &b, int base) {
	int need = a.size() + b.size();
	int n = 1;
	while (n < need)
		n <<= 1;
	vector<cpx> p(n);
	for (size_t i = 0; i < n; i++) {
		p[i] = cpx(i < a.size() ? a[i] : 0, i < b.size() ? b[i] : 0);
	}
	fft(p, false);
	// a[w[k]] = (p[w[k]] + conj(p[w[n-k]])) / 2
	// b[w[k]] = (p[w[k]] - conj(p[w[n-k]])) / (2*i)
	vector<cpx> ab(n);
	cpx r(0, -0.25);
	for (int i = 0; i < n; i++) {
		int j = (n - i) & (n - 1);
		ab[i] = (p[i] * p[i] - conj(p[j] * p[j])) * r;
	}
	fft(ab, true);
	vector<int> result(need);
	long long carry = 0;
	for (int i = 0; i < need; i++) {
		long long d = (long long)(ab[i].real() + 0.5) + carry;
		carry = d / base;
		result[i] = d % base;
	}
	return result;
}

vector<int> multiply_mod(const vector<int> &a, const vector<int> &b, int m) {
	int need = a.size() + b.size() - 1;
	int n = 1;
	while (n < need)
		n <<= 1;
	vector<cpx> A(n);
	for (size_t i = 0; i < a.size(); i++) {
		int x = (a[i] % m + m) % m;
		A[i] = cpx(x & ((1 << 15) - 1), x >> 15);
	}
	fft(A, false);

	vector<cpx> B(n);
	for (size_t i = 0; i < b.size(); i++) {
		int x = (b[i] % m + m) % m;
		B[i] = cpx(x & ((1 << 15) - 1), x >> 15);
	}
	fft(B, false);

	vector<cpx> fa(n);
	vector<cpx> fb(n);
	for (int i = 0, j = 0; i < n; i++, j = n - i) {
		cpx a1 = (A[i] + conj(A[j])) * cpx(0.5, 0);
		cpx a2 = (A[i] - conj(A[j])) * cpx(0, -0.5);
		cpx b1 = (B[i] + conj(B[j])) * cpx(0.5, 0);
		cpx b2 = (B[i] - conj(B[j])) * cpx(0, -0.5);
		fa[i] = a1 * b1 + a2 * b2 * cpx(0, 1);
		fb[i] = a1 * b2 + a2 * b1;
	}

	fft(fa, true);
	fft(fb, true);
	vector<int> res(need);
	for (int i = 0; i < need; i++) {
		long long aa = (long long)(fa[i].real() + 0.5);
		long long bb = (long long)(fb[i].real() + 0.5);
		long long cc = (long long)(fa[i].imag() + 0.5);
		res[i] = (aa % m + (bb % m << 15) + (cc % m << 30)) % m;
	}
	return res;
}

constexpr int digits(int base) noexcept {
	return base <= 1 ? 0 : 1 + digits(base / 10);
}

constexpr int base = 1000'000'000;
constexpr int base_digits = digits(base);

constexpr int fft_base = 10'000;  // fft_base^2 * n / fft_base_digits <= 10^15 for double
constexpr int fft_base_digits = digits(fft_base);

struct bigint {
	// value == 0 is represented by empty z
	vector<int> z;  // digits

	// sign == 1 <==> value >= 0
	// sign == -1 <==> value < 0
	int sign;

	bigint(long long v = 0) { *this = v; }

	bigint &operator=(long long v) {
		sign = v < 0 ? -1 : 1;
		v *= sign;
		z.clear();
		for (; v > 0; v = v / base)
			z.push_back((int)(v % base));
		return *this;
	}

	bigint(const string &s) { read(s); }

	bigint &operator+=(const bigint &other) {
		if (sign == other.sign) {
			for (int i = 0, carry = 0; i < other.z.size() || carry; ++i) {
				if (i == z.size())
					z.push_back(0);
				z[i] += carry + (i < other.z.size() ? other.z[i] : 0);
				carry = z[i] >= base;
				if (carry)
					z[i] -= base;
			}
		} else if (other != 0 /* prevent infinite loop */) {
			*this -= -other;
		}
		return *this;
	}

	friend bigint operator+(bigint a, const bigint &b) {
		a += b;
		return a;
	}

	bigint &operator-=(const bigint &other) {
		if (sign == other.sign) {
			if ((sign == 1 && *this >= other) || (sign == -1 && *this <= other)) {
				for (int i = 0, carry = 0; i < other.z.size() || carry; ++i) {
					z[i] -= carry + (i < other.z.size() ? other.z[i] : 0);
					carry = z[i] < 0;
					if (carry)
						z[i] += base;
				}
				trim();
			} else {
				*this = other - *this;
				this->sign = -this->sign;
			}
		} else {
			*this += -other;
		}
		return *this;
	}

	friend bigint operator-(bigint a, const bigint &b) {
		a -= b;
		return a;
	}

	bigint &operator*=(int v) {
		if (v < 0)
			sign = -sign, v = -v;
		for (int i = 0, carry = 0; i < z.size() || carry; ++i) {
			if (i == z.size())
				z.push_back(0);
			long long cur = (long long)z[i] * v + carry;
			carry = (int)(cur / base);
			z[i] = (int)(cur % base);
		}
		trim();
		return *this;
	}

	bigint operator*(int v) const { return bigint(*this) *= v; }

	friend pair<bigint, bigint> divmod(const bigint &a1, const bigint &b1) {
		int norm = base / (b1.z.back() + 1);
		bigint a = a1.abs() * norm;
		bigint b = b1.abs() * norm;
		bigint q, r;
		q.z.resize(a.z.size());

		for (int i = (int)a.z.size() - 1; i >= 0; i--) {
			r *= base;
			r += a.z[i];
			int s1 = b.z.size() < r.z.size() ? r.z[b.z.size()] : 0;
			int s2 = b.z.size() - 1 < r.z.size() ? r.z[b.z.size() - 1] : 0;
			int d = (int)(((long long)s1 * base + s2) / b.z.back());
			r -= b * d;
			while (r < 0)
				r += b, --d;
			q.z[i] = d;
		}

		q.sign = a1.sign * b1.sign;
		r.sign = a1.sign;
		q.trim();
		r.trim();
		return {q, r / norm};
	}

	friend bigint sqrt(const bigint &a1) {
		bigint a = a1;
		while (a.z.empty() || a.z.size() % 2 == 1)
			a.z.push_back(0);

		int n = a.z.size();

		int firstDigit = (int)::sqrt((double)a.z[n - 1] * base + a.z[n - 2]);
		int norm = base / (firstDigit + 1);
		a *= norm;
		a *= norm;
		while (a.z.empty() || a.z.size() % 2 == 1)
			a.z.push_back(0);

		bigint r = (long long)a.z[n - 1] * base + a.z[n - 2];
		firstDigit = (int)::sqrt((double)a.z[n - 1] * base + a.z[n - 2]);
		int q = firstDigit;
		bigint res;

		for (int j = n / 2 - 1; j >= 0; j--) {
			for (;; --q) {
				bigint r1 = (r - (res * 2 * base + q) * q) * base * base +
					(j > 0 ? (long long)a.z[2 * j - 1] * base + a.z[2 * j - 2] : 0);
				if (r1 >= 0) {
					r = r1;
					break;
				}
			}
			res *= base;
			res += q;

			if (j > 0) {
				int d1 = res.z.size() + 2 < r.z.size() ? r.z[res.z.size() + 2] : 0;
				int d2 = res.z.size() + 1 < r.z.size() ? r.z[res.z.size() + 1] : 0;
				int d3 = res.z.size() < r.z.size() ? r.z[res.z.size()] : 0;
				q = (int)(((long long)d1 * base * base + (long long)d2 * base + d3) / (firstDigit * 2));
			}
		}

		res.trim();
		return res / norm;
	}

	bigint operator/(const bigint &v) const { return divmod(*this, v).first; }

	bigint operator%(const bigint &v) const { return divmod(*this, v).second; }

	bigint &operator/=(int v) {
		if (v < 0)
			sign = -sign, v = -v;
		for (int i = (int)z.size() - 1, rem = 0; i >= 0; --i) {
			long long cur = z[i] + rem * (long long)base;
			z[i] = (int)(cur / v);
			rem = (int)(cur % v);
		}
		trim();
		return *this;
	}

	bigint operator/(int v) const { return bigint(*this) /= v; }

	int operator%(int v) const {
		if (v < 0)
			v = -v;
		int m = 0;
		for (int i = (int)z.size() - 1; i >= 0; --i)
			m = (int)((z[i] + m * (long long)base) % v);
		return m * sign;
	}

	bigint &operator*=(const bigint &v) {
		*this = *this * v;
		return *this;
	}

	bigint &operator/=(const bigint &v) {
		*this = *this / v;
		return *this;
	}

	bigint &operator%=(const bigint &v) {
		*this = *this % v;
		return *this;
	}

	bool operator<(const bigint &v) const {
		if (sign != v.sign)
			return sign < v.sign;
		if (z.size() != v.z.size())
			return z.size() * sign < v.z.size() * v.sign;
		for (int i = (int)z.size() - 1; i >= 0; i--)
			if (z[i] != v.z[i])
				return z[i] * sign < v.z[i] * sign;
		return false;
	}

	bool operator>(const bigint &v) const { return v < *this; }

	bool operator<=(const bigint &v) const { return !(v < *this); }

	bool operator>=(const bigint &v) const { return !(*this < v); }

	bool operator==(const bigint &v) const { return sign == v.sign && z == v.z; }

	bool operator!=(const bigint &v) const { return !(*this == v); }

	void trim() {
		while (!z.empty() && z.back() == 0)
			z.pop_back();
		if (z.empty())
			sign = 1;
	}

	bool isZero() const { return z.empty(); }

	friend bigint operator-(bigint v) {
		if (!v.z.empty())
			v.sign = -v.sign;
		return v;
	}

	bigint abs() const { return sign == 1 ? *this : -*this; }

	long long longValue() const {
		long long res = 0;
		for (int i = (int)z.size() - 1; i >= 0; i--)
			res = res * base + z[i];
		return res * sign;
	}

	friend bigint gcd(const bigint &a, const bigint &b) { return b.isZero() ? a : gcd(b, a % b); }

	friend bigint lcm(const bigint &a, const bigint &b) { return a / gcd(a, b) * b; }

	void read(const string &s) {
		sign = 1;
		z.clear();
		int pos = 0;
		while (pos < s.size() && (s[pos] == '-' || s[pos] == '+')) {
			if (s[pos] == '-')
				sign = -sign;
			++pos;
		}
		for (int i = (int)s.size() - 1; i >= pos; i -= base_digits) {
			int x = 0;
			for (int j = max(pos, i - base_digits + 1); j <= i; j++)
				x = x * 10 + s[j] - '0';
			z.push_back(x);
		}
		trim();
	}

	friend istream &operator>>(istream &stream, bigint &v) {
		string s;
		stream >> s;
		v.read(s);
		return stream;
	}

	friend ostream &operator<<(ostream &stream, const bigint &v) {
		if (v.sign == -1)
			stream << '-';
		stream << (v.z.empty() ? 0 : v.z.back());
		for (int i = (int)v.z.size() - 2; i >= 0; --i)
			stream << setw(base_digits) << setfill('0') << v.z[i];
		return stream;
	}

	static vector<int> convert_base(const vector<int> &a, int old_digits, int new_digits) {
		vector<long long> p(max(old_digits, new_digits) + 1);
		p[0] = 1;
		for (int i = 1; i < p.size(); i++)
			p[i] = p[i - 1] * 10;
		vector<int> res;
		long long cur = 0;
		int cur_digits = 0;
		for (int v : a) {
			cur += v * p[cur_digits];
			cur_digits += old_digits;
			while (cur_digits >= new_digits) {
				res.push_back(int(cur % p[new_digits]));
				cur /= p[new_digits];
				cur_digits -= new_digits;
			}
		}
		res.push_back((int)cur);
		while (!res.empty() && res.back() == 0)
			res.pop_back();
		return res;
	}

	bigint operator*(const bigint &v) const {
		if (min(z.size(), v.z.size()) < 150)
			return mul_simple(v);
		bigint res;
		res.sign = sign * v.sign;
		res.z = multiply_bigint(convert_base(z, base_digits, fft_base_digits),
			convert_base(v.z, base_digits, fft_base_digits), fft_base);
		res.z = convert_base(res.z, fft_base_digits, base_digits);
		res.trim();
		return res;
	}

	bigint mul_simple(const bigint &v) const {
		bigint res;
		res.sign = sign * v.sign;
		res.z.resize(z.size() + v.z.size());
		for (int i = 0; i < z.size(); ++i)
			if (z[i])
				for (int j = 0, carry = 0; j < v.z.size() || carry; ++j) {
					long long cur = res.z[i + j] + (long long)z[i] * (j < v.z.size() ? v.z[j] : 0) + carry;
					carry = (int)(cur / base);
					res.z[i + j] = (int)(cur % base);
				}
		res.trim();
		return res;
	}

	friend int signum(const bigint& v) {
		return v.sign == -1 ? -1 : (v == 0) ? 0 : 1;
	}
};

mt19937 rng(1);

bigint random_bigint(int n) {
	string s;
	for (int i = 0; i < n; i++) {
		s += uniform_int_distribution<int>('0', '9')(rng);
	}
	return bigint(s);
}

}
#pragma GCC diagnostic pop

template <typename T> struct Point {
public:
	T x, y;
	Point() : x(0), y(0) {}
	Point(T x_, T y_) : x(x_), y(y_) {}
	template <typename U> explicit Point(const Point<U>& p) : x(p.x), y(p.y) {}
	Point(const std::pair<T, T>& p) : x(p.first), y(p.second) {}
	Point(const std::complex<T>& p) : x(real(p)), y(imag(p)) {}
	explicit operator std::pair<T, T> () const { return std::pair<T, T>(x, y); }
	explicit operator std::complex<T> () const { return std::complex<T>(x, y); }

	friend std::ostream& operator << (std::ostream& o, const Point& p) { return o << '(' << p.x << ',' << p.y << ')'; }
	friend std::istream& operator >> (std::istream& i, Point& p) { return i >> p.x >> p.y; }
	friend bool operator == (const Point& a, const Point& b) { return a.x == b.x && a.y == b.y; }
	friend bool operator != (const Point& a, const Point& b) { return !(a==b); }

	Point operator + () const { return Point(+x, +y); }
	Point operator - () const { return Point(-x, -y); }

	Point& operator += (const Point& p) { x += p.x, y += p.y; return *this; }
	Point& operator -= (const Point& p) { x -= p.x, y -= p.y; return *this; }
	Point& operator *= (const T& t) { x *= t, y *= t; return *this; }
	Point& operator /= (const T& t) { x /= t, y /= t; return *this; }

	friend Point operator + (const Point& a, const Point& b) { return Point(a.x+b.x, a.y+b.y); }
	friend Point operator - (const Point& a, const Point& b) { return Point(a.x-b.x, a.y-b.y); }
	friend Point operator * (const Point& a, const T& t) { return Point(a.x*t, a.y*t); }
	friend Point operator * (const T& t ,const Point& a) { return Point(t*a.x, t*a.y); }
	friend Point operator / (const Point& a, const T& t) { return Point(a.x/t, a.y/t); }

	T dist2() const { return x * x + y * y; }
	auto dist() const { return std::sqrt(dist2()); }
	Point unit() const { return *this / this->dist(); }
	auto angle() const { return std::atan2(y, x); }

	T int_norm() const { return __gcd(x,y); }
	Point int_unit() const { if (!x && !y) return *this; return *this / this->int_norm(); }

	// Convenient free-functions, mostly for generic interop
	friend auto norm(const Point& a) { return a.dist2(); }
	friend auto abs(const Point& a) { return a.dist(); }
	friend auto unit(const Point& a) { return a.unit(); }
	friend auto arg(const Point& a) { return a.angle(); }
	friend auto int_norm(const Point& a) { return a.int_norm(); }
	friend auto int_unit(const Point& a) { return a.int_unit(); }

	Point perp_cw() const { return Point(y, -x); }
	Point perp_ccw() const { return Point(-y, x); }

	friend T dot(const Point& a, const Point& b) { return a.x * b.x + a.y * b.y; }
	friend T cross(const Point& a, const Point& b) { return a.x * b.y - a.y * b.x; }
	friend T cross3(const Point& a, const Point& b, const Point& c) { return cross(b-a, c-a); }

	// Complex numbers and rotation
	friend Point conj(const Point& a) { return Point(a.x, -a.y); }

	// Returns conj(a) * b
	friend Point dot_cross(const Point& a, const Point& b) { return Point(dot(a, b), cross(a, b)); }
	friend Point cmul(const Point& a, const Point& b) { return dot_cross(conj(a), b); }
	friend Point cdiv(const Point& a, const Point& b) { return dot_cross(b, a) / b.norm(); }

	// Must be a unit vector; otherwise multiplies the result by abs(u)
	Point rotate(const Point& u) const { return dot_cross(conj(u), *this); }
	Point unrotate(const Point& u) const { return dot_cross(u, *this); }

	friend bool lex_less(const Point& a, const Point& b) {
		return std::tie(a.x, a.y) < std::tie(b.x, b.y);
	}

	friend bool same_dir(const Point& a, const Point& b) { return cross(a,b) == 0 && dot(a,b) > 0; }

	// check if 180 <= s..t < 360
	friend bool is_reflex(const Point& a, const Point& b) { auto c = cross(a,b); return c ? (c < 0) : (dot(a, b) < 0); }

	// operator < (s,t) for angles in [base,base+2pi)
	friend bool angle_less(const Point& base, const Point& s, const Point& t) {
		int r = is_reflex(base, s) - is_reflex(base, t);
		return r ? (r < 0) : (0 < cross(s, t));
	}

	friend auto angle_cmp(const Point& base) {
		return [base](const Point& s, const Point& t) { return angle_less(base, s, t); };
	}
	friend auto angle_cmp_center(const Point& center, const Point& dir) {
		return [center, dir](const Point& s, const Point& t) -> bool { return angle_less(dir, s-center, t-center); };
	}

	// is p in [s,t] taken ccw? 1/0/-1 for in/border/out
	friend int angle_between(const Point& s, const Point& t, const Point& p) {
		if (same_dir(p, s) || same_dir(p, t)) return 0;
		return angle_less(s, p, t) ? 1 : -1;
	}
};

using bigint = indy256::bigint;

struct frac_t {
	bigint numer;
	bigint denom;

	void reduce() {
		auto g = gcd(numer, denom).abs();
		numer /= g;
		denom /= g;
	}

	friend frac_t operator - (const frac_t& a, const frac_t& b) {
		frac_t r{a.numer * b.denom - b.numer * a.denom, a.denom * b.denom};
		r.reduce();
		return r;
	}

	friend frac_t operator + (const frac_t& a, const frac_t& b) {
		frac_t r{a.numer * b.denom + b.numer * a.denom, a.denom * b.denom};
		r.reduce();
		return r;
	}

	friend std::strong_ordering operator <=> (const frac_t& a, const frac_t& b) {
		return signum(a.numer * b.denom - b.numer * a.denom) <=> 0;
	}
};


int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		std::mt19937 mt(48);
		int N; cin >> N;
		using pt_t = Point<int64_t>;
		std::vector<pt_t> P(N);
		for (auto& p : P) cin >> p;

		int64_t A = std::uniform_int_distribution(-int64_t(1e14), int64_t(1e14))(mt);
		int64_t B = std::uniform_int_distribution(-int64_t(1e14), int64_t(1e14))(mt);
		//int64_t A = 1, B = 1;

		pt_t dir(A, B);

		for (auto& p : P) p = p.unrotate(dir);
		std::sort(P.begin(), P.end(), [&](auto a, auto b) { return lex_less(a, b); });

		//for (int i = 0; i < N; i++) cerr << P[i] << '\n';

		std::vector<frac_t> vals(N+1);

		vals[0] = {0, 1};
		for (int i = 1; i < N-1; i++) {
			cout << '?' << ' ' << A << ' ' << B << ' ' << -P[i].x << '\n' << flush;
			std::string numer; cin >> numer;
			std::string denom; cin >> denom;
			vals[i] = {numer, denom};
		}
		vals[N-1] = {0, 1};

		std::vector<frac_t> slopes(N+1);
		slopes[0] = {0, 1};
		for (int i = 0; i+1 < N; i++) {
			frac_t slope = vals[i+1] - vals[i];
			assert(P[i+1].x > P[i].x);
			slope.denom *= bigint(P[i+1].x - P[i].x);
			slopes[i+1] = slope;
		}
		slopes[N] = {0, 1};

		std::vector<frac_t> delta_slopes(N+1);
		for (int i = 0; i < N; i++) {
			delta_slopes[i] = slopes[i+1] - slopes[i];
		}

		std::vector<int> prv(N, -1);
		std::vector<int> nxt(N, -1);
		for (int i = 0; i < N; i++) {
			//cerr << "trying " << i << '\n';
			frac_t d = delta_slopes[i];
			auto slope = [&](pt_t p) -> frac_t {
				assert(p.x != 0);
				if (p.x < 0) return frac_t{bigint(-p.y), bigint(-p.x)};
				else return frac_t{bigint(p.y), bigint(p.x)};
			};
			if (prv[i] != -1) {
				d = d - slope(P[prv[i]] - P[i]);
			}
			if (nxt[i] != -1) {
				d = d + slope(P[nxt[i]] - P[i]);
			}
			if (prv[i] != -1 && nxt[i] != -1) {
				assert(d.numer == 0);
				continue;
			}

			//cerr << "target slope " << d.numer << '/' << d.denom << '\n';

			// delta_slope = slope(P[prv] - P[i]) - slope(P[nxt] - P[i])
			std::vector<std::pair<frac_t, int>> v; v.reserve(N);
			for (int j = i+1; j < N; j++) {
				v.push_back({slope(P[j] - P[i]), j});
				//cerr << "cnd " << v.back().first.numer << '/' << v.back().first.denom << ' ' << j << '\n';
			}
			std::sort(v.begin(), v.end());
			if (prv[i] == -1 && nxt[i] == -1) {
				//cerr << "finding prv and nxt" << '\n';
				assert(d.denom > 0);
				for (int a = 0, b = 0; true; ) {
					frac_t cnd = v[b].first - v[a].first - d;
					assert(cnd.denom > 0);
					if (cnd.numer == 0) {
						prv[i] = v[b].second;
						nxt[i] = v[a].second;
						nxt[prv[i]] = i;
						prv[nxt[i]] = i;
						break;
					} else if (cnd.numer < 0) {
						b++;
					} else {
						a++;
					}
				}
			} else if (prv[i] == -1) {
				//cerr << "finding prv" << '\n';
				for (int a = 0; true; a++) {
					if ((v[a].first - d).numer == 0) {
						prv[i] = v[a].second;
						nxt[prv[i]] = i;
						break;
					}
				}
			} else if (nxt[i] == -1) {
				//cerr << "finding nxt" << '\n';
				for (int b = 0; true; b++) {
					if ((v[b].first + d).numer == 0) {
						nxt[i] = v[b].second;
						prv[nxt[i]] = i;
						break;
					}
				}
			} else assert(false);
			//cerr << "have " << i << ' ' << prv[i] << ' ' << nxt[i] << '\n';
		}

		std::vector<int> cyc; cyc.reserve(N);
		for (int cur = 0; true; cur = nxt[cur]) {
			cyc.push_back(cur);
			if (nxt[cur] == 0) break;
		}
		assert(int(cyc.size()) == N);

		cout << '!' << '\n';
		for (int i : cyc) {
			using pbig = Point<__int128_t>;
			pbig dir_big(A, B);
			pbig res = pbig(P[i]).rotate(dir_big) / dir_big.dist2();
			cout << int(res.x) << ' ' << int(res.y) << '\n';
		}
		cout << flush;
	}

	return 0;
}
