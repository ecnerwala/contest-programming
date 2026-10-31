#include <bits/stdc++.h>

struct dir_t {
	int64_t x, y;
	friend bool operator < (dir_t a, dir_t b) { return a.y * b.x < b.y * a.x; }
	friend bool operator > (dir_t a, dir_t b) { return a.y * b.x > b.y * a.x; }
	friend bool operator <= (dir_t a, dir_t b) { return a.y * b.x <= b.y * a.x; }
	friend bool operator >= (dir_t a, dir_t b) { return a.y * b.x >= b.y * a.x; }
	friend bool operator == (dir_t a, dir_t b) { return a.y * b.x == b.y * a.x; }
	friend bool operator != (dir_t a, dir_t b) { return a.y * b.x != b.y * a.x; }
	friend dir_t operator + (dir_t a, dir_t b) {
		return dir_t{a.x + b.x, a.y + b.y};
	}
	dir_t& operator += (dir_t b) {
		x += b.x;
		y += b.y;
		return *this;
	}
	friend dir_t operator * (dir_t a, int64_t k) {
		return dir_t{a.x * k, a.y * k};
	}
	friend dir_t operator * (int64_t k, dir_t a) {
		return dir_t{k * a.x, k * a.y};
	}
	friend std::ostream& operator << (std::ostream& o, dir_t a) {
		return o << '(' << a.x << ',' << a.y << ')';
	}
};

template <typename T> void setmin(T& a, T b) { if (b < a) a = b; }
template <typename T> void setmax(T& a, T b) { if (b > a) a = b; }

int64_t solve(int64_t a1, int64_t a2, int64_t b1, int64_t b2) {
	if (a1 == b1 && a2 == b2) {
		return 0;
	} else if (a1 == 0 && a2 == 0) {
		return -1;
	} else if (b1 == 0 && b2 == 0) {
		return 1;
	}

	dir_t a = {a1, a2};
	dir_t b = {b1, b2};
	dir_t mi = b + dir_t{1, 0};
	dir_t ma = b + dir_t{0, 1};
	if (b1 == 0) {
		if (a > mi) {
			return 1;
		} else if (a > dir_t{1, 1}){
			return 2;
		} else {
			return -1;
		}
	}
	if (b2 == 0) {
		if (a < ma) {
			return 1;
		} else if (a < dir_t{1, 1}) {
			return 2;
		} else {
			return -1;
		}
	}
	int steps = 1;
	while (true) {
		if (mi < a && a < ma) {
			return steps;
		}
		dir_t old_mi = mi;
		dir_t old_ma = ma;

		auto gen_convergents = [](dir_t frac, auto&& callback) -> void {
			dir_t lo = {1, 0};
			int64_t v_lo = frac.y;

			dir_t hi = {0, 1};
			int64_t v_hi = frac.x;

			callback(lo);
			callback(hi);

			callback(lo + hi);
			if (frac == lo || frac == hi) {
				return;
			}

			while (true) {
				assert(v_lo > 0 && v_hi > 0);
				callback(lo + hi);
				if (v_lo == v_hi) {
					callback(lo + lo + hi);
					callback(lo + hi + hi);
					break;
				} else if (v_lo < v_hi) {
					int64_t k = (v_hi - 1) / v_lo;
					v_hi -= k * v_lo;
					hi += k * lo;
					callback(hi);
				} else if (v_lo > v_hi) {
					int64_t k = (v_lo - 1) / v_hi;
					v_lo -= k * v_hi;
					lo += k * hi;
					callback(lo);
				} else assert(false);
			}
		};

		gen_convergents(old_mi, [&](auto f) -> void {
			if (old_mi < f && f < old_ma) {
				setmin(mi, f + dir_t{1, 0});
				setmax(ma, f + dir_t{0, 1});
			}
		});
		gen_convergents(old_ma, [&](auto f) -> void {
			if (old_mi < f && f < old_ma) {
				setmin(mi, f + dir_t{1, 0});
				setmax(ma, f + dir_t{0, 1});
			}
		});

		if (mi == old_mi && ma == old_ma) {
			return -1;
		}
		steps++;
	}
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int A1, A2, B1, B2; cin >> A1 >> A2 >> B1 >> B2;
		cout << solve(A1, A2, B1, B2) << '\n';
	}

	return 0;
}
