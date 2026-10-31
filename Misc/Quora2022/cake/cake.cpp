#include <bits/stdc++.h>

template <typename T> struct Point {
public:
	T x, y;
	Point() : x(0), y(0) {}
	Point(T x_, T y_) : x(x_), y(y_) {}
	template <typename U> explicit Point(const Point<U>& p) : x(p.x), y(p.y) {}
	Point(const std::pair<T, T>& p) : x(p.first), y(p.second) {}
	Point(const std::complex<T>& p) : x(real(p)), y(imag(p)) {}
	explicit operator std::pair<T, T> () const { return std::pair<T, T>(x, y); }
	explicit operator std::complex<T> () const { return std::complex<T>(x, y); }

	friend std::ostream& operator << (std::ostream& o, const Point& p) { return o << '(' << p.x << ',' << p.y << ')'; }
	friend std::istream& operator >> (std::istream& i, Point& p) { return i >> p.x >> p.y; }
	friend bool operator == (const Point& a, const Point& b) { return a.x == b.x && a.y == b.y; }
	friend bool operator != (const Point& a, const Point& b) { return !(a==b); }

	Point operator + () const { return Point(+x, +y); }
	Point operator - () const { return Point(-x, -y); }

	Point& operator += (const Point& p) { x += p.x, y += p.y; return *this; }
	Point& operator -= (const Point& p) { x -= p.x, y -= p.y; return *this; }
	Point& operator *= (const T& t) { x *= t, y *= t; return *this; }
	Point& operator /= (const T& t) { x /= t, y /= t; return *this; }

	friend Point operator + (const Point& a, const Point& b) { return Point(a.x+b.x, a.y+b.y); }
	friend Point operator - (const Point& a, const Point& b) { return Point(a.x-b.x, a.y-b.y); }
	friend Point operator * (const Point& a, const T& t) { return Point(a.x*t, a.y*t); }
	friend Point operator * (const T& t ,const Point& a) { return Point(t*a.x, t*a.y); }
	friend Point operator / (const Point& a, const T& t) { return Point(a.x/t, a.y/t); }

	T dist2() const { return x * x + y * y; }
	auto dist() const { return std::sqrt(dist2()); }
	Point unit() const { return *this / this->dist(); }
	auto angle() const { return std::atan2(y, x); }

	T int_norm() const { return __gcd(x,y); }
	Point int_unit() const { if (!x && !y) return *this; return *this / this->int_norm(); }

	// Convenient free-functions, mostly for generic interop
	friend auto norm(const Point& a) { return a.dist2(); }
	friend auto abs(const Point& a) { return a.dist(); }
	friend auto unit(const Point& a) { return a.unit(); }
	friend auto arg(const Point& a) { return a.angle(); }
	friend auto int_norm(const Point& a) { return a.int_norm(); }
	friend auto int_unit(const Point& a) { return a.int_unit(); }

	Point perp_cw() const { return Point(y, -x); }
	Point perp_ccw() const { return Point(-y, x); }

	friend T dot(const Point& a, const Point& b) { return a.x * b.x + a.y * b.y; }
	friend T cross(const Point& a, const Point& b) { return a.x * b.y - a.y * b.x; }
	friend T cross3(const Point& a, const Point& b, const Point& c) { return cross(b-a, c-a); }

	// Complex numbers and rotation
	friend Point conj(const Point& a) { return Point(a.x, -a.y); }

	// Returns conj(a) * b
	friend Point dot_cross(const Point& a, const Point& b) { return Point(dot(a, b), cross(a, b)); }
	friend Point cmul(const Point& a, const Point& b) { return dot_cross(conj(a), b); }
	friend Point cdiv(const Point& a, const Point& b) { return dot_cross(b, a) / b.norm(); }

	// Must be a unit vector; otherwise multiplies the result by abs(u)
	Point rotate(const Point& u) const { return dot_cross(conj(u), *this); }
	Point unrotate(const Point& u) const { return dot_cross(u, *this); }

	friend bool same_dir(const Point& a, const Point& b) { return cross(a,b) == 0 && dot(a,b) > 0; }

	// check if 180 <= s..t < 360
	friend bool is_reflex(const Point& a, const Point& b) { auto c = cross(a,b); return c ? (c < 0) : (dot(a, b) < 0); }

	// operator < (s,t) for angles in [base,base+2pi)
	friend bool angle_less(const Point& base, const Point& s, const Point& t) {
		int r = is_reflex(base, s) - is_reflex(base, t);
		return r ? (r < 0) : (0 < cross(s, t));
	}

	friend auto angle_cmp(const Point& base) {
		return [base](const Point& s, const Point& t) { return angle_less(base, s, t); };
	}
	friend auto angle_cmp_center(const Point& center, const Point& dir) {
		return [center, dir](const Point& s, const Point& t) -> bool { return angle_less(dir, s-center, t-center); };
	}

	// is p in [s,t] taken ccw? 1/0/-1 for in/border/out
	friend int angle_between(const Point& s, const Point& t, const Point& p) {
		if (same_dir(p, s) || same_dir(p, t)) return 0;
		return angle_less(s, p, t) ? 1 : -1;
	}
};

