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

	int N; cin >> N;
	std::vector<std::array<int, 2>> ops(N * (N-1) / 2);
	for (auto& [a, b] : ops) {
		cin >> a >> b; a--, b--;
	}
	std::reverse(ops.begin(), ops.end());

	tensor<int, 2> op_time({N, N});
	for (int z = 0; z < int(ops.size()); z++) {
		auto [a, b] = ops[z];
		op_time[{a, b}] = op_time[{b, a}] = z;
	}

	tensor<int, 2> merge_id({N, N}, -1);
	{
		std::vector<std::vector<int>> labels(N);
		std::vector<int> par(N);
		for (int i = 0; i < N; i++) {
			par[i] = i;
			labels[i].push_back(i);
		}
		int num_merges = 0;
		for (auto [a, b] : ops) {
			a = par[a], b = par[b];
			if (a == b) continue;

			int cur_merge = num_merges;
			num_merges++;

			for (auto x : labels[a]) {
				for (auto y : labels[b]) {
					merge_id[{x, y}] = 2 * cur_merge + 0;
					merge_id[{y, x}] = 2 * cur_merge + 1;
				}
			}

			for (auto y : labels[b]) {
				labels[a].push_back(y);
				par[y] = a;
			}
			labels[b] = {};
		}
		assert(num_merges == N-1);
	}

	std::vector<int> par(2*(N-1), -1);
	auto get_par = [&](int a) -> int {
		while (par[a] >= 0) {
			if (par[par[a]] >= 0) par[a] = par[par[a]];
			a = par[a];
		}
		return a;
	};
	auto merge = [&](int a, int b) -> bool {
		a = get_par(a);
		b = get_par(b);
		if (a == b) return false;
		if (par[a] > par[b]) std::swap(a, b);
		par[a] += par[b];
		par[b] = a;
		return true;
	};
	auto merge2 = [&](int a, int b) {
		merge(a, b);
		merge(a^1, b^1);
	};

	for (int x = 0; x < N; x++) {
		for (int y = x+1; y < N; y++) {
			for (int z = y+1; z < N; z++) {
				int txy = op_time[{x, y}];
				int txz = op_time[{x, z}];
				int tyz = op_time[{y, z}];
				int min_t = std::min({txy, txz, tyz});
				if (txy == min_t) {
					merge2(merge_id[{x, z}], merge_id[{y, z}]);
				} else if (txz == min_t) {
					merge2(merge_id[{x, y}], merge_id[{z, y}]);
				} else if (tyz == min_t) {
					merge2(merge_id[{y, x}], merge_id[{z, x}]);
				} else assert(false);
			}
		}
	}

	bool is_good = true;
	for (int i = 0; i < N-1; i++) {
		if (get_par(2*i+0) == get_par(2*i+1)) {
			is_good = false;
			break;
		}
	}
	if (is_good) {
		int num_comps = 0;
		for (int i = 0; i < int(par.size()); i++) {
			num_comps += (par[i] < 0);
		}
		assert(num_comps % 2 == 0);
		int ans = 1;
		const int MOD = int(1e9)+7;
		for (int z = 0; z < num_comps / 2; z++) {
			ans <<= 1;
			ans = (ans >= MOD) ? ans - MOD : ans;
		}
		cout << ans << '\n';
	} else {
		cout << 0 << '\n';
	}

	return 0;
}
