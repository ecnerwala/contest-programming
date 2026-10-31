#include <bits/stdc++.h>

namespace seg_tree {

// Floor of log_2(a); index of highest 1-bit
int log_2(int a) {
	return a ? (8 * sizeof(a)) - 1 - __builtin_clz(a) : -1;
}

int next_pow_2(int a) {
	assert(a > 0);
	return 1 << log_2(2*a-1);
}

template <typename F> void for_parents_up(int a, F f) {
	a >>= 1;
	for (; a > 0; a >>= 1) {
		f(a);
	}
}

template <typename F> void for_parents_down(int a, F f) {
	a >>= 1;
	for (int L = log_2(a); L >= 0; L--) {
		f(a >> L);
	}
}

template <typename F> void for_range_parents_up(std::array<int, 2> r, F f) {
	auto [a, b] = r;
	assert(a && b);
	a >>= __builtin_ctz(a);
	b >>= __builtin_ctz(b);

	// TODO: dedup
	for_parents_up(a, f);
	for_parents_up(b, f);
}

template <typename F> void for_range_parents_down(std::array<int, 2> r, F f) {
	auto [a, b] = r;
	assert(a && b);
	a >>= __builtin_ctz(a);
	b >>= __builtin_ctz(b);

	// TODO: dedup
	for_parents_down(a, f);
	for_parents_down(b, f);
}

template <typename F> void for_point(int a, F f) {
	for (; a > 0; a >>= 1) {
		f(a);
	}
}

template <typename F> void for_range(std::array<int, 2> r, F f) {
	auto [a, b] = r;
	assert(a <= b && b <= 2*a);
	for (; a < b; a >>= 1, b >>= 1) {
		if (a & 1) f(a++, false);
		if (b & 1) f(--b, true);
	}
}

namespace in_order {

int get_point(int N, int a) {
	assert(0 <= a && a < N);
	int S = next_pow_2(N);
	a += S;
	return a >= 2 * N ? a - N : a;
}

std::array<int, 2> get_range(int N, std::array<int, 2> p) {
	auto [a, b] = p;
	assert(0 <= a && a <= b && b <= N);
	int S = next_pow_2(N);
	a += S, b += S;
	return { (a >= 2 * N ? 2*(a-N) : a), (b >= 2 * N ? 2*(b-N) : b) };
}

int get_node_index(int N, int a) {
	assert(N <= a && a < 2 * N);
	int S = next_pow_2(N);
	return (a < S ? a + N : a) - S;
}

std::array<int, 2> get_node_bounds(int N, int a) {
	assert(1 <= a && a < 2 * N);
	int l = __builtin_clz(a) - __builtin_clz(2*N-1);
	int S = next_pow_2(N);
	int x = a << l, y = (a+1) << l;
	assert(S <= x && x < y && y <= 2*S);
	return {(x >= 2 * N ? (x>>1) + N : x) - S, (y >= 2 * N ? (y>>1) + N : y) - S};
}

int get_node_size(int N, int a) {
	assert(1 <= a && a < 2 * N);
	auto [x, y] = get_node_bounds(N, a);
	return y - x;
}

} // namespace in_order

namespace circular {

int get_point(int N, int a) {
	assert(0 <= a && a < N);
	return N + a;
}

std::array<int, 2> get_range(int N, std::array<int, 2> p) {
	auto [a, b] = p;
	assert(0 <= a && a <= b && b <= N);
	return { N + a, N + b };
}

int get_node_index(int N, int a) {
	assert(N <= a && a < 2 * N);
	return a - N;
}

std::array<int, 2> get_node_bounds(int N, int a) {
	assert(1 <= a && a < 2 * N);
	int l = __builtin_clz(a) - __builtin_clz(2*N-1);
	int S = next_pow_2(N);
	int x = a << l, y = (a+1) << l;
	assert(S <= x && x < y && y <= 2*S);
	return {(x >= 2 * N ? x >> 1 : x) - N, (y >= 2 * N ? y >> 1 : y) - N};
}

int get_node_size(int N, int a) {
	assert(1 <= a && a < 2 * N);
	return in_order::get_node_size(N, a);
}

} // namespace circular

} // namespace seg_tree