using pt_t = Point<double>;

std::mt19937 mt(48);

struct treap_node {
	std::mt19937::result_type pri = mt();
	std::array<treap_node*, 2> c = {nullptr, nullptr};
	pt_t A, B;
	pt_t first, last;
	double tot_cross = 0;

	void update() {
		tot_cross = cross(A, B);
		if (c[0]) tot_cross += c[0]->tot_cross;
		if (c[1]) tot_cross += c[1]->tot_cross;
		first = c[0] ? c[0]->first : A;
		last = c[1] ? c[1]->last : B;
	}

	double norm() const {
		return (A-B).dist2();
	}

	~treap_node() {
		delete c[0];
		delete c[1];
	}
};

void dump_tree(treap_node* a) {
	if (!a) return;
	dump_tree(a->c[0]);
	std::cerr << a->A << ' ' << a->B << ' ' << a->tot_cross << ' ' << a->first << ' ' << a->last << '\n';
	dump_tree(a->c[1]);
}

treap_node* merge(treap_node* a, treap_node* b) {
	if (!b) return a;
	if (!a) return b;
	treap_node* r;
	if (a->pri < b->pri) {
		r = a;
		r->c[1] = merge(a->c[1], b);
	} else {
		r = b;
		r->c[0] = merge(a, b->c[0]);
	}
	r->update();
	return r;
}

std::tuple<treap_node*, treap_node*, treap_node*> split_neg_pos(treap_node* r, pt_t l1, pt_t l2) {
	if (!r) {
		return {nullptr, nullptr, nullptr};
	}
	assert(r);

	auto vfirst = cross3(l1, l2, r->first);
	auto vA = cross3(l1, l2, r->A);
	auto vB = cross3(l1, l2, r->B);
	auto vlast = cross3(l1, l2, r->last);

	bool sfirst = vfirst < 0;
	bool sA = vA < 0;
	bool sB = vB < 0;
	bool slast = vlast < 0;

	assert(!(slast && !sfirst));
	if (sA && !sB) {
		auto a = std::exchange(r->c[0], nullptr);
		auto b = std::exchange(r->c[1], nullptr);
		r->update();
		return {a, b, r};
	}
	// otherwise, we need to decide which side to go to
	bool go_right;
	if (sB && !slast) {
		go_right = true;
	} else if (sfirst && !sA) {
		go_right = false;
	} else {
		assert(sA == sB && sB == slast && slast == sfirst);
		// look for the closest point to 0
		if (sA) {
			// all negative
			if (std::max(vA, vfirst) > std::max(vB, vlast)) {
				go_right = false;
			} else {
				go_right = true;
			}
		} else {
			if (std::min(vA, vfirst) < std::min(vB, vlast)) {
				go_right = false;
			} else {
				go_right = true;
			}
		}
	}
	treap_node *a, *b, *n;
	if (go_right) {
		a = r;
		std::tie(a->c[1], b, n) = split_neg_pos(r->c[1], l1, l2);
	} else {
		b = r;
		std::tie(a, b->c[0], n) = split_neg_pos(r->c[0], l1, l2);
	}
	r->update();
	return {a, b, n};
}

