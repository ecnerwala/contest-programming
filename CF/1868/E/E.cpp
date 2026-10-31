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

template <typename T> void setmax(T& a, T b) { if (b > a) a = b; }

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int64_t> A(N); for (auto& a : A) cin >> a;
		std::vector<int64_t> pref(N+1);
		for (int i = 0; i < N; i++) pref[i+1] = pref[i] + A[i];
		std::vector<int64_t> vals = pref;
		std::sort(vals.begin(), vals.end());
		vals.resize(std::unique(vals.begin(), vals.end()) - vals.begin());
		std::vector<std::vector<int>> inds(vals.size());
		for (int i = 0; i <= N; i++) {
			inds[int(std::lower_bound(vals.begin(), vals.end(), pref[i]) - vals.begin())].push_back(i);
		}

		std::vector<std::pair<int64_t, std::array<int, 2>>> diffs;
		for (int i = 0; i < int(vals.size()); i++) {
			for (int j = i+1; j < int(vals.size()); j++) {
				diffs.push_back({vals[j] - vals[i], {i, j}});
			}
		}
		std::sort(diffs.begin(), diffs.end());

		int ans = 0;
		std::vector<int> dp_end(N+1, 0);
		std::vector<int> dp_start(N+1, 0);

		tensor<int, 2> dp_low({N+1, N+1}, -1);
		tensor<int, 2> dp_high({N+1, N+1}, -1);
		for (int i = 0; i <= N; i++) {
			dp_low[{i, i}] = 0;
			dp_high[{i, i}] = 0;
		}
		for (const auto& a : inds) {
			for (int i = 0; i < int(a.size()); i++) {
				for (int j = i+1; j < int(a.size()); j++) {
					dp_low[{a[i], a[j]}] = 1;
					dp_high[{a[i], a[j]}] = 1;
				}
			}
		}

		std::vector<std::pair<int, bool>> merged_inds; merged_inds.reserve(N+1);
		for (auto [_, cur_vals] : diffs) {
			auto [v_lo, v_hi] = cur_vals;
			const auto& inds_lo = inds[v_lo];
			const auto& inds_hi = inds[v_hi];
			merged_inds.clear();
			for (int a = 0, b = 0; a < int(inds_lo.size()) || b < int(inds_hi.size()); ) {
				if (a < int(inds_lo.size()) && (b == int(inds_hi.size()) || inds_lo[a] < inds_hi[b])) {
					merged_inds.push_back({inds_lo[a++], false});
					a++;
				} else {
					merged_inds.push_back({inds_hi[b++], true});
					b++;
				}
			}
		}
		cout << ans << '\n';
	}

	return 0;
}

// +a -b +c
// if c >= a, we need a <= b
