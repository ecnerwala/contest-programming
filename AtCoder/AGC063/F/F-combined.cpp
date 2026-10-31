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

		dir_t mi_lo{1, 0};
		dir_t mi_hi{0, 1};
		int64_t v_mi_lo = mi.y;
		int64_t v_mi_hi = mi.x;
		assert(v_mi_hi > 0);
		dir_t ma_lo{1, 0};
		dir_t ma_hi{0, 1};
		int64_t v_ma_lo = ma.y;
		int64_t v_ma_hi = ma.x;
		assert(v_ma_lo > 0);

		const int64_t INF = 1e18;
		auto get_mi_dir = [&]() -> int64_t {
			assert(v_mi_lo >= 0);
			assert(v_mi_hi > 0);
			if (v_mi_hi > v_mi_lo) {
				if (v_mi_lo == 0) return INF;
				return (v_mi_hi - 1) / v_mi_lo;
			} else {
				return -(v_mi_lo / v_mi_hi);
			}
		};
		auto update_mi = [&](int64_t k, bool check = false) {
			if (k > 0) {
				assert(k != INF);
				v_mi_hi -= k * v_mi_lo;
				mi_hi += k * mi_lo;
				if (check) {
					setmin(mi, mi_hi + dir_t{1, 0});
				}
			} else if (k < 0) {
				k = -k;
				v_mi_lo -= k * v_mi_hi;
				mi_lo += k * mi_hi;
			} else assert(false);
			assert(v_mi_hi > 0);
			assert(v_mi_lo >= 0);
		};
		auto get_ma_dir = [&]() -> int64_t {
			assert(v_ma_lo > 0);
			assert(v_ma_hi >= 0);
			if (v_ma_lo > v_ma_hi) {
				if (v_ma_hi == 0) return -INF;
				return -((v_ma_lo - 1) / v_ma_hi);
			} else {
				return v_ma_hi / v_ma_lo;
			}
		};
		auto update_ma = [&](int64_t k, bool check = false) {
			if (k < 0) {
				k = -k;
				assert(k != INF);
				v_ma_lo -= k * v_ma_hi;
				ma_lo += k * ma_hi;
				if (check) {
					setmax(ma, ma_lo + dir_t{0, 1});
				}
			} else if (k > 0) {
				v_ma_hi -= k * v_ma_lo;
				ma_hi += k * ma_lo;
			} else assert(false);
			assert(v_ma_lo > 0);
			assert(v_ma_hi >= 0);
		};

		while (true) {
			assert(mi_lo == ma_lo);
			assert(mi_hi == ma_hi);
			int64_t k_mi = get_mi_dir();
			int64_t k_ma = get_ma_dir();
			if (k_mi == k_ma) {
				update_mi(k_mi);
				update_ma(k_ma);
				continue;
			}
			assert(k_mi >= k_ma);
			if (k_ma > 0) {
				update_mi(k_ma + 1, true);
				if (k_mi - k_ma > 1) {
					update_mi(k_mi - k_ma - 1, true);
				}
				update_ma(k_ma);
				update_ma(-1, true);
				break;
			} else if (k_mi < 0) {
				update_mi(k_mi);
				update_mi(1, true);
				update_ma(k_mi-1, true);
				if (k_mi - k_ma > 1) {
					update_ma(k_ma - (k_mi - 1), true);
				}
				break;
			} else {
				assert(k_mi > 0 && k_ma < 0);
				update_mi(1, true);
				update_ma(-1, true);
				update_mi(k_mi - 1, true);
				update_ma(k_ma - 1, true);
				break;
			}
		}

		while (true) {
			int64_t k_mi = get_mi_dir();
			if (k_mi == INF) {
				break;
			} else {
				update_mi(k_mi, true);
			}
		}
		while (true) {
			int64_t k_ma = get_ma_dir();
			if (k_ma == -INF) {
				break;
			} else {
				update_ma(k_ma, true);
			}
		}

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
