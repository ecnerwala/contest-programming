#include <bits/stdc++.h>

template <typename T, int NDIMS> struct tensor_view {
	static_assert(NDIMS >= 0, "NDIMS must be nonnegative");

public:
	std::array<int, NDIMS> shape;
protected:
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

public:
	std::array<int, NDIMS> shape;
protected:
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
	std::vector<int64_t> seg;
	seg_1d() {}
	void set_size(int N) {
		seg = std::vector<int64_t>(N, 0);
	};
	void incr(int i, int v) {
		for (int a = i; a > 0; a &= a-1) {
			seg[a-1] += v;
		}
	}
	void pre_init_set(int i, int v) {
		// for_prefix
		if (i > 0) seg[i-1] += v;
	}
	void init() {
		for (int i = int(seg.size()); i > 0; i--) {
			if (i & (i-1)) seg[(i&(i-1)) - 1] += seg[i-1];
		}
	}
	int64_t query_suff(int i) {
		int64_t res = 0;
		for (int a = i; a < int(seg.size()); a |= a+1) {
			res += seg[a];
		}
		return res;
	}
};

struct seg_2d {
	tensor<int64_t, 2> seg;
	seg_2d() {}
	void set_size(int N, int M) {
		seg = tensor<int64_t, 2>({N, M}, 0);
	}
	void pre_init_set(int i, int j, int v) {
		if (i > 0 && j > 0) seg[{i-1,j-1}] += v;
	}
	void init() {
		for (int i = int(seg.shape[0]); i > 0; i--) {
			auto cur = seg[i-1];
			if (i & (i-1)) {
				auto nxt = seg[(i&(i-1)) - 1];
				for (int j = int(cur.shape[0]); j > 0; j--) {
					*nxt[j-1] += *cur[j-1];
				}
			}
			for (int j = int(cur.shape[0]); j > 0; j--) {
				if (j & (j-1)) *cur[(j&(j-1)) - 1] += *cur[j-1];
			}
		}
	}
	void incr(int i, int j, int v) {
		for (int a = i; a > 0; a &= a-1) {
			for (int b = j; b > 0; b &= b-1) {
				seg[{a-1, b-1}] += v;
			}
		}
	}
	int64_t query_suff(int i, int j) {
		int64_t res = 0;
		for (int a = i; a < int(seg.shape[0]); a |= a+1) {
			for (int b = j; b < int(seg.shape[1]); b |= b+1) {
				res += seg[{a, b}];
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
