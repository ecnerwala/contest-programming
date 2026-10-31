#include<bits/stdc++.h>

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

	int N; cin >> N;
	vector<pair<int, int>> A(N+2);
	for (int i = 0; i < N; i++) {
		cin >> A[i].first; A[i].second = i+2;
	}
	A[N] = {0,0};
	A[N+1] = {N+1,1};
	sort(A.begin(), A.end());

	// closed interval of times, now 2-indexed
	int L, R; cin >> L >> R; L++, R++;

	vector<int> prev_earlier(N+2);
	vector<int> next_earlier(N+2);
	{
		vector<int> stk; stk.reserve(N+2);
		stk.push_back(0);
		for (int i = 1; i <= N; i++) {
			while (A[stk.back()].second > A[i].second) stk.pop_back();
			prev_earlier[i] = stk.back();
			stk.push_back(i);
		}
	}
	{
		vector<int> stk; stk.reserve(N+2);
		stk.push_back(N+1);
		for (int i = N; i >= 1; i--) {
			while (A[stk.back()].second > A[i].second) stk.pop_back();
			next_earlier[i] = stk.back();
			stk.push_back(i);
		}
	}

	int64_t ans = 0;
	for (int i = 1; i <= N; i++) {
		int t = A[i].second;
		if (L <= t && t <= R) {
			continue;
		}
		ans += next_earlier[i] - prev_earlier[i] - 1;
	}

	const int NUM_MOVE = R - L + 1;
	vector<int> cur_locs; cur_locs.reserve(NUM_MOVE+2);
	tensor<int64_t, 2> dp({NUM_MOVE+2, NUM_MOVE+2});

	const int64_t INF = 1e18;

	for (int lo = 0, hi; lo <= N; lo = hi) {
		assert(A[lo].second < L);
		hi = lo+1;
		while (!(A[hi].second < L)) hi++;

		cur_locs.clear();
		for (int i = lo; i <= hi; i++) {
			if (A[i].second <= R) {
				cur_locs.push_back(i);
			}
		}

		assert(int(cur_locs.size()) >= 2);
		int M = int(cur_locs.size()) - 2;

		for (int i = 0; i <= M; i++) {
			dp[{i,i+1}] = 0;
		}

		for (int l = 2; l <= M+1; l++) {
			for (int i = 0, j = l; j <= M+1; i++, j++) {
				dp[{i,j}] = INF;
				for (int k = i+1; k <= j-1; k++) {
					dp[{i,j}] = min(dp[{i,j}], dp[{i,k}] + dp[{k,j}]);
				}
				dp[{i,j}] += cur_locs[j] - cur_locs[i] - 1;
			}
		}
		ans += dp[{0, M+1}];
	}

	cout << ans << '\n';

	return 0;
}
