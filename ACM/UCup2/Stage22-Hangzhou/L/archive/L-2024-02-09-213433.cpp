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

	T int_norm() const { return std::gcd(x,y); }
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

	friend bool lex_less(const Point& a, const Point& b) {
		return std::tie(a.x, a.y) < std::tie(b.x, b.y);
	}

	friend bool operator < (const Point& a, const Point& b) {
		return lex_less(a, b);
	}

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

struct angle_comparator {
	template <typename T> bool operator () (const Point<T>& a, const Point<T>& b) const {
		return angle_less(Point<T>(1, 0), a, b);
	}
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int Q; cin >> Q;
		using pt_t = Point<int64_t>;
		std::vector<std::array<pt_t, 2>> segs; segs.reserve(Q);
		std::vector<int> ops; ops.reserve(Q);
		for (int i = 0; i < Q; i++) {
			char op; cin >> op;
			if (op == '+') {
				pt_t a, b; cin >> a >> b;
				if (lex_less(b, a)) std::swap(a, b);
				ops.push_back(int(segs.size()));
				segs.push_back({a, b});
			} else if (op == '-') {
				int idx; cin >> idx; idx--;
				ops.push_back(~ops[idx]);
			} else assert(false);
		}
		int N = int(segs.size());

		// First, let's group the segments into lines
		std::vector<int> seg_line_id(N);
		std::vector<std::pair<pt_t, int64_t>> lines; lines.reserve(N);

		{
			std::vector<std::pair<std::tuple<int64_t, int64_t, int64_t>, int>> seg_lines(N);
			for (int i = 0; i < N; i++) {
				pt_t int_dir = (segs[i][1] - segs[i][0]).int_unit();
				int64_t int_cross = cross(segs[i][1], int_dir);
				seg_lines[i] = {{int_dir.x, int_dir.y, int_cross}, i};
			}
			std::sort(seg_lines.begin(), seg_lines.end());
			for (int z = 0; z < N; z++) {
				if (z == 0 || seg_lines[z].first != seg_lines[z-1].first) {
					auto [x, y, v] = seg_lines[z].first;
					lines.push_back({{x, y}, z});
				}
				seg_line_id[seg_lines[z].second] = int(lines.size())-1;
			}
		}

		int L = int(lines.size());

		std::vector<bool> in_set(N);

		std::vector<std::set<std::pair<pt_t, int>>> line_segments(L);
		std::vector<pt_t> line_dir(L);
		std::vector<std::array<pt_t, 2>> line_ends(L);
		std::map<pt_t, int, angle_comparator> alive_lines;

		auto update_line_ends = [&](int l) -> void {
			line_ends[l] = {line_segments[l].begin()->first, line_segments[l].rbegin()->first};
			if (line_dir[l] < pt_t{0, 0}) {
				std::swap(line_ends[l][0], line_ends[l][1]);
			}
		};

		auto remove_in_set = [&](int i) -> void {
			int l = seg_line_id[i];
			for (int z = 0; z < 2; z++) {
				int num_erased = int(line_segments[l].erase({segs[i][z], i}));
				assert(num_erased == 1);
			}
			if (line_segments[l].empty()) {
				int num_erased = int(alive_lines.erase(line_dir[l]));
				assert(num_erased == 1);
			} else {
				update_line_ends(l);
			}
		};

		auto check_pts = [&](int l1, int l2) -> bool {
			int64_t c0 = cross3(line_ends[l1][0], line_ends[l1][1], line_ends[l2][0]);
			if (c0 < 0) return false;
			int64_t c1 = cross3(line_ends[l1][0], line_ends[l1][1], line_ends[l2][1]);
			if (c1 < 0) return false;
			int64_t c2 = cross3(line_ends[l2][0], line_ends[l2][1], line_ends[l1][0]);
			if (c2 < 0) return false;
			int64_t c3 = cross3(line_ends[l2][0], line_ends[l2][1], line_ends[l1][1]);
			if (c3 < 0) return false;
			return true;
		};