template <class To, class From>
typename std::enable_if_t<
	sizeof(To) == sizeof(From) &&
	std::is_trivially_copyable_v<From> &&
	std::is_trivially_copyable_v<To>,
	To>
// constexpr support needs compiler magic
bit_cast(const From& src) noexcept
{
	static_assert(std::is_trivially_constructible_v<To>,
		"This implementation additionally requires destination type to be trivially constructible");

	To dst;
	std::memcpy(&dst, &src, sizeof(To));
	return dst;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int L, NR; cin >> L >> NR;

	struct robot_t {
		array<int, 2> x; int64_t d;
	};

	int64_t tot_val = 0;
	vector<robot_t> robots(NR);
	for (auto& r : robots) {
		cin >> r.x[0] >> r.x[1] >> r.d;
		r.x[0]--;
		tot_val += r.d;
	}


	sort(robots.begin(), robots.end(), [&](const auto& a, const auto& b) {
		return a.x[0] < b.x[0];
	});

	// First, binary search for the shortest pillar:
	double lowest = [&]() -> double {
		auto is_good = [&](double h) -> bool {
			auto it = robots.begin();
			priority_queue<pair<int, double>, vector<pair<int, double>>, greater<pair<int, double>>> pq;
			for (int l = 0; l < L; l++) {
				while (it != robots.end() && it->x[0] <= l) {
					assert(it->x[0] == l);
					pq.push({it->x[1], double(it->d)});
					++it;
				}
				double cur_need = h;
				while (true) {
					if (pq.empty()) return false;
					auto [r, v] = pq.top();
					pq.pop();
					if (r <= l) continue;
					if (v >= cur_need) {
						pq.push({r, v - cur_need});
						break;
					} else {
						cur_need -= v;
					}
				}
			}
			return true;
		};

		double mi = 0;
		double ma = double(tot_val * 2);
		for (int z = 0; z < 64; z++) {
			double md = bit_cast<double>((bit_cast<uint64_t>(mi) + bit_cast<uint64_t>(ma)) / 2);
			if (is_good(md)) {
				mi = md;
			} else {
				ma = md;
			}
		}
		return mi;
	}();

	cerr << fixed << setprecision(10) << lowest << '\n';

	// Binary search for the min highest pillar
	// Bad if there's an interval such that
	// l * H + (robots entirely inside l-r) > r * H
	double highest = [&]() -> double {
		auto is_good = [&](double h) -> bool {
			auto it = robots.begin();
			priority_queue<pair<int, double>, vector<pair<int, double>>, greater<pair<int, double>>> pq;
			for (int l = 0; l < L; l++) {
				while (it != robots.end() && it->x[0] <= l) {
					assert(it->x[0] == l);
					pq.push({it->x[1], double(it->d)});
					++it;
				}
				double cur_need = h;
				while (true) {
					if (pq.empty()) break;
					auto [r, v] = pq.top();
					pq.pop();
					if (r <= l) {
						return false;
					}
					if (v >= cur_need) {
						pq.push({r, v - cur_need});
						break;
					} else {
						cur_need -= v;
					}
				}
			}
			if (!pq.empty()) return false;
			return true;
		};

		double mi = 0;
		double ma = double(tot_val * 2);
		for (int z = 0; z < 64; z++) {
			double md = bit_cast<double>((bit_cast<uint64_t>(mi) + bit_cast<uint64_t>(ma)) / 2);
			if (is_good(md)) {
				ma = md;
			} else {
				mi = md;
			}
		}
		return mi;
	}();

	cerr << fixed << setprecision(10) << highest << '\n';

	cout << fixed << setprecision(20) << lowest / highest << '\n';

	return 0;
}
