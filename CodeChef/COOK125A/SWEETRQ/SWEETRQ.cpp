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

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	tensor<int, 2> A({N,M});
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			cin >> A[{i,j}];
		}
	}

	tensor<array<int, 2>, 2> idx({N,M});
	for (int i = 0; i < N; i++) {
		vector<pair<int, int>> vals(M);
		for (int j = 0; j < M; j++) {
			vals[j] = {A[{i,j}], j};
		}
		sort(vals.begin(), vals.end());
		for (int j = 0; j < M; j++) {
			idx[{i, vals[j].second}][0] = j;
		}
	}

	for (int j = 0; j < M; j++) {
		vector<pair<int, int>> vals(N);
		for (int i = 0; i < N; i++) {
			vals[i] = {A[{i,j}], i};
		}
		sort(vals.begin(), vals.end());
		for (int i = 0; i < N; i++) {
			idx[{vals[i].second, j}][1] = i;
		}
	}

	int64_t cur_ans = -int64_t(N * (N-1) / 2) * int64_t(M * (M-1) / 2);
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			cur_ans += int64_t(idx[{i,j}][0]) * int64_t(idx[{i,j}][1]);
		}
	}

	cout << cur_ans << '\n';

	int Q; cin >> Q;
	for (int q = 0; q < Q; q++) {
		int x, y, h; cin >> x >> y >> h; x--, y--;
		int oh = A[{x,y}];
		A[{x,y}] = h;
		cur_ans -= int64_t(idx[{x,y}][0]) * int64_t(idx[{x,y}][1]);
		idx[{x,y}][0] = 0;
		idx[{x,y}][1] = 0;
		for (int j = 0; j < M; j++) {
			if (j == y) continue;
			cur_ans -= int64_t(idx[{x,j}][0]) * int64_t(idx[{x,j}][1]);
			idx[{x,j}][0] -= (A[{x,j}] > oh);
			idx[{x,j}][0] += (A[{x,j}] > h);
			cur_ans += int64_t(idx[{x,j}][0]) * int64_t(idx[{x,j}][1]);

			idx[{x,y}][0] += h > A[{x,j}];
		}
		for (int i = 0; i < N; i++) {
			if (i == x) continue;
			cur_ans -= int64_t(idx[{i,y}][0]) * int64_t(idx[{i,y}][1]);
			idx[{i,y}][1] -= (A[{i,y}] > oh);
			idx[{i,y}][1] += (A[{i,y}] > h);
			cur_ans += int64_t(idx[{i,y}][0]) * int64_t(idx[{i,y}][1]);

			idx[{x,y}][1] += h > A[{i,y}];
		}
		cur_ans += int64_t(idx[{x,y}][0]) * int64_t(idx[{x,y}][1]);

		cout << cur_ans << '\n';
	}

	return 0;
}

// Main idea: sweet rectangle has 2 pairs of the form 
//   a[r][c] > a[r'][c] and a[r][c']
// a nonsweet rectangle has 1 pair
//
// To count this, we can take sum_{cells} (# smaller in row) * (# smaller in col)
