#include<bits/stdc++.h>

namespace std {

template<class Fun>
class y_combinator_result {
	Fun fun_;
public:
	template<class T>
	explicit y_combinator_result(T &&fun): fun_(std::forward<T>(fun)) {}

	template<class ...Args>
	decltype(auto) operator()(Args &&...args) {
		return fun_(std::ref(*this), std::forward<Args>(args)...);
	}
};

template<class Fun>
decltype(auto) y_combinator(Fun &&fun) {
	return y_combinator_result<std::decay_t<Fun>>(std::forward<Fun>(fun));
}

} // namespace std

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

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		cerr << "Case #" << case_num << ":" << '\n';

		cin >> std::ws;
		string line_; getline(cin, line_);
		istringstream line(line_);
		int N, K; line >> N >> K;

		struct rect {
			int l, r, h;
		};
		vector<rect> R(N);
		{
			for (int i = 0; i < K; i++) {
				cin >> R[i].l;
			}
			int64_t A, B, C, D; cin >> A >> B >> C >> D;
			for (int i = K; i < N; i++) {
				R[i].l = int((A * R[i-2].l + B * R[i-1].l + C) % D + 1);
			}
		}
		{
			// First get the width
			if (int W; line >> W) {
				for (int i = 0; i < N; i++) {
					R[i].r = W;
				}
			} else {
				for (int i = 0; i < K; i++) {
					cin >> R[i].r;
				}
				int64_t A, B, C, D; cin >> A >> B >> C >> D;
				for (int i = K; i < N; i++) {
					R[i].r = int((A * R[i-2].r + B * R[i-1].r + C) % D + 1);
				}
			}

			// Now make it the right bound
			for (int i = 0; i < N; i++) {
				R[i].r += R[i].l;
			}
		}
		{
			for (int i = 0; i < K; i++) {
				cin >> R[i].h;
			}
			int64_t A, B, C, D; cin >> A >> B >> C >> D;
			for (int i = K; i < N; i++) {
				R[i].h = int((A * R[i-2].h + B * R[i-1].h + C) % D + 1);
			}
		}

		//cerr << "rectangles" << '\n';
		//for (const rect& r : R) { cerr << r.l << '-' << r.r << ' ' << r.h << '\n'; }

		vector<pair<int, int*>> coords; coords.reserve(2*N);
		for (rect& r : R) {
			coords.emplace_back(r.l, &r.l);
			coords.emplace_back(r.r, &r.r);
		}
		sort(coords.begin(), coords.end());

		int M = 0;
		for (int i = 0; i < int(coords.size()); i++) {
			if (M == 0 || coords[i].first != coords[M-1].first) {
				coords[M++].first = coords[i].first;
			}
			*coords[i].second = M;
		}
		coords.resize(M);

		//cerr << "compressed rectangles" << '\n';
		//for (const rect& r : R) { cerr << r.l << '-' << r.r << ' ' << r.h << '\n'; }

		int S = M;

		const int INF = 1.1e9;
		struct seg_node {
			int w = 0; // total width
			int horiz = 0; // horizontal semiperimeter

			int64_t vert = 0; // total vertical perimeter

			int left_height = 0;
			int right_height = 0;

			int min_height = 0;
			int min_height2 = INF;

			int num_min_vert = 0;
		};
		vector<seg_node> seg(2*S);
		auto update_node = [&](int i) -> void {
			//cerr << "update_node " << i << '\n';
			if (i >= S) {
				// should be nothing to do
			} else {
				seg_node& r = seg[i];
				const seg_node& a = seg[2*i];
				const seg_node& b = seg[2*i+1];

				r.w = a.w + b.w;
				r.horiz = a.horiz + b.horiz;
				r.vert = a.vert + b.vert + abs(a.right_height - b.left_height);

				r.left_height = a.left_height;
				r.right_height = b.right_height;

				r.min_height = min(a.min_height, b.min_height);
				r.min_height2 = min(
					a.min_height == r.min_height ? a.min_height2 : a.min_height,
					b.min_height == r.min_height ? b.min_height2 : b.min_height
				);

				r.num_min_vert = 0;
				if (a.min_height == r.min_height) r.num_min_vert += a.num_min_vert;
				if (b.min_height == r.min_height) r.num_min_vert += b.num_min_vert;
				// if exactly one is taller
				if ((a.right_height == r.min_height) ^ (b.left_height == r.min_height)) {
					r.num_min_vert ++;
				}
			}
		};
		auto apply_change = std::y_combinator([&](auto self, int i, int h) -> void {
			seg_node& r = seg[i];
			assert(h < INF);
			assert(r.min_height < r.min_height2);
			if (h >= r.min_height2) {
				assert(i < S);
				self(2 * i, h);
				self(2 * i + 1, h);
				update_node(i);
				return;
			}

			// otherwise, we just apply ourself
			if (h <= r.min_height) return;
			assert(r.min_height < h && h < r.min_height2);

			// also set the horizontal width
			assert(h > 0);
			r.horiz = r.w;

			r.vert -= int64_t(r.num_min_vert) * int64_t(h - r.min_height);

			if (r.left_height == r.min_height) {
				r.left_height = h;
			}
			if (r.right_height == r.min_height) {
				r.right_height = h;
			}

			r.min_height = h;
		});
		auto downdate_node = [&](int i) -> void {
			//cerr << "downdate_node " << i << '\n';
			if (i < S) {
				apply_change(2 * i, seg[i].min_height);
				apply_change(2 * i + 1, seg[i].min_height);
				// in theory, no need to update
			}
		};
		auto downdate_all = std::y_combinator([&](auto self, int i) -> void {
			if (!i) return;
			self(i/2);
			downdate_node(i);
		});
		auto update_all = [&](int i) -> void {
			for (int a = i; a; a /= 2) update_node(a);
		};
		auto first_changed = [](int i) -> int {
			return (i >> __builtin_ctz(i)) >> 1;
		};
		auto downdate_changed = [&](int i) -> void {
			downdate_all(first_changed(i));
		};
		auto update_changed = [&](int i) -> void {
			update_all(first_changed(i));
		};

		// Initialize
		for (int i = 1; i < M; i++) {
			seg[S+i].w = coords[i].first - coords[i-1].first;
		}
		for (int a = S-1; a; --a) update_node(a);

		auto insert_rect = [&](const rect& r) -> void {
			assert(1 <= r.l && r.l < r.r && r.r <= M);
			downdate_changed(S + r.l);
			downdate_changed(S + r.r);
			for (int a = S + r.l, b = S + r.r; a < b; a /= 2, b /= 2) {
				if (a & 1) {
					//cerr << "apply " << a << ' ' << r.h << '\n';
					apply_change(a++, r.h);
				}
				if (b & 1) {
					//cerr << "apply " << b-1 << ' ' << r.h << '\n';
					apply_change(--b, r.h);
				}
			}
			update_changed(S + r.l);
			update_changed(S + r.r);
		};

		auto get_perimeter = [&]() -> int64_t {
			//cerr << "horiz " << seg[1].horiz << ' ' << "vert " << seg[1].vert << '\n';
			return int64_t(seg[1].horiz) * 2 + seg[1].vert + abs(seg[1].left_height - seg[1].right_height);
		};

		using num = modnum<int(1e9)+7>;
		num ans = 1;
		for (const rect& r : R) {
			//cerr << "insert_rect " << r.l << '-' << r.r << ' ' << r.h << '\n';
			insert_rect(r);
			ans *= num(get_perimeter());
		}
		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}
