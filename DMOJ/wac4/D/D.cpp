#include<bits/stdc++.h>
using namespace std;

void wssert(bool b) {
	if (!b) {
		cout << "GARBAGE" << '\n';
		exit(0);
	}
}

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

string incr(string A) {
	for (int i = int(A.size())-1; true; i--) {
		if (i == -1) {
			A.insert(A.begin(), '0');
			i++;
		}
		if (A[i] == '9') {
			A[i] = '0';
			continue;
		} else {
			A[i] ++;
			break;
		}
	}
	return A;
}
string decr(string A) {
	for (int i = int(A.size())-1; true; i--) {
		assert(i >= 0);
		if (A[i] == '0') {
			A[i] = '9';
			continue;
		} else {
			A[i] --;
			break;
		}
	}
	if (A[0] == '0') A.erase(A.begin());
	return A;
}

pair<string, int> solve(const string S) {
	if (S.find_first_not_of("0") == string::npos)  {
		return {"1"+S, 1};
	}
	for (int l = 1; true; l++) {
		std::optional<pair<string, int>> best;

		auto checkString = [&](string s0, int off) {
			//cerr << s0 << ' ' << off << '\n';
			assert(int(s0.size()) == l);
			if(s0[0] == '0') {
				return;
			}

			if (off) {
				string pref = decr(s0);
				if (int(pref.size()) <= off) return;
				if (string(S.begin(), S.begin()+off) != string(pref.end()-off, pref.end())) {
					return;
				}
			}
			string s1 = s0;
			for (int cur = off; cur < int(S.size()); s1 = incr(s1)) {
				for (int i = 0; cur < int(S.size()) && i < int(s1.size()); i++, cur++) {
					if (S[cur] != s1[i]) {
						return;
					}
				}
			}
			if (!best) {
				best = {s0, -off};
			} else {
				best = min(*best, {s0, -off});
			}
		};
		for (int s = 0; s < l; s++) {
			if (s+l <= int(S.size())) {
				checkString(S.substr(s, l), s);
			} else {
				//wssert(l <= int(S.size()));
				string cnd = S.substr(s);
				string suff = incr(S.substr(s-(l-int(cnd.size())), l - int(cnd.size())));
				while (int(suff.size()) > l - int(cnd.size())) {
					suff = suff.substr(1);
				}
				cnd += suff;
				checkString(cnd, s);
			}
		}

		if (!best) continue;
		return *best;
	}
}

int main() {
	ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
	string S; cin >> S;
	auto best = solve(S);
	num ans = 0;
	num val = 0;
	for (char c : best.first) {
		val *= 10;
		val += (c - '0');
	}
	int l = int(best.first.size());
	ans += val * num(l);
	for (int i = 0; i < l; i++) {
		ans -= pow(num(10), i);
	}

	ans += best.second;

	cout << ans+1 << '\n';

	return 0;
}
