#include <bits/stdc++.h>

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

template <typename T> T pow(T a, long long b) {
	assert(b >= 0);
	T r = 1; while (b) { if (b & 1) r *= a; b >>= 1; a *= a; } return r;
}

template <typename U, typename V> struct pairnum {
	U u;
	V v;

	pairnum() : u(0), v(0) {}
	pairnum(long long val) : u(val), v(val) {}
	pairnum(const U& u_, const V& v_) : u(u_), v(v_) {}

	friend std::ostream& operator << (std::ostream& out, const pairnum& n) { return out << '(' << n.u << ',' << ' ' << n.v << ')'; }
	friend std::istream& operator >> (std::istream& in, pairnum& n) { long long val; in >> val; n = pairnum(val); return in; }

	friend bool operator == (const pairnum& a, const pairnum& b) { return a.u == b.u && a.v == b.v; }
	friend bool operator != (const pairnum& a, const pairnum& b) { return a.u != b.u || a.v != b.v; }

	pairnum inv() const {
		return pairnum(u.inv(), v.inv());
	}
	pairnum neg() const {
		return pairnum(u.neg(), v.neg());
	}
	pairnum operator- () const {
		return pairnum(-u, -v);
	}
	pairnum operator+ () const {
		return pairnum(+u, +v);
	}

	pairnum& operator ++ () {
		++u, ++v;
		return *this;
	}
	pairnum& operator -- () {
		--u, --v;
		return *this;
	}

	pairnum& operator += (const pairnum& o) {
		u += o.u;
		v += o.v;
		return *this;
	}
	pairnum& operator -= (const pairnum& o) {
		u -= o.u;
		v -= o.v;
		return *this;
	}
	pairnum& operator *= (const pairnum& o) {
		u *= o.u;
		v *= o.v;
		return *this;
	}
	pairnum& operator /= (const pairnum& o) {
		u /= o.u;
		v /= o.v;
		return *this;
	}

	friend pairnum operator ++ (pairnum& a, int) { pairnum r = a; ++a; return r; }
	friend pairnum operator -- (pairnum& a, int) { pairnum r = a; --a; return r; }
	friend pairnum operator + (const pairnum& a, const pairnum& b) { return pairnum(a) += b; }
	friend pairnum operator - (const pairnum& a, const pairnum& b) { return pairnum(a) -= b; }
	friend pairnum operator * (const pairnum& a, const pairnum& b) { return pairnum(a) *= b; }
	friend pairnum operator / (const pairnum& a, const pairnum& b) { return pairnum(a) /= b; }
};

using mnum = modnum<998244353>;
using hsh = pairnum<mnum, mnum>;
const hsh BASE(48, 12903857);

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	string S; cin >> S;

	vector<hsh> base_pow(N+1);
	base_pow[0] = 1;
	for (int i = 0; i < N; i++) base_pow[i+1] = base_pow[i] * BASE;

	vector<int> val(N);
	vector<hsh> pref(N+1);
	pref[0] = 0;
	vector<vector<pair<int, hsh>>> corrections(N+1);

	array<int, 26> last_ind; last_ind.fill(-1);
	for (int i = 0; i < N; i++) {
		int c = S[i] - 'a';
		val[i] = i - last_ind[c];
		for (int j = last_ind[c]+1; j <= i; j++) {
			// this is corrected if len >= i-j+1
			corrections[j].push_back({i-j+1, hsh(val[i]) * base_pow[i]});
		}
		last_ind[c] = i;
		pref[i+1] = pref[i] + hsh(val[i]) * base_pow[i];
	}

	for (int l = 0; l <= N; l++) {
		sort(corrections[l].begin(), corrections[l].end(), [](const auto& a, const auto& b) { return a.first < b.first; });
	}

	vector<pair<int, hsh>> cur_corrections; cur_corrections.reserve(60);
	auto get_lcp = [&](int l1, int l2) -> int {
		assert(l1 != l2);
		if (l1 > l2) swap(l1, l2);
		int max_len = N - l2;
		hsh l1fac = base_pow[l2-l1];

		{
			cur_corrections.clear();
			cur_corrections.push_back({0, -pref[l2] + pref[l1] * l1fac});
			{
				auto l1it = corrections[l1].begin();
				auto l2it = corrections[l2].begin();
				while (l1it != corrections[l1].end() || l2it != corrections[l2].end())  {
					if (l1it != corrections[l1].end() && (l2it == corrections[l2].end() || l1it->first <= l2it->first)) {
						cur_corrections.push_back({l1it->first, l1it->second * l1fac});
						++l1it;
					} else {
						cur_corrections.push_back({l2it->first, -l2it->second});
						++l2it;
					}
				}
			}
			while (!cur_corrections.empty() && cur_corrections.back().first > max_len) cur_corrections.pop_back();
			cur_corrections.push_back({max_len+1, hsh(0)});
		}

		//cerr << "old corrections" << '\n';
		//for (auto& it : cur_corrections) { cerr << it.first << ' ' << it.second << '\n'; }

		{
			// unique this thing
			int i = 0;
			for (int j = 0; j < int(cur_corrections.size()); j++) {
				if (i > 0 && cur_corrections[j].first == cur_corrections[i-1].first) {
					cur_corrections[i-1].second += cur_corrections[j].second;
				} else {
					cur_corrections[i++] = cur_corrections[j];
				}
			}
			cur_corrections.resize(i);
		}

		//cerr << "compressed corrections" << '\n';
		//for (auto& it : cur_corrections) { cerr << it.first << ' ' << it.second << '\n'; }

		//cerr << "max len " << max_len << '\n';

		hsh corr = 0;
		for (int i = 0; i+1 < int(cur_corrections.size()); i++) {
			int lo = cur_corrections[i].first-1;
			int hi = cur_corrections[i+1].first-1;
			corr += cur_corrections[i].second;

			//cerr << "trying " << lo << ' ' << hi << ' ' << corr << '\n';
			if (hi <= max_len && pref[l2+hi] - pref[l1+hi] * l1fac + corr == hsh(0)) continue;
			while (hi - lo > 1) {
				int md = (lo + hi) / 2;
				if (pref[l2+md] - pref[l1+md] * l1fac + corr == hsh(0)) {
					lo = md;
				} else {
					hi = md;
				}
			}
			//cerr << "returning " << lo << '\n';
			return lo;
		}
		return max_len;
	};

	auto suff_cmp = [&](int l1, int l2) -> bool {
		if (l1 == l2) return false;
		assert(l1 != l2);

		int len = get_lcp(l1, l2);
		int r1 = l1 + len, r2 = l2 + len;
		int v1 = (r1 == N ? -1 : (val[r1] > len ? 0 : val[r1]));
		int v2 = (r2 == N ? -1 : (val[r2] > len ? 0 : val[r2]));
		return v1 < v2;
	};

	std::vector<int> sa(N); iota(sa.begin(), sa.end(), 0);
	sort(sa.begin(), sa.end(), suff_cmp);
	int64_t ans = 0;
	for (int i = 0; i < N; i++) {
		int l = i ? get_lcp(sa[i-1], sa[i]) : 0;
		//cerr << S.substr(sa[i]) << ' ' << l << '\n';
		ans += N - sa[i] - l;
	}
	cout << ans << '\n';

	return 0;
}
