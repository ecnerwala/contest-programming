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

using num = modnum<int(1e9)+7>;
struct mat {
	std::array<std::array<num, 2>, 2> v{std::array<num, 2>{num(1), num(0)}, {num(0), num(1)}};
	mat() {}
	mat(std::array<std::array<num, 2>, 2> v_) : v(v_) {}
	friend mat operator * (const mat& a, const mat& b) {
		mat r;
		for (int x = 0; x < 2; x++) {
			for (int y = 0; y < 2; y++) {
				r.v[x][y] = a.v[x][0]*b.v[0][y] + a.v[x][1]*b.v[1][y];
			}
		}
		return r;
	}
};
std::mt19937 mt(48);
struct treap {
	treap* c[2] = {nullptr, nullptr};
	std::mt19937::result_type pri = mt();
	int leftmost, rightmost;
	int ind;
	bool is_pos;
	mat val;

	static mat get_trans(int x, int y, bool x_pos) {
		if (!x_pos) {
			assert((y - x) % 2 == 0);
			return mat({std::array<num, 2>{num(1), 0}, {(y-x)/2, 1}});
		} else {
			return mat({std::array<num, 2>{num(1), (y-x+1)/2}, {0, (y-x+1)%2}});
		}
	}

	void update() {
		val = mat();
		leftmost = rightmost = ind;
		if (c[0]) {
			leftmost = c[0]->leftmost;
			val = c[0]->val * get_trans(c[0]->rightmost, ind, !is_pos) * val;
		}
		if (c[1]) {
			rightmost = c[1]->rightmost;
			val = val * get_trans(ind, c[1]->leftmost, is_pos) * c[1]->val;
		}
	}
};

template <typename F>
std::pair<treap*, treap*> split(treap* r, const F& f) {
	if (!r) return {nullptr, nullptr};
	treap* a;
	treap* b;
	if (f(r)) {
		b = r;
		std::tie(a, b->c[0]) = split(r->c[0], f);
	} else {
		a = r;
		std::tie(a->c[1], b) = split(r->c[1], f);
	}
	r->update();
	return {a, b};
}

treap* merge(treap* a, treap* b) {
	if (!a) return b;
	if (!b) return a;
	treap* r;
	if (a->pri > b->pri) {
		r = a;
		r->c[1] = merge(a->c[1], b);
	} else {
		r = b;
		r->c[0] = merge(a, b->c[0]);
	}
	r->update();
	return r;
}

std::pair<treap*, treap*> pop_back(treap* a) {
	bool done = false;
	return split(a, [&](const treap* t) -> bool {
		if (!done && t->c[1] == nullptr) {
			done = true;
			return 1;
		} else {
			return 0;
		}
	});
}

std::pair<treap*, treap*> pop_front(treap* a) {
	bool done = false;
	auto r = split(a, [&](const treap* t) -> bool {
		if (!done && t->c[0] == nullptr) {
			done = true;
			return 0;
		} else {
			return 1;
		}
	});
	return {r.second, r.first};
}

treap* push_back(treap* a, treap* n) {
	if (a && a->rightmost == n->ind) {
		return pop_back(a).first;
	} else {
		return merge(a, n);
	}
}

treap* push_front(treap* a, treap* n) {
	if (a && a->leftmost == n->ind) {
		return pop_front(a).first;
	} else {
		return merge(n, a);
	}
}

void print_treap(treap* a) {
	if (a->c[0]) print_treap(a->c[0]);
	std::cerr << "node " << a->ind << ' ' << (a->is_pos ? '+' : '-') << '\n';
	if (a->c[1]) print_treap(a->c[1]);
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int Q; cin >> Q;
	vector<treap> treap_pool(Q * 10);
	int pool_idx = 0;

	auto allocate = [&](int ind, bool is_pos) -> treap* {
		treap* res = &treap_pool[pool_idx++];
		res->ind = ind, res->is_pos = is_pos;
		res->update();
		return res;
	};

	treap* extra_root = allocate(1, true);

	while (Q--) {
		int a; cin >> a;
		while (true) {
			if (a == -1) break;
			if (a == 0) a = 1;

			treap* lo;
			treap* hi;
			tie(lo, hi) = split(extra_root->c[1], [&](const treap* t) { return t->is_pos ? (t->ind >= a) : (t->ind > a); });
			if (!hi) {
				lo = push_back(lo, allocate(a-1, false));
				hi = push_front(hi, allocate(a+1, true));
				extra_root->c[1] = merge(lo, hi);
				break;
			}
			treap* up;
			tie(hi, up) = pop_front(hi);
			if (!up->is_pos) {
				hi = push_front(hi, up);
				lo = push_back(lo, allocate(a-1, false));
				hi = push_front(hi, allocate(a+1, true));
				extra_root->c[1] = merge(lo, hi);
				break;
			}

			if (up->ind == a) {
				up->ind -= 2;
				up->update();
				lo = push_back(lo, up);
				extra_root->c[1] = merge(lo, hi);
				a++;
				continue;
			}

			treap* nup = allocate(up->ind-1, false);

			up->ind++;
			up->update();
			hi = push_front(hi, up);

			int nxt_a;
			if ((a - lo->rightmost) % 2 == 1) {
				treap* down; tie(lo, down) = pop_back(lo);
				nxt_a = down->ind - 1;
				down->ind++;
				down->update();
				lo = push_back(lo, down);
			} else {
				nxt_a = -1;
			}
			lo = push_back(lo, allocate(a, true));
			lo = push_back(lo, nup);

			extra_root->c[1] = merge(lo, hi);
			a = nxt_a;
		}
		extra_root->update();
		cout << extra_root->val.v[0][0] << '\n';

		//print_treap(extra_root);
		//cerr << extra_root->val.v[0][0] << '\n';

		//cerr << '\n';
	}

	return 0;
}
