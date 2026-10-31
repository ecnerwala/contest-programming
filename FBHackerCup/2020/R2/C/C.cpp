#include<bits/extc++.h>

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

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		int N, M, Q, K; cin >> N >> M >> Q >> K;

		auto read_vec = [K](int len, int64_t mod) -> vector<int64_t> {
			vector<int64_t> res(len);
			for (int i = 0; i < K; i++) {
				cin >> res[i];
			}
			int64_t a, b, c; cin >> a >> b >> c;
			for (int i = K; i < len; i++) {
				res[i] = (a * res[i-2] + b * res[i-1] + c) % mod;
			}
			return res;
		};
		auto X = read_vec(N, M);
		auto Y = read_vec(N, M);

		for (int i = 0; i < N; i++) {
			if (X[i] > Y[i]) swap(X[i], Y[i]);
			assert(X[i] <= Y[i]);
		}

		auto QI = read_vec(Q, N*M+N);
		auto QW = read_vec(Q, int(1e9));

		// MST means removing N+1 edges
		// We remove >= 1 from each small circle, which is >= N total
		// And the last is either a 2nd from a small circle or 1 from the big circle

		int E = N * M + N;
		vector<int64_t> W(E, 1);
		int64_t tot_weight = std::accumulate(W.begin(), W.end(), int64_t(0));

		using pq_t = __gnu_pbds::priority_queue<int64_t>;
		vector<array<pq_t, 2>> circle_weights(N);
		pq_t extra_weights;
		vector<pair<pq_t*, pq_t::point_iterator>> controller(E);
		vector<pq_t::point_iterator> circ_controller(N);

		for (int e = 0; e < E; e++) {
			if (e < N * M) {
				int i = e / M;
				int j = e % M;
				controller[e].first = &circle_weights[i][X[i] <= j && j < Y[i]];
			} else {
				controller[e].first = &extra_weights;
			}
			controller[e].second = controller[e].first->push(W[e]);
		}

		for (int i = 0; i < N; i++) {
			for (int z = 0; z < 2; z++) {
				if (circle_weights[i][z].empty()) {
					assert(z == 1 && X[i] == Y[i]);
					circle_weights[i][z].push(-1);
				}
			}
			tot_weight -= max(circle_weights[i][0].top(), circle_weights[i][1].top());
			circ_controller[i] = extra_weights.push(min(circle_weights[i][0].top(), circle_weights[i][1].top()));
		}

		tot_weight -= extra_weights.top();

		using mnum = modnum<int(1e9)+7>;

		mnum ans = 1;
		for (int q = 0; q < Q; q++) {
			int e = int(QI[q]);
			int64_t w = QW[q];
			tot_weight += extra_weights.top();
			if (e < N * M) {
				int i = e / M;
				tot_weight += max(circle_weights[i][0].top(), circle_weights[i][1].top());
			}
			tot_weight -= W[e];

			W[e] = w;

			tot_weight += W[e];
			controller[e].first->modify(controller[e].second, W[e]);
			if (e < N * M) {
				int i = e / M;
				tot_weight -= max(circle_weights[i][0].top(), circle_weights[i][1].top());
				extra_weights.modify(circ_controller[i], min(circle_weights[i][0].top(), circle_weights[i][1].top()));
			}
			tot_weight -= extra_weights.top();

			ans *= mnum(tot_weight);
		}

		cerr << "Case #" << case_num << ": " << ans << '\n';
		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}
