#include <bits/stdc++.h>

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

class PermTree {
public:
	enum class NodeType {
		LEAF,
		INCR,
		DECR,
		FULL,
		PARTIAL,
	};

	struct Node {
		std::array<int, 2> c;
		NodeType type;
		int l, r, lo, hi;
	};

	std::vector<Node> nodes;
	int root = -1;

	PermTree() {}
	Node& operator [] (int idx) { return nodes[idx]; }
	const Node& operator [] (int idx) const { return nodes[idx]; }

	int size() const { return int(nodes.size()); }

	PermTree(const std::vector<int>& A) : nodes(int(A.size())*2-1) {
		int N = int(A.size());
		std::vector<int> nxt_earlier(N);
		std::vector<int> prv_earlier(N);
		for (int i = 0; i < N; i++) {
			nxt_earlier[i] = i+1;
			prv_earlier[i] = i-1;
		}
		for (int i = N-1; i >= 0; i--) {
			int a = A[i];
			int p = prv_earlier[a];
			int n = nxt_earlier[a];
			if (p != -1) nxt_earlier[p] = n;
			if (n != N) prv_earlier[n] = p;
		}

		struct cnd_t {
			int left;
			int lo;
			int lo_gap;
			int hi;
			int hi_gap;
			int node;
		};

		std::vector<cnd_t> stk; stk.reserve(N);

		for (int i = 0; i < N; i++) {
			int a = A[i];
			while (true) {
				if (!stk.empty() && (a < stk.back().lo_gap || a > stk.back().hi_gap)) {
					assert(stk.size() >= 2);
					stk.end()[-2].lo = std::min(stk.end()[-2].lo, stk.back().lo);
					stk.end()[-2].hi = std::max(stk.end()[-2].hi, stk.back().hi);

					int n = 2 * stk.back().left - 1;
					nodes[n].c = {stk.end()[-2].node, stk.end()[-1].node};
					nodes[n].type = NodeType::PARTIAL;
					nodes[n].l = stk.end()[-2].left;
					nodes[n].r = i-1;
					nodes[n].lo = stk.end()[-2].lo;
					nodes[n].hi = stk.end()[-2].hi;

					stk.pop_back();

					stk.back().node = n;
				} else {
					break;
				}
			}

			stk.push_back({i, a, prv_earlier[a]+1, a, nxt_earlier[a]-1, 2*i});
			nodes[2*i].type = NodeType::LEAF;
			nodes[2*i].c = {-1, -1};
			nodes[2*i].l = nodes[2*i].r = i;
			nodes[2*i].lo = nodes[2*i].hi = a;

			while (stk.size() >= 2 && std::max(stk.back().hi, stk.end()[-2].hi) - std::min(stk.back().lo, stk.end()[-2].lo) == i - stk.end()[-2].left) {
				// merge these two nodes into one
				stk.end()[-2].lo = std::min(stk.end()[-2].lo, stk.back().lo);
				stk.end()[-2].hi = std::max(stk.end()[-2].hi, stk.back().hi);

				int n = 2 * stk.back().left - 1;
				nodes[n].c = {stk.end()[-2].node, stk.end()[-1].node};
				if (stk.end()[-2].lo == stk.end()[-1].lo) {
					nodes[n].type = NodeType::DECR;
				} else if (stk.end()[-2].hi == stk.end()[-1].hi) {
					nodes[n].type = NodeType::INCR;
				} else {
					nodes[n].type = NodeType::FULL;
				}
				nodes[n].l = stk.end()[-2].left;
				nodes[n].r = i;
				nodes[n].lo = stk.end()[-2].lo;
				nodes[n].hi = stk.end()[-2].hi;

				stk.pop_back();
				stk.back().node = n;
			}
		}

		assert(stk.size() == 1);
		root = stk.back().node;
	}
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) { cin >> a; a--; }
		std::vector<int> B(N);
		for (auto& b : B) { cin >> b; b--; }

		std::vector<int> invA(N);
		for (int i = 0; i < N; i++) invA[A[i]] = i;
		std::vector<int> P(N);
		for (int i = 0; i < N; i++) P[i] = invA[B[i]];
		std::reverse(P.begin(), P.end());
		// we have 0..N-1 and P[0]..P[N-1]
		PermTree tree(P);
		//cerr << "case " << '\n';
		//for (int p : P) { cerr << p << ' '; } cerr << '\n';
		using num = modnum<998244353>;
		auto ans = [&](this auto&& self, int cur) -> vector<num> {
			//int a = tree.nodes[cur].l, b = tree.nodes[cur].r; cerr << a << '-' << b << ' '; for (int i = a; i <= b; i++) cerr << P[i] << ' '; cerr << '\n';
			if (tree.nodes[cur].type == PermTree::NodeType::LEAF) {
				return std::vector<num>({num(1)});
			}
			if (tree.nodes[cur].type == PermTree::NodeType::DECR) {
				auto a = self(tree.nodes[cur].c[0]);
				a.push_back(num(0));
				// walk all the way left and check it
				if (tree.nodes[tree.nodes[cur].c[1]].type == PermTree::NodeType::LEAF) {
					for (int i = 0; i+1 < int(a.size()); i++) {
						a[i+1] += a[i];
					}
				} else {
					auto b = self(tree.nodes[cur].c[1]);
					num bw = std::accumulate(b.begin(), b.end(), num(0));
					for (auto& r : a) { r *= bw; }
				}
				return a;
			} else {
				num res = 1;
				{
					auto a = self(tree.nodes[cur].c[0]);
					res *= std::accumulate(a.begin(), a.end(), num(0));
				}
				{
					auto b = self(tree.nodes[cur].c[1]);
					res *= std::accumulate(b.begin(), b.end(), num(0));
				}
				return std::vector<num>({res});
			}
		}(tree.root);
		cout << std::accumulate(ans.begin(), ans.end(), num(0)) << '\n';
	}

	return 0;
}
