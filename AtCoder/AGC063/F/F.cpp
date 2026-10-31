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
	friend int64_t cross(dir_t a, dir_t b) {
		return a.x * b.y - a.y * b.x;
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

	if (b1 > b2 || (b1 == b2 && a1 > a2)) {
		std::swap(b1, b2);
		std::swap(a1, a2);
	}
	assert(b1 <= b2);

	dir_t a = {a1, a2};
	dir_t b = {b1, b2};
	dir_t mi = b + dir_t{1, 0};
	dir_t ma = b + dir_t{0, 1};
	if (b.x == 0) {
		if (a > mi) {
			return 1;
		} else if (a > dir_t{1, 1}){
			return 2;
		} else {
			return -1;
		}
	} else if (a.x == 0) {
		return -1;
	}

	int64_t a_slope = a.y / a.x;
	if (a_slope > b.y / b.x) {
		return -1;
	}

	if (b.x == b.y) {
		assert(a.x <= a.y);
		if (a < ma) {
			return 1;
		} else {
			return 2;
		}
	} else if (a.x >= a.y) {
		return -1;
	}

	dir_t lo = dir_t{1, a_slope - 1};
	dir_t hi = dir_t{0, 1};
	int64_t a_lo = a.y - a.x * lo.y;
	int64_t a_hi = a.x;
	assert(a_lo > 0 && a_hi > 0);
	assert(a.x > a_hi - a_lo && a_hi - a_lo > -a.y);
	int64_t v_lo = b.y - b.x * lo.y;
	int64_t v_hi = b.x;
	assert(v_lo > 0 && v_hi > 0);

	int ans = 1;
	while (false) {
		assert(a.x > a_hi - a_lo && a_hi - a_lo > -a.y);
		if (v_lo == v_hi) {
			return ans;
		} else if (v_lo > v_hi) {
			a_lo -= a_hi;
			v_lo -= v_hi;
			lo += hi;
			if (a.x > a_hi - a_lo && a_hi - a_lo > -a.y) {
				// do nothing
			} else {
				a = lo;
				a_lo = 0;
				a_hi = 1;
				assert(a.x > a_hi - a_lo && a_hi - a_lo > -a.y);
				ans++;
			}
		} else if (v_hi > v_lo) {
			a_hi -= a_lo;
			v_hi -= v_lo;
			hi += lo;
			if (a.x > a_hi - a_lo && a_hi - a_lo > -a.y) {
				// do nothing
			} else {
				a = hi;
				a_lo = 1;
				a_hi = 0;
				assert(a.x > a_hi - a_lo && a_hi - a_lo > -a.y);
				ans++;
			}
		} else assert(false);
		/*
		if (v_lo == v_hi) {
			return ans;
		} else if (v_lo > v_hi) {
			int64_t k = (v_lo - 1) / v_hi;
			// a_lo -= k * a_hi;
			int64_t ka;
			if (a_hi > 0) {
				ka = (a.x - a_hi + a_lo - 1) / a_hi;
			} else if (a_hi < 0) {
				ka = (a.y + a_hi - a_lo - 1) / -a_hi;
			} else {
				ka = k;
			}
			if (ka < k) {
				// just go up to ka
				a = lo + (ka+1) * hi;
				a_lo = 1;
				a_hi = 1;
				a_lo -= (k - ka) * a_hi;
				ans++;
			} else {
				a_lo -= k * a_hi;
			}
			v_lo -= k * v_hi;
			lo += k * hi;
		} else if (v_hi > v_lo) {
			int64_t k = (v_hi - 1) / v_lo;
			//a_hi -= k * a_lo;
			int64_t ka;
			if (a_lo > 0) {
				ka = (a.y + a_hi - a_lo - 1) / a_lo;
			} else if (a_lo < 0) {
				ka = (a.x - a_hi + a_lo - 1) / -a_hi;
			} else {
				ka = k;
			}
			if (ka < k) {
				// just go up to ka
				a = hi + (ka+1) * lo;
				a_lo = 1;
				a_hi = 1;
				a_hi -= (k - ka) * a_lo;
				ans++;
			} else {
				a_hi -= k * a_lo;
			}
			v_hi -= k * v_lo;
			hi += k * lo;
		} else assert(false);
		*/
	}


	std::vector<std::pair<int, dir_t>> b_cf; b_cf.reserve(50);
	// lo, then hi
	b_cf.push_back({0, {1, 0}});
	b_cf.push_back({0, {0, 1}});
	int64_t v0 = b.y;
	int64_t v1 = b.x;
	while (v0 != v1) {
		int64_t k = (v0 - 1) / v1;
		b_cf.push_back({k, b_cf.end()[-2].second + k * b_cf.end()[-1].second});
		v1 = std::exchange(v0, v1) - k * v1;
	}
	assert(b == b_cf.end()[-2].second + b_cf.end()[-1].second);

	int steps = 1;
	while (true) {
		if (mi < a && a < ma) {
			return steps;
		}

		dir_t old_mi = mi;
		dir_t old_ma = ma;

		assert(b == b_cf.end()[-2].second + b_cf.end()[-1].second);
		while (mi < b_cf.end()[-1].second && b_cf.end()[-1].second < ma) {
			b_cf.pop_back();
		}
		while (mi < b_cf.end()[-2].second && b_cf.end()[-2].second < ma) {
			b_cf.pop_back();
			b_cf.pop_back();
		}

		int64_t v0_mi = cross(mi, b_cf.end()[-2].second);
		int64_t v0_ma = cross(ma, b_cf.end()[-2].second);
		int64_t v1_mi = cross(mi, b_cf.end()[-1].second);
		int64_t v1_ma = cross(ma, b_cf.end()[-1].second);

		if (v0_mi + v1_mi <= 0) {
			assert(v0_ma + v1_ma < 0);
			assert(v1_mi > 0);
			int64_t k = (-v0_mi) / v1_mi;
			b_cf.push_back({k, b_cf.end()[-2].second + k * b_cf.end()[-1].second});
		} else if (v0_ma + v1_ma >= 0) {
			assert(v0_mi + v1_mi > 0);
			assert(v1_ma < 0);
			int64_t k = v0_ma / (-v1_ma);
			b_cf.push_back({k, b_cf.end()[-2].second + k * b_cf.end()[-1].second});
		} else {
			// we're fine already
		}

		b = b_cf.end()[-2].second + b_cf.end()[-1].second;
		mi = b + dir_t{1, 0};
		ma = b + dir_t{0, 1};
		// just mark it as infty, since we've already checked this
		if (ma.x == 1) ma = dir_t{0, 1};

		if (mi == old_mi && ma == old_ma) {
			assert(false);
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
