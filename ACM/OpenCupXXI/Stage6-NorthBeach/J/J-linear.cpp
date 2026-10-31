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

template <typename T> void setmin(T& a, T b) { if (b < a) a = b; }
template <typename T> void setmax(T& a, T b) { if (b > a) a = b; }

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	for (int N, M; cin >> N >> M; ) {
		tensor<int, 2> A({N, M});
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				cin >> A[{i,j}];
			}
		}
		int V = N*M + 1;
		struct rect_t {
			int xlo, xhi, ylo, yhi;
		};
		vector<rect_t> rects(V, rect_t{N, -1, M, -1});
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				auto& r = rects[A[{i,j}]];
				setmin(r.xlo, i);
				setmax(r.xhi, i);
				setmin(r.ylo, j);
				setmax(r.yhi, j);
			}
		}

		vector<vector<int>> by_area(N*M+1);
		for (int v = 0; v < V; v++) {
			const auto& r = rects[v];
			if (r.xhi == -1) continue;
			by_area[(r.xhi - r.xlo + 1) * (r.yhi - r.ylo + 1)].push_back(v);
		}
		vector<int> q; q.reserve(V);
		for (const auto& a : by_area) {
			q.insert(q.end(), a.begin(), a.end());
		}

		int64_t ans = 0;

		// stored by axis-that-moves, lower left, then half?
		tensor<int64_t, 4> has_edge({2,N+1,M+1,2});
		for (auto v : q) {
			const auto& r = rects[v];
			for (int i = r.xlo; i <= r.xhi; i++) {
				has_edge[{0, i, r.ylo, 0}] += v;
				has_edge[{0, i, r.yhi+1, 0}] += v;
			}
			for (int j = r.ylo; j <= r.yhi; j++) {
				has_edge[{1, r.xlo, j, 0}] += v;
				has_edge[{1, r.xhi+1, j, 0}] += v;
			}
		}

		for (auto v : q) {
			const auto& r = rects[v];
			for (int i = r.xlo; i <= r.xhi; i++) {
				has_edge[{0, i, r.ylo, 0}] -= v;
				has_edge[{0, i, r.yhi+1, 0}] -= v;
			}
			for (int j = r.ylo; j <= r.yhi; j++) {
				has_edge[{1, r.xlo, j, 0}] -= v;
				has_edge[{1, r.xhi+1, j, 0}] -= v;
			}

			int64_t res = 0;
			for (int i = r.xlo; i <= r.xhi; i++) {
				if (r.ylo >= 1) {
					res += has_edge[{1, i, r.ylo-1, 1}];
				}
				res += has_edge[{1, i, r.yhi, 1}];
			}
			for (int i = r.xlo+1; i <= r.xhi; i++) {
				res += has_edge[{1, i, r.ylo, 0}];
				res += has_edge[{1, i, r.yhi, 0}];
			}
			for (int j = r.ylo; j <= r.yhi; j++) {
				if (r.xlo >= 1) {
					res += has_edge[{0, r.xlo-1, j, 1}];
				}
				res += has_edge[{0, r.xhi, j, 1}];
			}
			for (int j = r.ylo+1; j <= r.yhi; j++) {
				res += has_edge[{0, r.xlo, j, 0}];
				res += has_edge[{0, r.xhi, j, 0}];
			}

			assert(res % 2 == 0);
			res /= 2;

			ans += v ^ res;

			for (int i = r.xlo; i <= r.xhi-1; i++) {
				has_edge[{0, i, r.ylo, 1}] += v;
				has_edge[{0, i, r.yhi, 1}] += v;
			}
			for (int j = r.ylo; j <= r.yhi-1; j++) {
				has_edge[{1, r.xlo, j, 1}] += v;
				has_edge[{1, r.xhi, j, 1}] += v;
			}
		}
		cout << ans << '\n';
	}

	return 0;
}
