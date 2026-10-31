#include <bits/stdc++.h>

/** Binary-indexed tree
 *
 *  A binary indexed tree with N nodes of type T provides the
 *  following two functions for 0 <= i <= N:
 *
 *      prefix(int i) -> prefix_iterator<T>
 *      suffix(int i) -> suffix_iterator<T>
 *
 *  such that size(suffix(i) intersect prefix(j)) = (1 if i < j else 0).
 *  Furthermore, the resulting lists always have size at most log_2(N).
 *
 *  This can be used to implement either point-update/(prefix|suffix)-query or
 *  (prefix|suffix)-update/point-query over a virtual array of size N of a
 *  commutative monoid. This can be generalized to implement
 *  point-update/range-query or range-update/point-query over a virtual array
 *  of size N of a commutative group.
 *
 *  With 0-indexed data, prefixes are more natural:
 *   * For range update/query, use for_prefix for the ranges and for_suffix for the points.
 *   * For prefix update/query, no change.
 *   * For suffix update/query, use for_prefix(point + 1); 1-index the data.
 */
template <typename T> class binary_indexed_tree {
private:
	std::vector<T> dat;
public:
	binary_indexed_tree() {}
	explicit binary_indexed_tree(size_t N) : dat(N) {}
	binary_indexed_tree(size_t N, const T& t) : dat(N, t) {}

	size_t size() const { return dat.size(); }
	const std::vector<T>& data() const { return dat; }
	std::vector<T>& data() { return dat; }

private:
	template <typename I, typename S = I> struct iterator_range {
	private:
		I begin_;
		S end_;
	public:
		iterator_range() : begin_(), end_() {}
		iterator_range(const I& begin__, const S& end__) : begin_(begin__), end_(end__) {}
		iterator_range(I&& begin__, S&& end__) : begin_(begin__), end_(end__) {}
		I begin() const { return begin_; }
		S end() const { return end_; }
	};

public:
	class const_suffix_iterator {
	private:
		const T* dat;
		int a;
		const_suffix_iterator(const T* dat_, int a_) : dat(dat_), a(a_) {}
		friend class binary_indexed_tree;
	public:
		friend bool operator != (const const_suffix_iterator& i, const const_suffix_iterator& j) {
			assert(j.dat == nullptr);
			return i.a < j.a;
		}
		const_suffix_iterator& operator ++ () {
			a |= a+1;
			return *this;
		}
		const T& operator * () const {
			return dat[a];
		}
	};
	using const_suffix_range = iterator_range<const_suffix_iterator>;
	const_suffix_range suffix(int a) const {
		assert(0 <= a && a <= int(dat.size()));
		return const_suffix_range{const_suffix_iterator{dat.data(), a}, const_suffix_iterator{nullptr, int(dat.size())}};
	}

	class suffix_iterator {
	private:
		T* dat;
		int a;
		suffix_iterator(T* dat_, int a_) : dat(dat_), a(a_) {}
		friend class binary_indexed_tree;
	public:
		friend bool operator != (const suffix_iterator& i, const suffix_iterator& j) {
			assert(j.dat == nullptr);
			return i.a < j.a;
		}
		suffix_iterator& operator ++ () {
			a |= a+1;
			return *this;
		}
		T& operator * () const {
			return dat[a];
		}
	};
	using suffix_range = iterator_range<suffix_iterator>;
	suffix_range suffix(int a) {
		assert(0 <= a && a <= int(dat.size()));
		return suffix_range{suffix_iterator{dat.data(), a}, suffix_iterator{nullptr, int(dat.size())}};
	}

	class const_prefix_iterator {
	private:
		const T* dat;
		int a;
		const_prefix_iterator(const T* dat_, int a_) : dat(dat_), a(a_) {}
		friend class binary_indexed_tree;
	public:
		friend bool operator != (const const_prefix_iterator& i, const const_prefix_iterator& j) {
			assert(j.dat == nullptr);
			return i.a > 0;
		}
		const_prefix_iterator& operator ++ () {
			a &= a-1;
			return *this;
		}
		const T& operator * () const {
			return dat[a-1];
		}
	};
	using const_prefix_range = iterator_range<const_prefix_iterator>;
	const_prefix_range prefix(int a) const {
		return const_prefix_range{const_prefix_iterator{dat.data(), a}, const_prefix_iterator{nullptr, 0}};
	}

	class prefix_iterator {
	private:
		T* dat;
		int a;
		prefix_iterator(T* dat_, int a_) : dat(dat_), a(a_) {}
		friend class binary_indexed_tree;
	public:
		friend bool operator != (const prefix_iterator& i, const prefix_iterator& j) {
			assert(j.dat == nullptr);
			return i.a > 0;
		}
		prefix_iterator& operator ++ () {
			a &= a-1;
			return *this;
		}
		T& operator * () const {
			return dat[a-1];
		}
	};
	using prefix_range = iterator_range<prefix_iterator>;
	prefix_range prefix(int a) {
		return prefix_range{prefix_iterator{dat.data(), a}, prefix_iterator{nullptr, 0}};
	}
};

