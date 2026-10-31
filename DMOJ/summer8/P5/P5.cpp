#include<bits/stdc++.h>

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

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	using num = modnum<998244353>;

	const num I = pow(num(3), (num::MOD - 1) / 4);
	assert(pow(I, 4) == 1);
	assert(pow(I, 2) != 1);

	const array<num, 4> roots{1, I, -1, -I};

	int N, M, K; cin >> N >> M >> K;
	vector<vector<int8_t>> strings(N);
	for (int i = 0; i < N; i++) {
		string S; cin >> S;
		strings[i].resize(M);
		for (int j = 0; j < M; j++) {
			char c = S[j];
			switch (c) {
				case 'A': strings[i][j] = 0; break;
				case 'C': strings[i][j] = 1; break;
				case 'G': strings[i][j] = 2; break;
				case 'T': strings[i][j] = 3; break;
				default: assert(false);
			}
		}
	}

	mt19937 mt(48);
	vector<num> coeffs(N);
	for (auto& a : coeffs) a = num(mt());

	vector<array<num, 4>> coeffs_mat(M);
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			for (int z = 0; z < 4; z++) {
				coeffs_mat[j][z] += roots[((-z * strings[i][j]) & 3)] * coeffs[i];
			}
		}
	}

	num tot_coeffs = accumulate(coeffs.begin(), coeffs.end(), num(0));

	for (int i = 0; i < N; i++) {
		num test_val = 0;
		for (int j = 0; j < M; j++) {
			for (int z = 0; z < 4; z++) {
				test_val += coeffs_mat[j][z] * roots[(z * strings[i][j]) & 3];
			}
		}
		if (test_val == (tot_coeffs * num(M-K) + coeffs[i] * num(K)) * 4) {
			cout << i+1 << '\n';
			exit(0);
		}
	}

	assert(false);

	return 0;
}
