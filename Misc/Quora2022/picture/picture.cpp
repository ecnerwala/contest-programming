#include <bits/stdc++.h>

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
		strides[NDIMS-1] = 1;
		for (int i = NDIMS-1; i > 0; i--) {
			strides[i-1] = strides[i] * shape[i];
		}
		len = strides[0] * shape[0];
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
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<std::pair<int, int>> Xs(N);
	std::vector<std::pair<int, int>> Ys(N);
	for (int i = 0; i < N; i++) {
		int x, y; cin >> x >> y;
		Xs[i] = {x, i};
		Ys[i] = {y, i};
	}
	std::vector<std::array<int, 2>> P(N);
	int X = 0;
	std::sort(Xs.begin(), Xs.end());
	for (int st = 0, en = 0; st < N; st = en) {
		while (en < N && Xs[en].first == Xs[st].first) {
			P[Xs[en].second][0] = X;
			en++;
		}
		X++;
	}
	int Y = 0;
	std::sort(Ys.begin(), Ys.end());
	for (int st = 0, en = 0; st < N; st = en) {
		while (en < N && Ys[en].first == Ys[st].first) {
			P[Ys[en].second][1] = Y;
			en++;
		}
		Y++;
	}

	std::vector<std::pair<std::array<int, 2>, int>> pairs; pairs.reserve(N*(N-1)/2);
	for (int i = 0; i < N; i++) {
		for (int j = i+1; j < N; j++) {
			if (P[i][0] == P[j][0]) {
				pairs.emplace_back(std::array<int, 2>{std::min(P[i][1], P[j][1]), std::max(P[i][1], P[j][1])}, P[i][0]);
			}
		}
	}
	std::sort(pairs.begin(), pairs.end());

	tensor<int, 2> horiz_pref({X, Y});
	tensor<int, 2> vert_pref({Y, X});
	for (int z = 0; z+1 < int(pairs.size()); z++) {
		if (pairs[z].first == pairs[z+1].first) {
			int x0 = pairs[z].second, x1 = pairs[z+1].second;
			auto [y0, y1] = pairs[z].first;
			assert(x0 <= x1);
			assert(y0 <= y1);
			horiz_pref[{x0, y0}]++;
			horiz_pref[{x0, y1}]--;
			horiz_pref[{x1, y0}]++;
			horiz_pref[{x1, y1}]--;

			vert_pref[{y0, x0}]++;
			vert_pref[{y0, x1}]--;
			vert_pref[{y1, x0}]++;
			vert_pref[{y1, x1}]--;
		}
	}

	std::vector<std::pair<int, std::array<int, 2>>> horiz_segs; horiz_segs.reserve(N);
	for (int x = 0; x < X; x++) {
		int cur = 0;
		for (int y = 0; y < Y; y++) {
			int nxt = cur + horiz_pref[{x, y}];
			if (cur != 0 && nxt == 0) {
				horiz_segs.back().second[1] = y;
			}
			if (cur == 0 && nxt != 0) {
				horiz_segs.emplace_back(x, std::array<int, 2>{y, y});
			}
			cur = nxt;
		}
		assert(cur == 0);
	}

	std::vector<std::pair<int, std::array<int, 2>>> vert_segs; vert_segs.reserve(N);
	for (int y = 0; y < Y; y++) {
		int cur = 0;
		for (int x = 0; x < X; x++) {
			int nxt = cur + vert_pref[{y, x}];
			if (cur != 0 && nxt == 0) {
				vert_segs.back().second[1] = x;
			}
			if (cur == 0 && nxt != 0) {
				vert_segs.emplace_back(y, std::array<int, 2>{x, x});
			}
			cur = nxt;
		}
		assert(cur == 0);
	}

	// sort them decreasing by right endpoint
	std::sort(vert_segs.begin(), vert_segs.end(), [&](auto a, auto b) {
		return a.second[1] > b.second[1];
	});

	int64_t ans = 0;

	for (int i = 0; i < int(horiz_segs.size()); i++) {
		auto it = vert_segs.begin();
		binary_indexed_tree<int> bit(Y);
		for (int j = int(horiz_segs.size())-1; j > i; j--) {
			if (horiz_segs[j].first <= horiz_segs[i].first) break;

			for (; it != vert_segs.end() && it->second[1] >= horiz_segs[j].first; ++it) {
				if (it->second[0] <= horiz_segs[i].first && horiz_segs[i].second[0] <= it->first && it->first <= horiz_segs[i].second[1]) {
					// insert it
					for (auto& a : bit.suffix(it->first)) a++;
				}
			}
			int cnt = 0;
			for (auto a : bit.prefix(horiz_segs[j].second[1]+1)) cnt += a;
			for (auto a : bit.prefix(horiz_segs[j].second[0])) cnt -= a;
			ans += int64_t(cnt) * int64_t(cnt-1) / 2;
		}
	}

	cout << ans << '\n';

	return 0;
}
