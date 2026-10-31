#include <bits/stdc++.h>

const int INF = int(1e9) + 100;
struct sat_num {
	int v = 0;
	sat_num() {}
	sat_num(long long a) : v(int(std::min<long long>(a, INF))) {}
	explicit operator int() const { return v; }
	explicit operator int64_t() const { return v; }
	friend bool operator < (sat_num a, sat_num b) { return a.v < b.v; }
	friend bool operator <= (sat_num a, sat_num b) { return a.v <= b.v; }
	friend bool operator >= (sat_num a, sat_num b) { return a.v >= b.v; }
	friend bool operator > (sat_num a, sat_num b) { return a.v > b.v; }

	sat_num& operator += (sat_num o) { v += o.v; if (v > INF) v = INF; return *this; }
	//sat_num& operator *= (sat_num o) { if (v > 0 && o.v > INF / v) v = INF; else v *= o.v; return *this; }
	friend sat_num operator + (sat_num a, sat_num b) { return sat_num(a) += b; }
	//friend sat_num operator * (sat_num a, sat_num b) { return sat_num(a) *= b; }
};

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

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	int S; cin >> S;
	int Q; cin >> Q;
	using num = pairnum<sat_num, uint32_t>;
	// choose[a][b] = (a + b choose a)
	vector<vector<num>> dp(S+1, vector<num>(N+S+1));
	for (int i = 0; i <= N+S; i++) {
		dp[0][i] = 1;
	}
	for (int s = 1; s <= S; s++) {
		num pref = 0;
		for (int i = 0; i <= N+S; i++) {
			pref += dp[s-1][i];
			dp[s][i] = pref;
		}
	}

	auto get_choose = [&](int n, int r) -> uint32_t {
		assert(n <= 0);
		assert(r >= 0);
		if (n == 0) {
			return r == 0 ? 1 : 0;
		} else {
			return dp[r][-n-1].v * uint32_t(r & 1 ? -1 : 1);
		}
	};

	vector<int> P(N); for (auto& p : P) { cin >> p; p--; }
	vector<int> invP(N); for (int i = 0; i < N; i++) { invP[P[i]] = i; }

	struct query_t {
		int rt; int k; int a; int off; int idx; uint32_t resp;
	};
	vector<query_t> queries(Q);

	for (auto& q : queries) {
		q.rt = N-1;
		cin >> q.k >> q.a; q.k--;
	}
	for (int q = 0; q < Q; q++) queries[q].idx = q;

	std::vector<std::pair<sat_num, std::array<uint32_t, 7>>> bit(N);
	for (int s = S-1; s >= 0; s--) {
		int cur_val = 0;
		auto clear_bit = [&]() {
			for (auto& v : bit) v = {0, array<uint32_t, 7>{0,0,0,0,0,0,0}};
			cur_val = 0;
		};
		auto bump_cur = [&](int lim) {
			assert(cur_val <= lim);
			for (; cur_val < lim; cur_val++) {
				int p = invP[cur_val];
				for (int a = p; a < N; a |= a+1) {
					auto& b = bit[a];
					b.first += dp[s][cur_val].u;
					for (int j = 0; j <= s-1; j++) {
						b.second[j] += dp[j][cur_val + (s-1-j)].v;
					}
				}
			}
		};

		std::sort(queries.begin(), queries.end(), [&](const auto& a, const auto& b) { return a.rt < b.rt; });
		clear_bit();
		for (auto& q : queries) {
			if (q.rt < q.k) continue;
			assert(0 <= q.a && q.a <= dp[s+1][q.rt].u);
			bump_cur(q.rt+1);
			pair<sat_num, std::array<uint32_t, 7>> cur{0, {0,0,0,0,0,0,0}};
			int a = 0;
			for (int bt = (1 << 18); bt > 0; bt >>= 1) {
				if (a + bt <= N && cur.first + bit[a+bt-1].first <= q.a) {
					a += bt;
					auto& b = bit[a-1];
					cur.first += b.first;
					for (int j = 0; j <= s-1; j++) {
						cur.second[j] += b.second[j];
					}
				}
			}
			q.a -= int(cur.first);
			q.off = a;
			assert(q.off == N || P[q.off] <= q.rt);
			if (q.off == N) assert(q.a == 0);
			else assert(0 <= q.a && q.a < dp[s][P[q.off]].u);
			if (s == 0) {
				assert(q.a == 0);
				q.resp += q.k <= q.rt && invP[q.k] < q.off;
			}
			for (int i = 0; i <= s-1; i++) {
				q.resp += get_choose(-q.k, i) * cur.second[s-1-i];
			}
		}

		if (s == 0) break;
		std::sort(queries.begin(), queries.end(), [&](const auto& a, const auto& b) { return a.k < b.k; });
		clear_bit();
		for (auto& q : queries) {
			if (q.rt < q.k) continue;
			bump_cur(q.k);
			pair<sat_num, std::array<uint32_t, 7>> cur{0, {0,0,0,0,0,0,0}};
			for (int a = q.off; a > 0; a &= a-1) {
				auto& b = bit[a-1];
				for (int j = 0; j <= s-1; j++) {
					cur.second[j] += b.second[j];
				}
			}
			for (int i = 0; i <= s-1; i++) {
				q.resp -= get_choose(-q.k, i) * cur.second[s-1-i];
			}
		}
		for (auto& q : queries) {
			if (q.rt < q.k) continue;
			assert(q.off == N || P[q.off] <= q.rt);
			q.rt = q.off == N ? -1 : P[q.off];
		}
	}

	std::sort(queries.begin(), queries.end(), [&](const auto& a, const auto& b) { return a.idx < b.idx; });
	for (auto q : queries) {
		cout << q.resp << '\n';
	}

	return 0;
}
