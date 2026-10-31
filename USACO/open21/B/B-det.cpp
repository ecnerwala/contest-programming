#include <bits/stdc++.h>

template <int MOD_> struct modnum {
	static const int MOD = MOD_;
private:
	int v;
public:
	modnum() : v(0) {}
	modnum(int64_t v_) : v(int(v_ % MOD)) { if (v < 0) v += MOD; }
	explicit operator int() const { return v; }
	friend std::ostream& operator << (std::ostream& o, modnum m) { return o << int(m); }
	friend std::istream& operator >> (std::istream& i, modnum& m) { int64_t n; i >> n; m = modnum(n); return i; }

	friend bool operator == (modnum a, modnum b) { return a.v == b.v; }
	friend bool operator != (modnum a, modnum b) { return a.v != b.v; }

	modnum operator + () const { return *this; }
	modnum operator - () const { modnum r; r.v = v ? MOD - v : 0; return r; }
	friend modnum neg(modnum a) {
		return -a;
	}
private:
	static int minv(int a, int m) {
		return a == 1 ? 1 : int(m - int64_t(minv(m % a, a)) * m / a);
	}
public:
	friend modnum inv(modnum a) {
		assert(a.v);
		modnum r;
		r.v = minv(a.v, MOD);
		return r;
	}

	modnum& operator ++ () {
		v++;
		if (v == MOD) v = 0;
		return *this;
	}
	modnum& operator -- () {
		if (!v) v = MOD;
		v--;
		return *this;
	}
	modnum operator ++ (int) { modnum r = *this; operator ++ (); return r; }
	modnum operator -- (int) { modnum r = *this; operator -- (); return r; }

	modnum& operator += (modnum o) {
		v -= MOD - o.v;
		v = v < 0 ? v + MOD : v;
		return *this;
	}
	modnum& operator -= (modnum o) {
		v -= o.v;
		v = v < 0 ? v + MOD : v;
		return *this;
	}
	modnum& operator *= (modnum o) {
		v = int(int64_t(v) * int64_t(o.v) % MOD);
		return *this;
	}
	modnum& operator /= (modnum o) {
		return *this *= inv(o);
	}

	friend modnum operator + (modnum a, modnum b) { return a += b; }
	friend modnum operator - (modnum a, modnum b) { return a -= b; }
	friend modnum operator * (modnum a, modnum b) { return a *= b; }
	friend modnum operator / (modnum a, modnum b) { return a /= b; }
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	using num = modnum<int(1e9)+7>;

	int T; cin >> T; while (T--) {
		int N, K; cin >> N >> K;
		string type; cin >> type;

		vector<int> deg(N);
		vector<vector<num>> mat(N, vector<num>(N));
		for (int i = 0; i < N; i++) {
			string r; cin >> r;
			for (int j = 0; j < N; j++) {
				if (r[j] == '1') {
					deg[i]++;
					mat[i][j] = -1;
				}
			}
			deg[i] += type[i] == 'R';
			mat[i][i] += deg[i];
		}

		num ans = 1;
		for (int i = 0; i < N; i++) {
			if (!deg[i]) continue;

			for (int v = 1; v < deg[i]; v++) ans *= v;

			if (mat[i][i] == 0) {
				int j = i+1;
				while (j < N && mat[j][i] == 0) j++;
				if (j == N) {
					ans = 0;
					break;
				}
				swap(mat[i], mat[j]);
				ans = -ans;
			}

			ans *= mat[i][i];
			num iv = inv(mat[i][i]);
			for (int j = i+1; j < N; j++) {
				num v = mat[j][i];
				if (v == 0) continue;
				v *= iv;
				for (int k = i; k < N; k++) {
					mat[j][k] -= v * mat[i][k];
				}
			}
		}

		cout << ans << '\n';
	}

	return 0;
}
