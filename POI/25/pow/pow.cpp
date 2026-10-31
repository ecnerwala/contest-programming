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

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M, H; cin >> N >> M >> H; H++;
	const int V = N * M;
	vector<pair<int, array<int, 2>>> edges; edges.reserve(N * (M-1) + (N-1) * M);
	auto vert = [&](int i, int j) { return i * M + j; };
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M-1; j++) {
			int h; cin >> h; h++;
			edges.push_back({h, {vert(i, j), vert(i, j+1) }});
		}
	}
	for (int i = 0; i < N-1; i++) {
		for (int j = 0; j < M; j++) {
			int h; cin >> h; h++;
			edges.push_back({h, {vert(i, j), vert(i+1, j) }});
		}
	}
	sort(edges.begin(), edges.end());

	using num = modnum<int(1e9)+7>;

	vector<int> par(V, -1);
	auto get_par = [&](int a) -> int {
		while (par[a] >= 0) {
			if (par[par[a]] >= 0) par[a] = par[par[a]];
			a = par[a];
		}
		return a;
	};

	vector<pair<num, int>> val(V, {num(0), 0});
	for (auto [h, v] : edges)  {
		auto [a, b] = v;
		a = get_par(a);
		b = get_par(b);
		if (a == b) continue;
		if (par[a] > par[b]) swap(a, b);

		val[a].first += h - val[a].second;
		val[a].second = h;

		val[b].first += h - val[b].second;
		val[b].second = h;

		val[a].first *= val[b].first;

		par[a] += par[b];
		par[b] = a;
	}

	int ROOT = get_par(1);
	val[ROOT].first += H - val[ROOT].second;
	val[ROOT].second = H;

	cout << val[ROOT].first << '\n';

	return 0;
}
