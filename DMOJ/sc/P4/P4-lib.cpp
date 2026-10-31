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

template <typename T, int NDIMS> struct tensor_view {
	static_assert(NDIMS >= 0, "NDIMS must be nonnegative");

protected:
	std::array<int, NDIMS> shape;
	std::array<int, NDIMS> strides;
	T* data;

	tensor_view(std::array<int, NDIMS> shape_, std::array<int, NDIMS> strides_, T* data_) : shape(shape_), strides(strides_), data(data_) {}

public:
	tensor_view() : shape{0}, strides{0}, data(nullptr) {}

protected:
	int flatten_index(std::array<int, NDIMS> idx) const {
		int res = 0;
		for (int i = 0; i < NDIMS; i++) { res += idx[i] * strides[i]; }
		return res;
	}
	int flatten_index_checked(std::array<int, NDIMS> idx) const {
		int res = 0;
		for (int i = 0; i < NDIMS; i++) {
			assert(0 <= idx[i] && idx[i] < shape[i]);
			res += idx[i] * strides[i];
		}
		return res;
	}

public:
	T& operator[] (std::array<int, NDIMS> idx) const {
		return data[flatten_index(idx)];
	}
	T& at(std::array<int, NDIMS> idx) const {
		return data[flatten_index_checked(idx)];
	}

	template <int D = NDIMS>
	typename std::enable_if<(0 < D), tensor_view<T, NDIMS-1>>::type operator[] (int idx) const {
		std::array<int, NDIMS-1> nshape; std::copy(shape.begin()+1, shape.end(), nshape.begin());
		std::array<int, NDIMS-1> nstrides; std::copy(strides.begin()+1, strides.end(), nstrides.begin());
		T* ndata = data + (strides[0] * idx);
		return tensor_view<T, NDIMS-1>(nshape, nstrides, ndata);
	}
	template <int D = NDIMS>
	typename std::enable_if<(0 < D), tensor_view<T, NDIMS-1>>::type at(int idx) const {
		assert(0 <= idx && idx < shape[0]);
		return operator[](idx);
	}

	template <int D = NDIMS>
	typename std::enable_if<(0 == D), T&>::type operator * () const {
		return *data;
	}

	template <typename U, int D> friend struct tensor_view;
	template <typename U, int D> friend struct tensor;
};

template <typename T, int NDIMS> struct tensor {
	static_assert(NDIMS >= 0, "NDIMS must be nonnegative");

protected:
	std::array<int, NDIMS> shape;
	std::array<int, NDIMS> strides;
	int len;
	T* data;

public:
	tensor() : shape{0}, strides{0}, len(0), data(nullptr) {}

	explicit tensor(std::array<int, NDIMS> shape_, const T& t = T()) {
		shape = shape_;
		len = 1;
		for (int i = NDIMS-1; i >= 0; i--) {
			strides[i] = len;
			len *= shape[i];
		}
		data = new T[len];
		std::fill(data, data + len, t);
	}

	tensor(const tensor& o) : shape(o.shape), strides(o.strides), len(o.len), data(new T[len]) {
		for (int i = 0; i < len; i++) {
			data[i] = o.data[i];
		}
	}

	tensor& operator=(tensor&& o) noexcept {
		using std::swap;
		swap(shape, o.shape);
		swap(strides, o.strides);
		swap(len, o.len);
		swap(data, o.data);
		return *this;
	}
	tensor(tensor&& o) : tensor() {
		*this = std::move(o);
	}
	tensor& operator=(const tensor& o) {
		return *this = tensor(o);
	}
	~tensor() { delete[] data; }

	using view_t = tensor_view<T, NDIMS>;
	view_t view() {
		return tensor_view<T, NDIMS>(shape, strides, data);
	}
	operator view_t() {
		return view();
	}

	using const_view_t = tensor_view<const T, NDIMS>;
	const_view_t view() const {
		return tensor_view<const T, NDIMS>(shape, strides, data);
	}
	operator const_view_t() const {
		return view();
	}

	T& operator[] (std::array<int, NDIMS> idx) { return view()[idx]; }
	T& at(std::array<int, NDIMS> idx) { return view().at(idx); }
	const T& operator[] (std::array<int, NDIMS> idx) const { return view()[idx]; }
	const T& at(std::array<int, NDIMS> idx) const { return view().at(idx); }

	template <int D = NDIMS>
	typename std::enable_if<(0 < D), tensor_view<T, NDIMS-1>>::type operator[] (int idx) {
		return view()[idx];
	}
	template <int D = NDIMS>
	typename std::enable_if<(0 < D), tensor_view<T, NDIMS-1>>::type at(int idx) {
		return view().at(idx);
	}

	template <int D = NDIMS>
	typename std::enable_if<(0 < D), tensor_view<const T, NDIMS-1>>::type operator[] (int idx) const {
		return view()[idx];
	}
	template <int D = NDIMS>
	typename std::enable_if<(0 < D), tensor_view<const T, NDIMS-1>>::type at(int idx) const {
		return view().at(idx);
	}

	template <int D = NDIMS>
	typename std::enable_if<(0 == D), T&>::type operator * () {
		return *view();
	}
	template <int D = NDIMS>
	typename std::enable_if<(0 == D), const T&>::type operator * () const {
		return *view();
	}
};

