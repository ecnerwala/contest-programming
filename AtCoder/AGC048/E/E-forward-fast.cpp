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
	int N, K; cin >> N >> K;
	int T; cin >> T;

	vector<vector<int>> B(N, vector<int>(K));
	for (auto& b : B) {
		for (auto& v : b) cin >> v;
		sort(b.begin(), b.end());
	}

	using num = modnum<int(1e9)+7>;

	num num_seq = 1;
	vector<pair<int, num>> cuts;
	for (int i = 0; i < N; i++, num_seq *= num(K)) {
		vector<pair<int, num>> c0; c0.reserve(cuts.size());
		vector<pair<int, num>> cT; cT.reserve(cuts.size());
		vector<pair<int, num>> c_cur(K);

		num ans = 0;

		for (int j = 0; j < K; j++) {
			c_cur[j] = {B[i][j]-T, num_seq};
		}

		int j0 = 0;
		int jT = 0;
		num pref = 0;

		for (auto it : cuts) {
			while (j0 < K && B[i][j0] <= it.first) {
				c_cur[j0].second += pref;
				ans += pref;
				j0++;
			}
			while (jT < K && B[i][jT]-T <= it.first) {
				c_cur[jT].second -= pref;
				jT++;
			}

			cT.push_back({it.first-T, it.second * num(j0)});
			c0.push_back({it.first, it.second * num(K - jT)});

			pref += it.second;
		}
		while (j0 < K) {
			c_cur[j0].second += pref;
			ans += pref;
			j0++;
		}
		while (jT < K) {
			c_cur[jT].second -= pref;
			jT++;
		}

		for (int a = i+1; a < N; a++) ans *= num(K);
		cout << ans << '\n';

		// merge the ranges
		{
			auto cmp = [](const auto& a, const auto& b) { return a.first < b.first; };
			vector<pair<int, num>> tmp(c0.size() + cT.size());
			std::merge(c0.begin(), c0.end(), cT.begin(), cT.end(), tmp.begin(), cmp);
			cuts.resize(tmp.size() + c_cur.size());
			std::merge(tmp.begin(), tmp.end(), c_cur.begin(), c_cur.end(), cuts.begin(), cmp);
		}

		// squash the ranges
		{
			int L = 0;
			for (int z = 0; z < int(cuts.size()); z++) {
				if (L && cuts[z].first == cuts[L-1].first) {
					cuts[L-1].second += cuts[z].second;
				} else {
					cuts[L++] = cuts[z];
				}
			}
			cuts.resize(L);
		}
	}

	return 0;
}
