#include <bits/stdc++.h>
#include <cassert>

template <typename T> T mod_inv_in_range(T a, T m) {
	// assert(0 <= a && a < m);
	T x = a, y = m;
	// abs coeff of a in x and y (they're always opposite sign)
	T vx = 1, vy = 0;
	bool swap = false;
	while (x) {
		T k = y / x;
		y %= x;
		vy += k * vx;
		std::swap(x, y);
		std::swap(vx, vy);
		swap ^= 1;
	}
	assert(y == 1);
	return swap ? vy : m - vy;
}

template <int MOD_> struct modnum {
	// Uses subtraction to support MOD up to 2^31 - 1
	static constexpr int MOD = MOD_;
	static_assert(MOD_ > 0, "MOD must be positive");
	struct is_reduced_tag {};

	int v;
	modnum() : v(0) {}
	modnum(int v_, is_reduced_tag) : v(v_) { assert(0 <= v && v < MOD); }
	static modnum from_reduced(int v) { return modnum(v, is_reduced_tag{}); }

	modnum(int v_) : v(int(v_ % MOD)) { if (v < 0) v += MOD; }
	modnum(unsigned v_) : v(int(v_ % MOD)) { }
	modnum(int64_t v_) : v(int(v_ % MOD)) { if (v < 0) v += MOD; }
	modnum(uint64_t v_) : v(int(v_ % MOD)) { }

	explicit operator int() const { return v; }
	int as_signed() const { return MOD-v > v ? v : v - MOD; }
	friend std::ostream& operator << (std::ostream& out, modnum n) { return out << int(n); }
	friend std::istream& operator >> (std::istream& in, modnum& n) { int64_t v_; in >> v_; n = modnum(v_); return in; }

	friend bool operator == (modnum a, modnum b) { return a.v == b.v; }
	friend bool operator != (modnum a, modnum b) { return a.v != b.v; }

	modnum inv() const { return from_reduced(mod_inv_in_range(v, MOD)); }
	friend modnum inv(modnum m) { return m.inv(); }
	modnum neg() const { return from_reduced(v ? MOD-v : 0); }
	friend modnum neg(modnum m) { return m.neg(); }

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
	modnum& operator += (modnum o) {
		v -= MOD-o.v;
		v = (v < 0) ? v + MOD : v;
		return *this;
	}
	modnum& operator -= (modnum o) {
		v -= o.v;
		v = (v < 0) ? v + MOD : v;
		return *this;
	}
	modnum& operator *= (modnum o) {
		v = int(int64_t(v) * int64_t(o.v) % MOD);
		return *this;
	}
	modnum& operator /= (modnum o) {
		return *this *= o.inv();
	}

	friend modnum operator ++ (modnum& a, int) { modnum r = a; ++a; return r; }
	friend modnum operator -- (modnum& a, int) { modnum r = a; --a; return r; }
	friend modnum operator + (modnum a, modnum b) { return modnum(a) += b; }
	friend modnum operator - (modnum a, modnum b) { return modnum(a) -= b; }
	friend modnum operator * (modnum a, modnum b) { return modnum(a) *= b; }
	friend modnum operator / (modnum a, modnum b) { return modnum(a) /= b; }
};

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int S; std::cin >> S;
	while (S--) {
		int N; std::cin >> N;
		struct edge_t {
			int64_t w;
			std::array<int, 2> v;
		};
		std::vector<edge_t> edges(N-1);
		for (auto& e : edges) {
			std::cin >> e.v[0] >> e.v[1] >> e.w;
			e.v[0]--, e.v[1]--;
		}
		std::ranges::sort(edges, [&](const auto& a, const auto& b) -> bool { return a.w > b.w; });
		const int64_t MAX_W = edges.back().w * (N-1);
		std::vector<edge_t> cuts(2*(N-1));
		{
			std::vector<int> par(N, -1);
			std::vector<int> label(N); std::ranges::iota(label, 0);
			std::vector<int> sz(2*N-1, 0); std::ranges::fill(sz.begin(), sz.begin() + N, 1);
			auto get_par = [&](int a) -> int {
				while (par[a] >= 0) {
					if (par[par[a]] >= 0) {
						par[a] = par[par[a]];
					}
					a = par[a];
				}
				return a;
			};
			for (int e = 0; e < N-1; e++) {
				int64_t w = edges[e].w;
				auto [a, b] = edges[e].v;
				a = get_par(a), b = get_par(b);
				assert(a != b);
				if (par[a] > par[b]) std::swap(a, b);

				cuts[label[a]] = {w * (-par[b]), {label[a], N+e}};
				cuts[label[b]] = {w * (-par[a]), {label[b], N+e}};

				par[a] += par[b];
				par[b] = a;
				label[a] = N+e;
				sz[N+e] = -par[a];
			}

			// Now, fill in the cut costs all the way
			for (int i = 2*(N-1) - 1; i >= 0; i--) {
				int ng = cuts[i].v[1];
				assert(ng > i);
				cuts[i].w += ng == 2 * (N-1) ? 0 : cuts[ng].w;
			}
			for (int i = 0; i < 2 * (N-1); i++) {
				cuts[i].w = int64_t(std::min<__int128_t>(__int128_t(cuts[i].w) * sz[i], MAX_W));
			}
		}
		assert(int(cuts.size()) == 2*(N-1));
		std::ranges::sort(cuts, [&](const auto& a, const auto& b) -> bool { return a.w > b.w; });
		{
			std::vector<int> par(2*N-1, -1);
			std::vector<int> freq(2*N-1, 0); std::ranges::fill(freq.begin(), freq.begin() + N, 1);
			auto get_par = [&](int a) -> int {
				while (par[a] >= 0) {
					if (par[par[a]] >= 0) {
						par[a] = par[par[a]];
					}
					a = par[a];
				}
				return a;
			};
			using num = modnum<998244353>;
			num ans = 0;
			for (auto [w, v] : cuts) {
				auto [a, b] = v;
				a = get_par(a), b = get_par(b);
				assert(a != b);
				if (par[a] > par[b]) std::swap(a, b);

				ans += num(int64_t(freq[a]) * int64_t(freq[b])) * num(w);
				std::cerr << freq[a] << ' ' << freq[b] << ' ' << w << '\n';

				par[a] += par[b];
				par[b] = a;
				freq[a] += freq[b];
			}
			std::cout << ans << '\n';
		}
	}

	return 0;
}
