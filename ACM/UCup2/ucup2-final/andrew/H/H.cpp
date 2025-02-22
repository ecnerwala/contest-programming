#include <bits/stdc++.h>

using u32 = unsigned int;
using u64 = unsigned long long;

constexpr unsigned int P = 998244353, P2 = P * 2;

struct Fp {
	u32 v;
	Fp() : v() {}
	Fp(u32 v) : v(v) {}

	Fp operator+(const Fp& rhs) const {
		u32 a = v + rhs.v;
		if (a >= P) a -= P;
		return a;
	}
	Fp& operator+=(const Fp& rhs) { if ((v += rhs.v) >= P) v -= P; return *this; }
	Fp operator-() const { return v ? P - v : v; }
	Fp operator-(const Fp& rhs) const {
		u32 a = v - rhs.v;
		if (int(a) < 0) a += P;
		return a;
	}
	Fp& operator-=(const Fp& rhs) { if (int(v -= rhs.v) < 0) v += P; return *this; }
	Fp operator*(const Fp& rhs) const { return u32(u64(v) * rhs.v % P); }
	Fp& operator*=(const Fp& rhs) { v = u64(v) * rhs.v % P; return *this; }
	Fp inv() const;
	Fp operator/(const Fp& rhs) const { return *this * rhs.inv(); }
	Fp& operator/=(const Fp& rhs) { return *this *= rhs.inv(); }
	operator u32() const { return v; }

	Fp quo2() const { return (v & 1) ? ((v + P) >> 1) : (v >> 1); }
};

Fp mpow(const Fp& a, unsigned k) {
	if (k == 0) return 1u;
	Fp ret = mpow(a * a, k >> 1);
	if (k & 1) ret *= a;
	return ret;
}
Fp Fp::inv() const { return mpow(*this, P - 2); }

std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
std::uniform_int_distribution<int> uid(0, P - 1);

using Poly = std::vector<Fp>;
using Vec = std::vector<Fp>;
using Matrix = std::vector<Vec>;

const int _ = 505;

int N;

Matrix transpose(const Matrix& mat) {
	Matrix ret(N, Vec(N));
	for (int i = 0; i != N; ++i)
		for (int j = 0; j != N; ++j)
			ret[i][j] = mat[j][i];
	return ret;
}

Matrix operator*(const Matrix& lhs, Matrix rhs) {
	for (int i = 0; i != N; ++i)
		for (int j = 0; j != i; ++j)
			std::swap(rhs[i][j], rhs[j][i]);
	Matrix ret(N, Vec(N));
	for (int i = 0; i != N; ++i)
		for (int j = 0; j != N; ++j)
			for (int k = 0; k != N; ++k)
				ret[i][j] += lhs[i][k] * rhs[j][k];
	return ret;
}

Vec operator+(const Vec& lhs, const Vec& rhs) {
	Vec ret(N);
	for (int i = 0; i != N; ++i) ret[i] = lhs[i] + rhs[i];
	return ret;
}
Vec operator-(const Vec& lhs, const Vec& rhs) {
	Vec ret(N);
	for (int i = 0; i != N; ++i) ret[i] = lhs[i] - rhs[i];
	return ret;
}
Vec operator*(const Vec& lhs, const Fp& rhs) {
	Vec ret(N);
	for (int i = 0; i != N; ++i) ret[i] = lhs[i] * rhs;
	return ret;
}

struct Basis {
	std::vector<Vec> vectors;
	Matrix reduced, coefficients;
	Basis() : reduced(N), coefficients(N) {}
	Basis(const Basis& basis) : vectors(basis.vectors), reduced(basis.reduced), coefficients(basis.coefficients) {}

	Poly insert(Vec vec) {
		int id = vectors.size();
		Vec coefficient(N);

		vectors.push_back(vec);
		for (int i = 0; i != N; ++i) {
			if (vec[i] == 0) continue;
			if (!reduced[i].empty()) {
				Fp c = vec[i];
				vec = vec - reduced[i] * c;
				coefficient = coefficient - coefficients[i] * c;
			} else {
				Fp nv = vec[i].inv();
				coefficient[id] = 1;
				reduced[i] = vec * nv;
				coefficients[i] = coefficient * nv;
				return Poly();
			}
		}
		coefficient.resize(id + 1);
		coefficient[id] = 1;
		return coefficient;
	}

