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

template <typename T> T pow(T a, long long b) {
	assert(b >= 0);
	T r = 1; while (b) { if (b & 1) r *= a; b >>= 1; a *= a; } return r;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int N, M; cin >> N >> M;
	array<vector<vector<int>>, 2> adj;
	adj[0].resize(N);
	adj[1].resize(N);
	for (int e = 0; e < M; e++) {
		int u, v; cin >> u >> v; u--, v--;
		adj[0][u].push_back(v);
		adj[1][v].push_back(u);
	}

	const int L = 18;
	vector<array<int, L>> dist(N);
	for (int i = 0; i < N; i++) {
		dist[i].fill(-1);
	}
	pair<int, int> BAD(-1, -1);
	vector<array<pair<int, int>, 2>> distBig(N, array<pair<int, int>, 2>{BAD, BAD});

	auto canonicalize = [&](pair<int, int> a) -> pair<int, int> {
		assert(a.second < (1 << L));
		if (a.first >= L) {
			return a;
		}
		int v = (1 << a.first) + a.second;
		int l = 31 - __builtin_clz(v);
		assert(v >= (1 << l));
		assert(v < (2 << l));
		return {l, v - (1 << l)};
	};

	using pq_t = pair<pair<int, int>, pair<int, int>>;
	priority_queue<pq_t, vector<pq_t>, greater<pq_t>> pq;
	dist[0][0] = 0;
	pq.push({canonicalize({0, dist[0][0]}), {0, 0}});

	while (!pq.empty()) {
		auto [d, p] = pq.top(); pq.pop();
		int i = p.first;
		int j = p.second;
		if (j < L) {
			if (d != canonicalize({j, dist[i][j]})) continue;
			d = {j, dist[i][j]};
		} else {
			if (d != distBig[i][d.first & 1]) continue;
			assert(d.first == j);
		}

		assert(d.first == j);
		//cerr << i << ' ' << j << ' ' << d.second << '\n';

		if (i == N-1) {
			using num = modnum<998244353>;
			cout << pow(num(2), d.first)-1 + d.second << '\n';
			exit(0);
		}

		assert(d.first == j);

		// flip
		{
			int ni = i;
			int nj = j+1;
			auto nd = d;
			nd.first++;
			if (nj < L) {
				if (dist[ni][nj] == -1 || nd.second < dist[ni][nj]) {
					dist[ni][nj] = nd.second;
					pq.push({canonicalize(nd), {ni, nj}});
				}
			} else {
				assert(canonicalize(nd) == nd);
				if (distBig[ni][nd.first & 1] == BAD || nd < distBig[ni][nd.first & 1]) {
					distBig[ni][nd.first & 1] = nd;
					pq.push({nd, {ni, nj}});
				}
			}
		}

		for (int ni : adj[d.first & 1][i]) {
			int nj = j;
			auto nd = d;
			nd.second++;
			if (nj < L) {
				if (dist[ni][nj] == -1 || nd.second < dist[ni][nj]) {
					dist[ni][nj] = nd.second;
					pq.push({canonicalize(nd), {ni, nj}});
				}
			} else {
				assert(canonicalize(nd) == nd);
				if (distBig[ni][nd.first & 1] == BAD || nd < distBig[ni][nd.first & 1]) {
					distBig[ni][nd.first & 1] = nd;
					pq.push({nd, {ni, nj}});
				}
			}
		}
	}

	return 0;
}
