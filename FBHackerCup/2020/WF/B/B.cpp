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

template <typename T> void setmax(T& a, T b) { if (b > a) a = b; }
template <typename T> void setmin(T& a, T b) { if (b < a) a = b; }

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		int N; cin >> N;
		vector<int> P(N);
		P[0] = -1;
		vector<vector<int>> ch(N);

		for (int i = 1; i < N; i++) {
			cin >> P[i]; P[i]--;
			assert(0 <= P[i] && P[i] < i);
			ch[P[i]].push_back(i);
		}

		vector<int> depth(N);
		depth[0] = 1;
		for (int i = 1; i < N; i++) {
			depth[i] = depth[P[i]] + 1;
		}

		// deepest vertex in the subtree
		vector<int> deepest = depth;
		for (int i = N-1; i > 0; i--) {
			setmax(deepest[P[i]], deepest[i]);
		}

		// deepest vertex which doesn't touch this subtree
		vector<int> ceo_path(N);
		ceo_path[0] = 0;
		for (int i = 0; i < N; i++) {
			int m1 = max(ceo_path[i], depth[i]);
			int m2 = m1;
			for (int j : ch[i]) {
				int v = deepest[j];
				if (v >= m1) { m2 = m1; m1 = v; }
				else if (v >= m2) { m2 = v; }
			}
			for (int j : ch[i]) {
				int v = deepest[j];
				ceo_path[j] = (v == m1 ? m2 : m1);
			}
		}

		vector<int> up_path(N);
		up_path[0] = 0;
		for (int i = 0; i < N; i++) {
			int m1 = max(up_path[i], ceo_path[i] + 1);
			int m2 = m1;
			for (int j : ch[i]) {
				int v = ceo_path[i] + deepest[j] - depth[i] + 1;
				if (v >= m1) { m2 = m1; m1 = v; }
				else if (v >= m2) { m2 = v; }
			}
			for (int j : ch[i]) {
				int v = ceo_path[i] + deepest[j] - depth[i] + 1;
				up_path[j] = (v == m1 ? m2 : m1) + 1;
			}
		}

		vector<int> R(N);
		for (int i = 0; i < N; i++) {
			R[i] = deepest[i] - depth[i] + 1 + ceo_path[i];
			if (i > 0) setmax(R[i], up_path[i]);

			R[i]--;
		}

		//for (int i = 0; i < N; i++) { cerr << R[i] << ' '; } cerr << '\n';

		using num = modnum<int(1e9)+7>;
		num ans = 1;
		for (int i = 0; i < N; i++) {
			ans *= num(R[i]);
		}

		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}