	Matrix inv() {
		for (int i = N - 1; i; --i)
			for (int j = 0; j != i; ++j) {
				Fp c = reduced[j][i];
				reduced[j] = reduced[j] - reduced[i] * c;
				coefficients[j] = coefficients[j] - coefficients[i] * c;
			}
		return coefficients;
	}
};

Vec operator*(const Matrix& mat, const Vec& vec) {
	Vec ret(N);
	for (int i = 0; i != N; ++i)
		for (int j = 0; j != N; ++j)
			ret[i] += mat[i][j] * vec[j];
	return ret;
}

Poly div(Poly a, Poly b) {
	int n = a.size() - 1, m = b.size() - 1;
	Poly ret(n - m + 1);
	for (int i = n; i >= m; --i) {
		ret[i - m] = a[i];
		Fp c = -a[i];
		for (int j = 0; j <= m; ++j) {
			a[i - m + j] += c * b[j];
		}
	}
	for (int i = 0; i != m; ++i) assert(a[i] == 0);
	return ret;
}

Poly mod(Poly a, Poly b) {
	int n = b.size() - 1;
	if (a.size() < n) a.resize(n);
	int m = a.size() - 1;
	for (int i = m; i >= n; --i)
		for (int j = 1; j <= n; ++j)
			a[i - j] -= a[i] * b[n - j];
	a.resize(n);
	return a;
}

Poly powmod(Poly modulo, u64 K) {
	int n = modulo.size() - 1;
	if (K == 0) {
		Poly ret(n); ret[0] = 1;
		return ret;
	}
	Poly half = powmod(modulo, K >> 1);
	Poly ret(n * 2 - 1);
	for (int i = 0; i != n; ++i)
		for (int j = 0; j != n; ++j)
			ret[i + j] += half[i] * half[j];
	if (K & 1) ret.insert(ret.begin(), 0);
	return mod(ret, modulo);
}

int main() {
	std::ios::sync_with_stdio(false); std::cin.tie(nullptr); std::cout.tie(nullptr);

	std::cin >> N;
	
	Matrix a(N, Vec(N));
	for (int i = 0; i != N; ++i)
		for (int j = 0; j != N; ++j)
			std::cin >> a[i][j].v;
	
	std::vector<Poly> elementaryDivisors;

	Basis basis = Basis();
	while (basis.vectors.size() < N) {
		// std::cerr << basis.vectors.size() << '\n';

		Vec initVector(N);
		for (int i = 0; i != N; ++i) initVector[i].v = uid(rng);
		Vec iterVector = initVector;
		Basis test = basis;
		Poly coefficient;
		while (true) {
			coefficient = test.insert(iterVector);
			if (!coefficient.empty()) break;
			iterVector = a * iterVector;
		}

		Poly minimalPolynomial(coefficient.begin() + basis.vectors.size(), coefficient.end());
		int minPolyDegree = minimalPolynomial.size() - 1;

		int pre = 0;
		for (int i = 0; i != elementaryDivisors.size(); ++i) {
			int degree = elementaryDivisors[i].size() - 1;
			if (degree <= minPolyDegree) {
				pre += degree;
				continue;
			}
			// std::cerr << degree << ' ' << minPolyDegree << '\n';
			// std::cerr << "COEFF ";
			// for (int j = 0; j != degree; ++j)
			// 	std::cerr << coefficient[pre + j].v << ' ';
			Poly res = div(Poly(coefficient.begin() + pre, coefficient.begin() + pre + degree), minimalPolynomial);
			// std::cerr << "SIZ " << res.size() << '\n';
			// std::cerr << "RES:      " << res[0] << '\n';
			for (int j = 0; j != degree - minPolyDegree; ++j)
				initVector = initVector + basis.vectors[pre + j] * res[j];
			pre += degree;
		}
		
		elementaryDivisors.push_back(minimalPolynomial);
		for (int rep = 0; rep != minPolyDegree; ++rep) {
			basis.insert(initVector);
			initVector = a * initVector;
		}
	}

	int64_t ans = 0;
	int64_t coeff = 1;

	for (Poly poly : elementaryDivisors) {
		int degree = int(poly.size()) - 1;
		ans += int64_t(degree) * int64_t(coeff);
		coeff += 2;
	}
	std::cout << ans << '\n';

	return 0;
}
