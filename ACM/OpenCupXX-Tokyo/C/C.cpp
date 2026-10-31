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

template <typename num>
vector<num> mul_mod(std::vector<num> a, std::vector<num> b, std::vector<num> tr) {
	int n = int(tr.size());
	assert(n == int(a.size()));
	assert(n == int(b.size()));
	std::vector<num> res(n * 2 - 1);
	for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) res[i + j] += a[i] * b[j];
	for (int i = 2 * n - 2; i >= n; --i) for (int j = 0; j < n; j++)
		res[i - 1 - j] += res[i] * tr[j];
	res.resize(n);
	return res;
}

template <typename num>
vector<num> linearRec(const std::vector<num>& tr, long long k) {
	int n = int(tr.size());
	assert(n);

	std::vector<num> pol(n), e(pol);
	pol[0] = 1;

	if (n > 1) e[1] = 1;
	else if (n == 1) e[0] = tr[0];
	else assert(false);

	for (; k; k /= 2) {
		if (k % 2) pol = mul_mod(pol, e, tr);
		e = mul_mod(e, e, tr);
	}

	return pol;
}

using num = modnum<998244353>;

using i64 = int64_t;

int main() {
	ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
	int N; cin >> N;
	i64 M, K; cin >> M >> K;
	K = M-K;
	assert(1 <= N && N < M);
	assert(1 <= K && K < M);

	vector<num> tr(N);
	for (int i = 0; i < N; i++) {
		cin >> tr[i];
	}
	{ // normalize
		num totProb = accumulate(tr.begin(), tr.end(), num(0));
		for (num& a : tr) a /= totProb;
	}
	auto getProbs = [&](int64_t d) {
		assert(d >= 0);
		vector<num> probs(2*N-1);

		vector<num> poly = linearRec(tr, d);
		for (int i = 0; i < int(probs.size()); i++) {
			// poly[j] is the dependence on j
			probs[i] = poly[N-1];

			poly.insert(poly.begin(), num(0));
			for (int j = 0; j < N; j++) {
				poly[N-1-j] += poly[N] * tr[j];
			}
			poly.pop_back();
		}

		return probs;
	};

	vector<num> probs = getProbs(M-N);

	vector<vector<num>> A(N, vector<num>(N));
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			for (int k = N-j; k <= N; k++) {
				A[i][j+k-N] += tr[k-1] * probs[(N-1-i) + j];
			}
		}

		num probSum = 0;
		for (int j = 0; j < N; j++) {
			probSum += A[i][j];
		}
		assert(probSum == 1);
	}

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			A[i][j] = -A[i][j];
		}
	}
	for (int j = 0; j < N; j++) {
		A[0][j] = 0;
	}
	for (int i = 0; i < N; i++) A[i][i] += 1;

	for (int i = 0; i < N; i++) {
		A[i].push_back(num(1));
	}
	A[0][N] = 0;

	for (int i = 0; i < N; i++) {
		{
			int j = i;
			while (A[j][i] == num(0)) {
				j++;
				assert(j < N);
			}
			swap(A[i], A[j]);
		}
		assert(A[i][i] != 0);
		num v = inv(A[i][i]);
		for (int j = 0; j <= N; j++) A[i][j] *= v;
		assert(A[i][i] == 1);
		for (int j = 0; j < i; j++) assert(A[i][j] == 0);

		for (int j = 0; j < N; j++) {
			if (j == i) continue;
			num fac = A[j][i];
			for (int k = 0; k <= N; k++) {
				A[j][k] -= A[i][k] * fac;
			}
			for (int k = 0; k <= i; k++) {
				assert(A[j][k] == (j == k ? 1 : 0));
			}
		}
	}

	num ans = 0;
	vector<num> kProbs = getProbs(M-K-1);
	for (int j = 0; j < N; j++) {
		for (int k = N-j; k <= N; k++) {
			ans += tr[k-1] * kProbs[j] * A[j+k-N][N];
		}
	}

	ans = ans * num(M) + num(M-K);
	num evDist = 0;
	for (int i = 0; i < N; i++) {
		evDist += num(i+1) * tr[i];
	}

	ans /= evDist;
	cout << ans << '\n';

	return 0;
}
