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

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		cerr << "Case #" << case_num << ": " << '\n';
		string S; cin >> S;
		int N = int(S.size());
		cerr << "N = " << N << '\n';

		using num = modnum<int(1e9)+7>;

		num ans = 0;
		vector<int> locs; locs.reserve(N+2); locs.push_back(-1);
		for (int i = 0; i < N; i++) {
			if (S[i] == '*') {
			} else if (S[i] == '0') {
				// add 1 for everything including this
				ans += num(i+1) * num(N-i);

				// subtract 1 for everything starting here
				ans -= num(N-i);

				locs.push_back(i);
			} else assert(false);
		}
		locs.push_back(N);

		// We have our conservative answer, now we need to finish patching it
		cerr << "conservative ans " << ans << '\n';
		
		// First, all-*
		for (int i = 0; i+1 < int(locs.size()); i++) {
			int a = locs[i], b = locs[i+1];
			int len = b - a - 1;
			// all of these defaulted to 0, so we need to fix those up
			for (int h = 0; ; h++) {
				// if you have 2 things, 4 things, ...
				int bad_sz = (1 << (h+1));
				if (bad_sz > len) break;
				// everything that's size >= bad_sz gets +1
				// there are len - bad_sz + 1 + ... + 0
				ans += num(len - bad_sz + 1) * num(len - bad_sz + 2) / 2;
			}
		}

		// If we have at least 21 0's, then we can always cram them in
		for (int i = 1; i+1 < int(locs.size()); i++) {
			for (int j = i; j+1 < int(locs.size()) && j - i + 1 < 25; j++) {
				int slo = locs[i] - locs[i-1] - 1;
				int smid = locs[j] - locs[i] - (j - i);
				int shi = locs[j+1] - locs[j] - 1;

				int n0 = j - i + 1;
				assert(n0 >= 1);

				// first, if we start at this 0, then we have (1 << h) - 1 leeway
				for (int h = n0 - 1; ; h++) {
					int bad_sz = 1 << h;
					if (bad_sz > smid + shi) break;
					ans += num((shi + smid) - max(smid, bad_sz) + 1);
				}

				// if we don't start at this 0, we have a few edge cases if the original is small
				for (int h = n0; h < 25; h++) {
					// find the number of things that don't fit into height h
					for (int lo = 1; lo <= slo; lo++) {
						int pref = min(lo, h - n0 + 1);

						// first, in these cases, our prefix is limited
						if (lo > (1 << (h+1)) - (1 << (h+1 - pref))) {
							// all bad
							ans += num(shi+1);
							continue;
						}

						int bad_sz = (1 << (h+1)) - (1 << (h+1 - pref)) + (1 << (h - pref));
						if (bad_sz > lo + smid + shi) {
							continue;
						}
						ans += num((lo + smid + shi) - max(lo + smid, bad_sz) + 1);
					}
				}
			}
		}

		cerr << "final ans " << ans << '\n';

		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}

// 0's form a chain down the LHS
// If the number of 0's is big, then we're already screwed and we just take that many