const int MAXV = int(2.5e5) + 10;

using rect_t = std::array<std::array<int, 2>, 2>;

std::vector<std::pair<std::array<int, 2>, int>> rects_to_quadrants(std::vector<std::array<rect_t, 2>> rects) {
	std::vector<std::pair<std::array<int, 2>, int>> resp; resp.reserve(rects.size() * 8);
	for (auto move : rects) {
		for (int z = 0; z < 2; z++) {
			auto r = move[z];
			int w = z ? -1 : 1;
			resp.push_back({{r[0][0], r[1][0]}, w});
			resp.push_back({{r[0][1], r[1][0]}, -w});
			resp.push_back({{r[0][0], r[1][1]}, -w});
			resp.push_back({{r[0][1], r[1][1]}, w});
		}
	}
	return resp;
}
void solve_ortho_real(std::vector<std::pair<std::array<int, 2>, int64_t*>> points, std::vector<std::pair<std::array<int, 2>, int>> quadrants) {

	std::sort(points.begin(), points.end(), [](auto& a, auto& b) {
		return a.first[0] < b.first[0];
	});
	std::sort(quadrants.begin(), quadrants.end(), [](auto& a, auto& b) {
		return a.first[0] < b.first[0];
	});

	auto qit = quadrants.begin();
	binary_indexed_tree<std::pair<int64_t, int64_t>> bit(2*MAXV+1);
	for (auto [p, cnt] : points) {
		for (; qit != quadrants.end() && qit->first[0] < p[0]+1; ++qit) {
			for (auto& a : bit.suffix(qit->first[1])) {
				a.first += qit->second;
				a.second += qit->second * qit->first[0];
			}
		}
		for (auto& a : bit.prefix(p[1]+1)) {
			*cnt += a.first * int64_t(p[0]+1) - a.second;
		}
	}
}

std::vector<int64_t> solve_ortho(std::vector<std::array<int, 2>> points_, std::vector<std::array<rect_t, 2>> rects) {
	std::vector<int64_t> res(points_.size());

	std::vector<std::pair<std::array<int, 2>, int64_t*>> points(points_.size());
	for (int i = 0; i < int(points.size()); i++) {
		points[i] = {points_[i], &res[i]};
	}

	solve_ortho_real(points, rects_to_quadrants(rects));

	return res;
}

