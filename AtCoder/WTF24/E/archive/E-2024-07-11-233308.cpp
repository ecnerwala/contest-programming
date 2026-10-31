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
#ifdef _GLIBCXX_DEBUG
		return data[flatten_index_checked(idx)];
#else
		return data[flatten_index(idx)];
#endif
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
public:
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

template <typename T> void setmin(T& a, T b) { if (b < a) a = b; }
template <typename T> void setmax(T& a, T b) { if (b > a) a = b; }

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, K; cin >> N >> K;
		using range_t = std::array<int, 2>;
		using rect_t = std::array<range_t, 2>;
		tensor<std::optional<rect_t>, 2> rects({N+1, N+1}, std::nullopt);
		std::vector<std::pair<std::array<int, 2>, rect_t>> ops; ops.reserve(K);
		for (int k = 0; k < K; k++) {
			int h, w, a, b; cin >> h >> w >> a >> b; a--, b--;
			rect_t r{range_t{a, a+h}, range_t{b, b+w}};
			ops.push_back({std::array<int, 2>{h, w}, r});
			rects[{h, w}] = r;
		}
		std::reverse(ops.begin(), ops.end());
		struct symb_interval_t {
			int len;
			range_t inner;
			range_t outer;
		};
		// add inner restriction
		auto update_inner = [&](symb_interval_t& a, range_t b) {
			assert(b[1] - b[0] <= a.len);
			setmin(a.inner[0], b[0]);
			setmax(a.inner[1], b[1]);
		};
		auto update_outer = [&](symb_interval_t& a, range_t b) {
			assert(b[1] - b[0] >= a.len);
			setmax(a.inner[0], b[0]);
			setmin(a.inner[1], b[1]);
		};
		std::vector<std::array<symb_interval_t, 2>> inters; inters.reserve(N);
		for (int h = 1; h <= N; h++) {
			for (int w = 1; w <= N; w++) {
				if (!rects[{h, w}] && (h+1>N || rects[{h+1, w}]) && (w+1>N || rects[{h, w+1}])) {
					symb_interval_t sh{h, {N, 0}, {0, N}};
					symb_interval_t sw{w, {N, 0}, {0, N}};
					// extremal point
					for (auto op : ops) {
						auto [op_h, op_w] = op.first;
						auto [op_rh, op_rw] = op.second;
						assert(op_h > h || op_w > w);
						assert(op_h <= h || op_w <= w); 
						if (op_h > h) {
							update_outer(sh, op_rh);
							update_inner(sw, op_rw);
						} else if (op_w > w) {
							update_outer(sw, op_rw);
							update_inner(sh, op_rh);
						} else assert(false);

						if (op_h >= h && op_w >= w) {
							assert(op_h == h+1 || op_w == w+1);
							break;
						}
					}
				}
			}
		}
		// increasing h, decreasing w
		for (int z = 0; z+1 < int(inters.size()); z++) {
			auto& a = inters[z];
			auto& b = inters[z+1];
			assert(a[0].len < b[0].len);
			update_inner(b[0], a[0].inner);
			assert(a[1].len > b[1].len);
			update_outer(b[1], a[1].outer);
		}
		for (int z = int(inters.size()) - 2; z >= 0; z--) {
			auto& a = inters[z];
			auto& b = inters[z+1];
			assert(a[0].len < b[0].len);
			update_outer(a[0], b[0].outer);
			assert(a[1].len > b[1].len);
			update_inner(b[1], a[1].inner);
		}
	}

	return 0;
}