		auto cyc_prev = [&](auto it) {
			return std::prev(it == alive_lines.begin() ? alive_lines.end() : it);
		};
		auto cyc_next = [&](auto it) {
			++it;
			if (it == alive_lines.end()) it = alive_lines.begin();
			return it;
		};

		auto try_insert = [&](int i) -> bool {
			int l = seg_line_id[i];
			if (line_segments[l].empty()) line_dir[l] = lines[l].first;
			for (int z = 0; z < 2; z++) {
				auto [_, did_insert] = line_segments[l].insert({segs[i][z], i});
				assert(did_insert);
			}
			update_line_ends(l);

			if (line_segments[l].size() > 2) {
				assert(!alive_lines.empty());

				auto it = alive_lines.find(line_dir[l]);
				assert(it != alive_lines.end() && it->second == l);
				// we just need to extend
				if (alive_lines.size() >= 2) {
					auto pt = cyc_prev(it);
					auto nt = cyc_next(it);
					if (!(check_pts(it->second, nt->second) && check_pts(pt->second, it->second))) {
						remove_in_set(i);
						return false;
					}
				}
				return true;
			} else {
				if (alive_lines.empty()) {
					alive_lines.insert({line_dir[l], l});
					return true;
				}
				{
					int l2 = alive_lines.begin()->second;
					int64_t c0 = cross3(line_ends[l][0], line_ends[l][1], line_ends[l2][0]);
					int64_t c1 = cross3(line_ends[l][0], line_ends[l][1], line_ends[l2][1]);
					assert(c0 || c1);
					if (c0 < 0 || c1 < 0) {
						line_dir[l] = -line_dir[l];
						std::swap(line_ends[l][0], line_ends[l][1]);
					}
					if (alive_lines.size() == 1) {
						// We might have to flip the guy already in there
						int64_t c2 = cross3(line_ends[l2][0], line_ends[l2][1], line_ends[l][0]);
						int64_t c3 = cross3(line_ends[l2][0], line_ends[l2][1], line_ends[l][1]);
						assert(c2 || c3);
						if (c2 < 0 || c3 < 0) {
							line_dir[l2] = -line_dir[l2];
							std::swap(line_ends[l2][0], line_ends[l2][1]);
							alive_lines.clear();
							alive_lines.insert({line_dir[l2], l2});
						}
					}
				}
				if (alive_lines.find(line_dir[l]) != alive_lines.end()) {
					line_segments[l].clear();
					return false;
				}

				auto [it, did_insert] = alive_lines.insert({line_dir[l], l});
				assert(did_insert);
				auto pt = cyc_prev(it);
				auto nt = cyc_next(it);
				if (!(check_pts(it->second, nt->second) && check_pts(pt->second, it->second))) {
					remove_in_set(i);
					return false;
				}

				return true;
			}
		};

		std::vector<bool> alive(N);
		std::vector<int> to_insert; to_insert.reserve(N);

		auto reconcile = [&]() -> void {
			while (!to_insert.empty()) {
				int i = to_insert.back();
				if (!alive[i]) {
					to_insert.pop_back();
					continue;
				}
				if (try_insert(i)) {
					in_set[i] = true;
					to_insert.pop_back();
				} else {
					break;
				}
			}
		};

		auto insert = [&](int i) -> void {
			cerr << "Insert " << i << '\n';
			assert(!alive[i]);
			alive[i] = true;
			if (try_insert(i)) {
				in_set[i] = true;
			} else {
				to_insert.push_back(i);
			}
		};

		auto remove = [&](int i) -> void {
			cerr << "Remove " << i << '\n';
			assert(alive[i]);
			alive[i] = false;
			if (in_set[i]) {
				remove_in_set(i);
				in_set[i] = false;
				reconcile();
			}
		};

		std::string ans; ans.reserve(Q);
		for (int op : ops) {
			if (op >= 0) {
				insert(op);
			} else {
				remove(~op);
			}
			ans.push_back(to_insert.empty() ? '1' : '0');
		}
		cout << ans << '\n';
	}

	return 0;
}
