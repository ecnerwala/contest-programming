#include<bits/stdc++.h>

template <typename T, int NDIMS> struct tensor_view {
	static_assert(NDIMS >= 0);

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
	std::enable_if_t<(0 < D), tensor_view<T, NDIMS-1>> operator[] (int idx) const {
		std::array<int, NDIMS-1> nshape; std::copy(shape.begin()+1, shape.end(), nshape.begin());
		std::array<int, NDIMS-1> nstrides; std::copy(strides.begin()+1, strides.end(), nstrides.begin());
		T* ndata = data + (strides[0] * idx);
		return tensor_view<T, NDIMS-1>(nshape, nstrides, ndata);
	}
	template <int D = NDIMS>
	std::enable_if_t<(0 < D), tensor_view<T, NDIMS-1>> at(int idx) const {
		assert(0 <= idx && idx < shape[0]);
		return operator[](idx);
	}

	template <int D = NDIMS>
	std::enable_if_t<(0 == D), T&> operator * () const {
		return *data;
	}

	template <typename U, int D> friend struct tensor_view;
	template <typename U, int D> friend struct tensor;
};

template <typename T, int NDIMS> struct tensor {
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
	std::enable_if_t<(0 < D), tensor_view<T, NDIMS-1>> operator[] (int idx) {
		return view()[idx];
	}
	template <int D = NDIMS>
	std::enable_if_t<(0 < D), tensor_view<T, NDIMS-1>> at(int idx) {
		return view().at(idx);
	}

	template <int D = NDIMS>
	std::enable_if_t<(0 < D), tensor_view<const T, NDIMS-1>> operator[] (int idx) const {
		return view()[idx];
	}
	template <int D = NDIMS>
	std::enable_if_t<(0 < D), tensor_view<const T, NDIMS-1>> at(int idx) const {
		return view().at(idx);
	}

	template <int D = NDIMS>
	std::enable_if_t<(0 == D), T&> operator * () {
		return *view();
	}
	template <int D = NDIMS>
	std::enable_if_t<(0 == D), const T&> operator * () const {
		return *view();
	}
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int N, L; cin >> N >> L;
	vector<int> A(N); for (auto& a : A) cin >> a;
	int Q; cin >> Q; vector<int> queries(Q); for (auto& q : queries) cin >> q;

	vector<int> cutoffs = queries;
	cutoffs.push_back(-1); cutoffs.push_back(L);
	sort(cutoffs.begin(), cutoffs.end());
	cutoffs.resize(unique(cutoffs.begin(), cutoffs.end()) - cutoffs.begin());

	tensor<int, 2> bucket_cnt({int(cutoffs.size()), N-L+1});
	for (int d = 1; d < N-L+1; d++) {
		int s = 0;
		int b = 1;
		for (int i = 0; i < L-1; i++) {
			s += A[i] != A[i+d];
			if (s > cutoffs[b]) b++;
		}
		for (int i = 0, j = d; j < N-L+1; i++, j++) {
			// insert the last guy
			s += A[i+L-1] != A[j+L-1];
			if (s > cutoffs[b]) b++;
			
			bucket_cnt[{b,i}]++;
			bucket_cnt[{b,j}]++;

			// delete the first guy
			s -= A[i] != A[j];
			if (s <= cutoffs[b-1]) --b;
		}
	}
	
	for (int b = 0; b+1 < int(cutoffs.size()); b++) {
		for (int i = 0; i < N-L+1; i++) {
			bucket_cnt[{b+1, i}] += bucket_cnt[{b, i}];
		}
	}

	for (int k : queries) {
		int b = int(lower_bound(cutoffs.begin(), cutoffs.end(), k) - cutoffs.begin());
		for (int i = 0; i < N-L+1; i++) {
			cout << bucket_cnt[{b, i}] << " \n"[i+1==N-L+1];
		}
	}

	return 0;
}
