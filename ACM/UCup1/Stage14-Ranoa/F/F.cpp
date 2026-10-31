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

template <typename T> struct extended_gcd_result {
	T gcd;
	T coeff_a, coeff_b;
};
template <typename T> extended_gcd_result<T> extended_gcd(T a, T b) {
	T x = a, y = b;
	// coeff of a and b in x and y
	T ax = 1, ay = 0;
	T bx = 0, by = 1;
	while (x) {
		T k = y / x;
		y %= x;
		ay -= k * ax;
		by -= k * bx;
		std::swap(x, y);
		std::swap(ax, ay);
		std::swap(bx, by);
	}
	return {y, ay, by};
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

	int N, M; cin >> N >> M;
	std::vector<std::array<int, 2>> edges(M);
	std::vector<std::vector<std::pair<int, int>>> adj_bigger(N);
	std::vector<std::vector<std::pair<int, int>>> adj_smaller(N);
	std::vector<std::vector<std::pair<int, int>>> adj_both(N);
	{
		std::vector<std::pair<int, int>> deg(N);
		for (auto& [u, v] : edges) {
			cin >> u >> v;
			deg[u].first++;
			deg[v].first++;
		}
		for (int i = 0; i < N; i++) {
			deg[i].second = i;
		}
		std::sort(deg.begin(), deg.end());

		std::vector<int> new_idx(N);
		for (int i = 0; i < N; i++) {
			new_idx[deg[i].second] = i;
		}
		for (int e = 0; e < M; e++) {
			auto& [u, v] = edges[e];
			u = new_idx[u], v = new_idx[v];
			if (u > v) std::swap(u, v);
			adj_bigger[u].push_back({v, e});
			adj_smaller[v].push_back({u, e});
			adj_both[u].push_back({v, e});
			adj_both[v].push_back({u, e});
		}
	}

	std::vector<int> buf(N, -1);

	using num = modnum<998244353>;
	std::vector<num> num_triangles(N);
	std::vector<num> edge_triangles(M);

	for (int i = 0; i < N; i++) {
		for (auto [j, e_ij] : adj_bigger[i]) {
			buf[j] = e_ij;
		}
		for (auto [j, e_ij] : adj_bigger[i]) {
			for (auto [k, e_jk] : adj_bigger[j]) {
				if (buf[k] != -1) {
					int e_ik = buf[k];
					// triangle i, j, k
					num_triangles[i]++;
					num_triangles[j]++;
					num_triangles[k]++;

					edge_triangles[e_ij]++;
					edge_triangles[e_jk]++;
					edge_triangles[e_ik]++;
				}
			}
		}
		for (auto [j, e_ij] : adj_bigger[i]) {
			buf[j] = -1;
		}
	}

	for (int i = 0; i < N; i++) {
		buf[i] = 0;
	}

	std::vector<num> edge_squares(M);
	for (int i = 0; i < N; i++) {
		for (auto [j, e_ij] : adj_both[i]) {
			for (auto [k, e_jk] : adj_bigger[j]) {
				if (k > i) {
					buf[k] ++;
				}
			}
		}

		for (auto [j, e_ij] : adj_both[i]) {
			for (auto [k, e_jk] : adj_bigger[j]) {
				if (k > i) {
					edge_squares[e_ij] += (buf[k] - 1);
					edge_squares[e_jk] += (buf[k] - 1);
				}
			}
		}
		for (auto [j, e_ij] : adj_both[i]) {
			for (auto [k, e_jk] : adj_bigger[j]) {
				if (k > i) {
					buf[k] --;
				}
			}
		}
	}

	std::vector<num> num_1paths(N);
	for (int i = 0; i < N; i++) {
		num_1paths[i] = int(adj_both[i].size());
	}
	std::vector<num> num_2paths(N);
	for (int i = 0; i < N; i++) {
		for (auto [j, _] : adj_both[i]) {
			// -1 cause of the return path
			num_2paths[i] += num_1paths[j] - 1;
		}
	}
	std::vector<num> num_3paths(N);
	for (int i = 0; i < N; i++) {
		for (auto [j, _] : adj_both[i]) {
			// don't come back to ourself
			num_3paths[i] += num_2paths[j] - (num_1paths[i] - 1);
		}
		// triangles get counted twice
		num_3paths[i] -= num_triangles[i] * 2;
	}

	num tri = 0;
	for (int i = 0; i < N; i++) {
		tri += num_triangles[i];
	}
	tri /= 3;
	cerr << "tri " << tri << '\n';

	num tri2 = 0;
	num tri2_1path = 0;
	for (int e = 0; e < M; e++) {
		num w = edge_triangles[e];
		auto [u, v] = edges[e];
		tri2 += w * (w-1) / num(2);
		tri2_1path += w * (w-1) / num(2) * (num_1paths[u] - 3 + num_1paths[v] - 3);
	}
	cerr << "tri2 " << tri2 << '\n';
	cerr << "tri2_1path " << tri2_1path << '\n';

	num tri_1path = 0;
	for (int i = 0; i < N; i++) {
		tri_1path += num_triangles[i] * (num_1paths[i] - 2);
	}
	cerr << "tri_1path " << tri_1path << '\n';

	num tri_2path = 0;
	for (int i = 0; i < N; i++) {
		tri_2path += num_triangles[i] * num_2paths[i];
	}
	tri_2path -= 6 * tri;
	tri_2path -= 2 * tri_1path;
	tri_2path -= 4 * tri2;
	cerr << "tri_2path " << tri_2path << '\n';

	num tri_square = 0;
	for (int e = 0; e < M; e++) {
		tri_square += edge_squares[e] * edge_triangles[e];
	}
	tri_square -= 4 * tri2;
	cerr << "tri_square " << tri_square << '\n';

	num tri_3path = 0;
	for (int i = 0; i < N; i++) {
		tri_3path += num_triangles[i] * num_3paths[i];
	}
	cerr << "tri_3path_naive " << tri_3path << '\n';
	tri_3path -= 4 * tri2;
	tri_3path -= 2 * tri_1path;
	tri_3path -= 2 * tri_2path;

	tri_3path -= 2 * tri2_1path;
	tri_3path -= 4 * tri2;

	tri_3path -= 2 * tri_square;

	cerr << "tri_3path" << ' ' << tri_3path << '\n';

	cout << tri_3path << '\n';

	return 0;
}
