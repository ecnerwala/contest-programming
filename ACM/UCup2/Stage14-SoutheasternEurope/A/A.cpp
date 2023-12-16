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

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	const int MAX_B = 40;
	std::vector<int> cnt_0(MAX_B);
	std::vector<int> cnt_1(MAX_B);
	tensor<int, 2> has_1_0({MAX_B, MAX_B}, 0);
	int N_inps; cin >> N_inps;
	for (int i = 0; i < N_inps; i++) {
		int64_t a; cin >> a;
		for (int b = 0; b < MAX_B; b++) {
			int v = (a >> b) & 1;
			if (v) {
				cnt_1[b]++;
				for (int c = 0; c < MAX_B; c++) {
					if (!((a >> c) & 1)) has_1_0[{b, c}] = true;
				}
			} else {
				cnt_0[b]++;
			}
		}
	}

	std::vector<std::pair<int, int>> alive_inds; alive_inds.reserve(MAX_B);
	for (int i = 0; i < MAX_B; i++) {
		if (cnt_1[i] && cnt_0[i]) {
			bool is_eq = false;
			for (auto [j, j_cnt] : alive_inds) {
				if (!has_1_0[{i, j}] && !has_1_0[{j, i}]) {
					is_eq = true;
					break;
				}
			}
			if (is_eq) continue;
			alive_inds.push_back({i, cnt_1[i]});
		}
	}

	int B = int(alive_inds.size());
	std::sort(alive_inds.begin(), alive_inds.end(), [&](auto a, auto b) { return a.second < b.second; });
	std::vector<int64_t> implies_msks(B);
	for (int i = 0; i < B; i++) {
		for (int j = i+1; j < B; j++) {
			assert((has_1_0[{alive_inds[j].first, alive_inds[i].first}]));
			if (!has_1_0[{alive_inds[i].first, alive_inds[j].first}]) {
				implies_msks[i] |= int64_t(1) << j;
			}
		}
	}

	int L = (B / 2);
	std::vector<int64_t> l_msks(1 << L);
	for (int i = 0; i < L; i++) {
		for (int m = 0; m < (1 << i); m++) {
			l_msks[(1 << i) + m] = l_msks[m] | implies_msks[i];
		}
	}
	std::vector<int64_t> dp(1 << (B - L));
	for (int m = 0; m < (1 << L); m++) {
		// if there's something we need but don't have, we're screwed already
		if (l_msks[m] & ((1 << L) - 1) & ~m) {
			continue;
		}
		dp[l_msks[m] >> L]++;
	}
	for (int i = L; i < B; i++) {
		assert(int(dp.size()) == (1 << (B - i)));
		std::vector<int64_t> ndp(1 << (B - 1 - i));
		int n_msk = int(implies_msks[i] >> (i+1));
		for (int m = 0; m < (1 << (B - i)); m++) {
			if (!(m & 1)) {
				ndp[m >> 1] += dp[m];
			}
			ndp[(m >> 1) | n_msk] += dp[m];
		}
		dp = ndp;
	}
	cout << dp[0] << '\n';

	return 0;
}