std::tuple<treap_node*, treap_node*, treap_node*> split_pos_neg(treap_node* r, pt_t l1, pt_t l2) {
	assert(r);

	auto vA = cross3(l1, l2, r->A);
	auto vB = cross3(l1, l2, r->B);

	bool sA = vA < 0;
	bool sB = vB < 0;

	if (!sA && sB) {
		auto a = std::exchange(r->c[0], nullptr);
		auto b = std::exchange(r->c[1], nullptr);
		r->update();
		return {a, b, r};
	}
	// otherwise, we need to decide which side to go to
	bool go_right = !sB;
	treap_node *a, *b, *n;
	if (go_right) {
		a = r;
		std::tie(a->c[1], b, n) = split_pos_neg(r->c[1], l1, l2);
	} else {
		b = r;
		std::tie(a, b->c[0], n) = split_pos_neg(r->c[0], l1, l2);
	}
	r->update();
	return {a, b, n};
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	std::vector<pt_t> pts(N);
	for (auto& p : pts) cin >> p;

	treap_node* ROOT = nullptr;
	for (int i = 0; i < N; i++) {
		auto* t = new treap_node;
		t->A = pts[i];
		t->B = pts[(i+1)%N];
		t->update();
		ROOT = merge(ROOT, t);
	}

	for (int z = 0; z < M; z++) {
		pt_t l1, l2; cin >> l1 >> l2;
		//cerr << "Splitting" << ' ' << l1 << ' ' << l2 << '\n';
		auto [a, b, neg_pos] = split_neg_pos(ROOT, l1, l2);
		if (neg_pos == nullptr) {
			ROOT = merge(a, b);
			continue;
		}
		ROOT = merge(b, a);
		//cerr << "Midpoint" << '\n'; dump_tree(neg_pos);
		//cerr << "Rest" << '\n'; dump_tree(ROOT);
		auto [pos, neg, pos_neg] = split_pos_neg(ROOT, l1, l2);

		assert(pos_neg);

		//cerr << "Split into" << '\n';
		//cerr << "neg_pos" << '\n'; dump_tree(neg_pos);
		//cerr << "pos" << '\n'; dump_tree(pos);
		//cerr << "pos_neg" << '\n'; dump_tree(pos_neg);
		//cerr << "neg" << '\n'; dump_tree(neg);

		pt_t neg_pos_pt;
		{
			auto vA = cross3(l1, l2, neg_pos->A);
			auto vB = cross3(l1, l2, neg_pos->B);
			assert(vA < 0 && !(vB < 0));
			if (false && vB - vA < 1e-12) {
				neg_pos_pt = (neg_pos->A + neg_pos->B) / 2;
			} else {
				neg_pos_pt = neg_pos->A * (vB / (vB - vA)) + neg_pos->B * (vA / (vA - vB));
			}
		}
		pt_t pos_neg_pt;
		{
			auto vA = cross3(l1, l2, pos_neg->A);
			auto vB = cross3(l1, l2, pos_neg->B);
			assert(!(vA < 0) && (vB < 0));
			if (false && vA - vB < 1e-12) {
				pos_neg_pt = (pos_neg->A + pos_neg->B) / 2;
			} else {
				pos_neg_pt = pos_neg->A * (vB / (vB - vA)) + pos_neg->B * (vA / (vA - vB));
			}
		}

		treap_node* new_neg_pos = new treap_node;
		new_neg_pos->A = neg_pos_pt;
		new_neg_pos->B = std::exchange(neg_pos->B, neg_pos_pt);
		neg_pos->update();
		new_neg_pos->update();
		if (neg_pos->norm() < 1e-12) neg_pos = nullptr;
		if (new_neg_pos->norm() < 1e-12) new_neg_pos = nullptr;

		treap_node* new_pos_neg = new treap_node;
		new_pos_neg->A = pos_neg_pt;
		new_pos_neg->B = std::exchange(pos_neg->B, pos_neg_pt);
		pos_neg->update();
		new_pos_neg->update();
		if (pos_neg->norm() < 1e-12) pos_neg = nullptr;
		if (new_pos_neg->norm() < 1e-12) new_pos_neg = nullptr;

		treap_node* neg_root = merge(new_pos_neg, merge(neg, neg_pos));
		if (neg_root) {
			treap_node* new_neg_edge = new treap_node;
			new_neg_edge->A = neg_root->last;
			new_neg_edge->B = neg_root->first;
			new_neg_edge->update();
			neg_root = merge(new_neg_edge, neg_root);
		}

		treap_node* pos_root = merge(new_neg_pos, merge(pos, pos_neg));
		if (pos_root) {
			treap_node* new_pos_edge = new treap_node;
			new_pos_edge->A = pos_root->last;
			new_pos_edge->B = pos_root->first;
			new_pos_edge->update();
			pos_root = merge(new_pos_edge, pos_root);
		}

		//cerr << "pos cnd" << '\n'; dump_tree(pos_root);
		//cerr << "neg cnd" << '\n'; dump_tree(neg_root);
		if (neg_root && (!pos_root || pos_root->tot_cross < neg_root->tot_cross)) {
			ROOT = neg_root;
			delete pos_root;
		} else {
			ROOT = pos_root;
			delete neg_root;
		}
		//cerr << "keeping" << '\n'; dump_tree(ROOT);
	}

	cout << fixed << setprecision(10) << (ROOT ? ROOT->tot_cross : 0) / 2 << '\n';

	delete ROOT;

	return 0;
}
