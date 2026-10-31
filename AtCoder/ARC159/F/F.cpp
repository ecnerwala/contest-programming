#include <bits/stdc++.h>

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

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<int> A(2*N);
	for (auto& a : A) { cin >> a; a--; }
	const int V = 2*N;

	using num = modnum<998244353>;

	std::vector<std::vector<int>> end_values(N+1);

	{
		std::vector<int> cur_excess(V);
		std::vector<int> nonzeros; nonzeros.reserve(60);

		end_values[0] = nonzeros;
		for (int i = 0; i < N; i++) {
			for (int v : {A[2*i], A[2*i+1]}) {
				if (cur_excess[v] == 0) nonzeros.push_back(v);
				cur_excess[v]++;
			}
			// subtract one from everything in nonzeros
			{
				int L = 0;
				for (int z = 0; z < int(nonzeros.size()); z++) {
					int v = nonzeros[z];
					assert(cur_excess[v] > 0);
					cur_excess[v]--;
					if (cur_excess[v] > 0) {
						nonzeros[L++] = v;
					}
				}
				nonzeros.resize(L);
			}
			end_values[i+1] = nonzeros;
		}
	}

	num tot_dp = 0;
	std::vector<std::map<int, num>> by_value(V);
	std::vector<int> cur_cnt(V);
	
	std::vector<int> cur_excess(V);
	std::vector<int> nonzeros; nonzeros.reserve(60);

	for (int i = N; i >= 0; i--) {
		num cur_dp;
		if (i == N) {
			cur_dp = 1;
		} else {
			for (int v : {A[2*i+1], A[2*i]}) {
				if (cur_excess[v] == 0) nonzeros.push_back(v);
				cur_excess[v]++;
				// move from cur_cnt[v] + (i+1) to cur_cnt[v]+1 + (i+1)
				auto it = by_value[v].find(cur_cnt[v] + (i+1));
				if (it != by_value[v].end()) tot_dp -= it->second;
				cur_cnt[v]++;
			}
			{
				int L = 0;
				for (int z = 0; z < int(nonzeros.size()); z++) {
					int v = nonzeros[z];

					// move from cur_cnt[v] + (i+1) to cur_cnt[v] + i
					auto it = by_value[v].find(cur_cnt[v] + i);
					if (it != by_value[v].end()) tot_dp += it->second;

					assert(cur_excess[v] > 0);
					cur_excess[v]--;
					if (cur_excess[v] > 0) {
						nonzeros[L++] = v;
					}
				}
				nonzeros.resize(L);
			}
			cur_dp = tot_dp;
		}

		//cerr << i << ' ' << cur_dp << '\n';

		if (i == 0) {
			cout << cur_dp << '\n';
			break;
		} else {
			// update the thing
			tot_dp += cur_dp;
			for (int v : end_values[i]) {
				by_value[v][cur_cnt[v] + i] += cur_dp;
			}
		}
	}

	return 0;
}
