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

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N, Q; std::cin >> N >> Q;
		std::vector<int> A(N);
		for (auto& a : A) std::cin >> a;
		// double inclusive
		std::vector<std::array<std::array<int, 2>, 2>> rects; rects.reserve(3 * N);
		std::vector<int> prv_smaller(N);
		std::vector<int> nxt_smaller(N);
		// find the next thing strictly smaller on each side
		for (int z = 0; z < 2; z++, std::reverse(A.begin(), A.end())) {
			struct stk_t {
				int val;
				int idx;
				std::array<int, 2> l_bounds;
			};
			auto add_rect = [&](std::array<int, 2> ls, std::array<int, 2> rs) -> void {
				if (ls[1] < ls[0]) return;
				if (rs[1] < rs[0]) return;
				assert(ls[1] <= rs[0]);
				if (z) {
					std::swap(ls, rs);
					ls = {N-1-ls[0], N-1-ls[1]};
					rs = {N-1-rs[0], N-1-rs[1]};
					assert(ls[1] <= rs[0]);
				}
				rects.push_back({ls, rs});
			};
			std::vector<stk_t> stk; stk.reserve(N);
			for (int i = 0; i < N; i++) {
				while (!stk.empty() && stk.back().val >= A[i]) {
					// remove them at i, and also add their thing
					add_rect(stk.back().l_bounds, {stk.back().idx, i-1});
					stk.pop_back();
				}
				prv_smaller[i] = stk.empty() ? -1 : stk.back().idx;
				// we're looking for a stack element strictly bigger
				std::array<int, 2> l_bounds{0, -1};
				if (int(stk.size()) >= 2) {
					int v = 2 * stk.back().val - A[i];
					assert(v < stk.back().val);
					int mi = -1, ma = int(stk.size()) - 1;
					while (ma - mi > 1) {
						int md = (mi + ma) / 2;
						if (stk[md].val >= v) ma = md;
						else mi = md;
					}
					std::cerr << "val " << stk.back().val << A[i] << '\n';
					l_bounds = {mi == -1 ? 0 : stk[mi].idx + 1, stk.end()[-2].idx};
				}
				stk.push_back({A[i], i, l_bounds});
			}
			while (!stk.empty()) {
				add_rect(stk.back().l_bounds, {stk.back().idx, N-1});
				stk.pop_back();
			}

			std::swap(prv_smaller, nxt_smaller);
		}
		std::reverse(nxt_smaller.begin(), nxt_smaller.end());
		for (int i = 0; i < N; i++) {
			nxt_smaller[i] = N-1-nxt_smaller[i];
		}

		for (int i = 0; i < N; i++) {
			assert(prv_smaller[i] < i);
			assert(i < nxt_smaller[i]);
			rects.push_back({std::array<int, 2>{prv_smaller[i]+1, i-1}, std::array<int, 2>{i+1, nxt_smaller[i]-1}});
		}

		std::vector<std::vector<std::tuple<int, int, int>>> ops(N+1);
		for (auto r : rects) {
			//assert(r[0][0] <= r[0][1] && r[0][1] <= r[1][0] && r[1][0] <= r[1][1]);
			std::cerr << r[0][0] << '-' << r[0][1] << ' ' << r[1][0] << '-' << r[1][1] << '\n';
			r[0][1]++, r[1][1]++;
			ops[r[0][0]].push_back({r[1][0], -1, +1});
			ops[r[0][0]].push_back({r[1][1], -1, -1});
			ops[r[0][1]].push_back({r[1][0], -1, -1});
			ops[r[0][1]].push_back({r[1][1], -1, +1});
		}

		std::vector<int64_t> ans(Q);
		for (int q = 0; q < Q; q++) {
			int l, r; std::cin >> l >> r; l--;
			int len = r - l;
			assert(len >= 3);
			ans[q] = int64_t(len) * int64_t(len - 1) * int64_t(len - 2) / 6;
			ops[l].push_back({l, q, +1});
			ops[l].push_back({r, q, -1});
			ops[r].push_back({l, q, -1});
			ops[r].push_back({r, q, +1});
		}

		binary_indexed_tree<std::array<int64_t, 4>> bit(N+1);
		for (int x = 0; x <= N; x++) {
			for (auto [y, o, w] : ops[x]) {
				if (o == -1) {
					// we're inserting
					for (auto& v : bit.suffix(y)) {
						v[0] += w;
						v[1] += x * w;
						v[2] += y * w;
						v[3] += int64_t(x) * int64_t(y) * w;
					}
				} else {
					assert(0 <= o && o < Q);
					int64_t r = 0;
					for (const auto& v : bit.prefix(y)) {
						r += v[0] * int64_t(x) * int64_t(y);
						r -= v[1] * int64_t(y);
						r -= v[2] * int64_t(x);
						r += v[3];
					}
					r *= w;
					ans[o] -= r;
				}
			}
		}
		for (auto a : ans) std::cout << a << '\n';
	}

	return 0;
}