struct seg_1d {
	binary_indexed_tree<int64_t> seg;
	seg_1d() {}
	void set_size(int N) {
		seg = binary_indexed_tree<int64_t>(N, 0);
	};
	void incr(int i, int v) {
		for (auto& a : seg.prefix(i)) {
			a += v;
		}
	}
	void pre_init_set(int i, int v) {
		// for_prefix
		if (i > 0) seg.data()[i-1] += v;
	}
	void init() {
		for (int i = int(seg.size()); i > 0; i--) {
			if (i & (i-1)) seg.data()[(i&(i-1)) - 1] += seg.data()[i-1];
		}
	}
	int64_t query_suff(int i) {
		int64_t res = 0;
		for (const auto& a : seg.suffix(i)) {
			res += a;
		}
		return res;
	}
};

struct seg_2d {
	binary_indexed_tree<binary_indexed_tree<int64_t>> seg;
	seg_2d() {}
	void set_size(int N, int M) {
		seg = binary_indexed_tree<binary_indexed_tree<int64_t>>(N, binary_indexed_tree<int64_t>(M, 0));
	}
	void pre_init_set(int i, int j, int v) {
		if (i > 0 && j > 0) seg.data()[i-1].data()[j-1] += v;
	}
	void init() {
		for (int i = int(seg.size()); i > 0; i--) {
			auto& cur = seg.data()[i-1];
			if (i & (i-1)) {
				auto& nxt = seg.data()[(i&(i-1)) - 1];
				for (int j = int(cur.size()); j > 0; j--) {
					nxt.data()[j-1] += cur.data()[j-1];
				}
			}
			for (int j = int(cur.size()); j > 0; j--) {
				if (j & (j-1)) cur.data()[(j&(j-1)) - 1] += cur.data()[j-1];
			}
		}
	}
	void incr(int i, int j, int v) {
		for (auto& a : seg.prefix(i)) {
			for (auto& b : a.prefix(j)) {
				b += v;
			}
		}
	}
	int64_t query_suff(int i, int j) {
		int64_t res = 0;
		for (const auto& a : seg.suffix(i)) {
			for (const auto& b : a.suffix(j)) {
				res += b;
			}
		}
		return res;
	}
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M, Q; cin >> N >> M >> Q;

	// M - 1 - c <= M - 1 - c0
	// r <= r0
	// c - r + N - 1 <= c0 - r0 + h - 1 + N - 1
	auto get_coords = [&](int r, int c) -> std::array<int, 3> {
		return {r, M-1-c, c - r + (N-1)};
	};
	auto get_query_coords = [&](int r, int c, int h) -> std::array<int, 3> {
		return {r, M-1-c, c - r + (h - 1) + (N - 1)};
	};

	tensor<int, 2> G({N, M}, 0);

	int64_t tot_g = 0;
	std::array<seg_1d, 3> seg_1;
	std::array<seg_2d, 3> seg_2;
	{
		std::array<int, 3> sizes({N, M, N+M-1});
		for (int z = 0; z < 3; z++) {
			seg_1[z].set_size(sizes[z]);
		}
		for (int z = 0; z < 3; z++) {
			seg_2[z].set_size(sizes[z], sizes[(z+1)%3]);
		}
	}

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			int g; cin >> g;
			G[{i, j}] = g;
			auto coords = get_coords(i, j);
			tot_g += g;
			for (int z = 0; z < 3; z++) {
				seg_1[z].pre_init_set(coords[z], g);
			}
			for (int z = 0; z < 3; z++) {
				seg_2[z].pre_init_set(coords[z], coords[(z+1)%3], g);
			}
		}
	}

	for (int z = 0; z < 3; z++) {
		seg_1[z].init();
	}
	for (int z = 0; z < 3; z++) {
		seg_2[z].init();
	}

	for (int q = 0; q < Q; q++) {
		int op; cin >> op;
		if (op == 1) {
			int r, c, v; cin >> r >> c >> v; r--, c--;
			v -= std::exchange(G[{r, c}], v);

			auto coords = get_coords(r, c);
			tot_g += v;
			for (int z = 0; z < 3; z++) {
				seg_1[z].incr(coords[z], v);
			}
			for (int z = 0; z < 3; z++) {
				seg_2[z].incr(coords[z], coords[(z+1)%3], v);
			}
		} else if (op == 2) {
			int r, c, h; cin >> r >> c >> h; r--, c--;
			auto coords = get_query_coords(r, c, h);

			int64_t ans = tot_g;
			for (int z = 0; z < 3; z++) {
				ans -= seg_1[z].query_suff(coords[z]);
			}
			for (int z = 0; z < 3; z++) {
				ans += seg_2[z].query_suff(coords[z], coords[(z+1)%3]);
			}
			// nothing violates all 3
			cout << ans << '\n';
		}
	}

	return 0;
}