std::vector<int64_t> solve_diag(std::vector<std::array<int, 2>> points_, std::vector<std::array<rect_t, 2>> rects) {
	std::vector<int64_t> res(points_.size());

	std::vector<std::pair<std::array<int, 2>, int64_t*>> points(points_.size());
	for (int i = 0; i < int(points.size()); i++) {
		points[i] = {points_[i], &res[i]};
	}

	auto quadrants = rects_to_quadrants(std::move(rects));
	{
		auto cur_pts = points;
		auto cur_quads = quadrants;
		for (auto& a : cur_pts) {
			a.first[1] += MAXV - a.first[0];
		}
		for (auto& a : cur_quads) {
			a.first[1] += MAXV - a.first[0];
		}
		solve_ortho_real(cur_pts, cur_quads);
	}
	{
		auto cur_pts = points;
		auto cur_quads = quadrants;
		for (auto& a : cur_pts) {
			std::swap(a.first[0], a.first[1]);
			a.first[1] += MAXV - a.first[0];
		}
		for (auto& a : cur_quads) {
			a.first[0]++;
			std::swap(a.first[0], a.first[1]);
			a.first[1] += MAXV - a.first[0];
		}
		solve_ortho_real(cur_pts, cur_quads);
	}

	return res;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M, Q; cin >> N >> M >> Q;
	// x interval and y interval (half-open)
	std::vector<rect_t> rects(N);
	for (int i = 0; i < N; i++) {
		int xlo, ylo, xhi, yhi; cin >> xlo >> ylo >> xhi >> yhi;
		rects[i][0] = {xlo, xhi+1};
		rects[i][1] = {ylo, yhi+1};
	}

	// increasing x
	std::array<std::vector<std::array<rect_t, 2>>, 2> ortho_moves;

	// increasing x and y
	std::array<std::vector<std::array<rect_t, 2>>, 2> diag_moves;

	auto do_move = [&](int d, int x, int l) {
		if (d == 0) {
			// (+1, 0)
			auto old_rect = rects[x];
			rects[x][0][0] += l;
			rects[x][0][1] += l;
			auto new_rect = rects[x];
			ortho_moves[0].push_back({old_rect, new_rect});
		} else if (d == 4) {
			// (-1, 0)
			auto old_rect = rects[x];
			rects[x][0][0] -= l;
			rects[x][0][1] -= l;
			auto new_rect = rects[x];
			std::swap(old_rect, new_rect);
			new_rect[0][0] ++;
			new_rect[0][1] ++;
			old_rect[0][0] ++;
			old_rect[0][1] ++;
			ortho_moves[0].push_back({old_rect, new_rect});
		} else if (d == 2) {
			// (0, +1)
			auto old_rect = rects[x];
			rects[x][1][0] += l;
			rects[x][1][1] += l;
			auto new_rect = rects[x];

			swap(old_rect[0], old_rect[1]);
			swap(new_rect[0], new_rect[1]);

			ortho_moves[1].push_back({old_rect, new_rect});
		} else if (d == 6) {
			// (0, -1)
			auto old_rect = rects[x];
			rects[x][1][0] -= l;
			rects[x][1][1] -= l;
			auto new_rect = rects[x];

			swap(old_rect[0], old_rect[1]);
			swap(new_rect[0], new_rect[1]);

			std::swap(old_rect, new_rect);
			new_rect[0][0] ++;
			new_rect[0][1] ++;
			old_rect[0][0] ++;
			old_rect[0][1] ++;
			ortho_moves[1].push_back({old_rect, new_rect});
		} else if (d == 1) {
			// (+1, +1)
			auto old_rect = rects[x];
			rects[x][0][0] += l;
			rects[x][0][1] += l;
			rects[x][1][0] += l;
			rects[x][1][1] += l;
			auto new_rect = rects[x];
			diag_moves[0].push_back({old_rect, new_rect});
		} else if (d == 5) {
			// (-1, -1)
			auto old_rect = rects[x];
			rects[x][0][0] -= l;
			rects[x][0][1] -= l;
			rects[x][1][0] -= l;
			rects[x][1][1] -= l;
			auto new_rect = rects[x];

			std::swap(old_rect, new_rect);

			old_rect[0][0]++;
			old_rect[0][1]++;
			old_rect[1][0]++;
			old_rect[1][1]++;

			new_rect[0][0]++;
			new_rect[0][1]++;
			new_rect[1][0]++;
			new_rect[1][1]++;

			diag_moves[0].push_back({old_rect, new_rect});
		} else if (d == 3) {
			// (-1, +1)
			auto old_rect = rects[x];
			rects[x][0][0] -= l;
			rects[x][0][1] -= l;
			rects[x][1][0] += l;
			rects[x][1][1] += l;
			auto new_rect = rects[x];

			// negate x
			old_rect[0] = {MAXV - old_rect[0][1] + 1, MAXV - old_rect[0][0] + 1};
			new_rect[0] = {MAXV - new_rect[0][1] + 1, MAXV - new_rect[0][0] + 1};

			diag_moves[1].push_back({old_rect, new_rect});
		} else if (d == 7) {
			// (+1, -1)
			auto old_rect = rects[x];
			rects[x][0][0] += l;
			rects[x][0][1] += l;
			rects[x][1][0] -= l;
			rects[x][1][1] -= l;
			auto new_rect = rects[x];

			// negate x
			old_rect[0] = {MAXV - old_rect[0][1] + 1, MAXV - old_rect[0][0] + 1};
			new_rect[0] = {MAXV - new_rect[0][1] + 1, MAXV - new_rect[0][0] + 1};

			std::swap(old_rect, new_rect);
			old_rect[0][0]++;
			old_rect[0][1]++;
			old_rect[1][0]++;
			old_rect[1][1]++;

			new_rect[0][0]++;
			new_rect[0][1]++;
			new_rect[1][0]++;
			new_rect[1][1]++;

			diag_moves[1].push_back({old_rect, new_rect});
		} else assert(false);
	};

	for (int move = 0; move < M; move++) {
		int d, x, l; cin >> d >> x >> l;
		x--;
		do_move(d, x, l);
	}
	for (int x = 0; x < N; x++) {
		do_move(0, x, 1);
	}

	std::vector<std::array<int, 2>> points(Q);
	for (auto& q : points) {
		cin >> q[0] >> q[1];
	}

	std::vector<int64_t> ans(Q);
	{
		auto r = solve_ortho(points, ortho_moves[0]);
		for (int i = 0; i < Q; i++) ans[i] += r[i];
	}
	{
		auto cur_pts = points;
		for (auto& q : cur_pts) std::swap(q[0], q[1]);
		auto r = solve_ortho(cur_pts, ortho_moves[1]);
		for (int i = 0; i < Q; i++) ans[i] += r[i];
	}
	{
		auto r = solve_diag(points, diag_moves[0]);
		for (int i = 0; i < Q; i++) ans[i] += r[i];
	}
	{
		auto cur_pts = points;
		for (auto& q : cur_pts) q[0] = MAXV - q[0];
		auto r = solve_diag(cur_pts, diag_moves[1]);
		for (int i = 0; i < Q; i++) ans[i] += r[i];
	}

	for (int i = 0; i < Q; i++) {
		cout << ans[i] << '\n';
	}

	return 0;
}
