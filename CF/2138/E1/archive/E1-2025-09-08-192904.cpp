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

std::vector<double> cuts = {
		0.550816,
0.559081,
0.559191,
0.559232,
0.559462,
0.559463,
0.560476,
0.560476,
0.560529,
0.560704,
0.560708,
0.560708,
0.560708,
0.560708,
0.560709,
0.560709,
0.560717,
0.560719,
0.560791,
0.560827,
0.564387,
0.5662  ,
0.577552,
0.577552,
0.577553,
0.577553,
0.577569,
0.57766 ,
0.578008,
0.578008,
0.578054,
0.578209,
0.579599,
0.579599,
0.579771,
0.579796,
0.579796,
0.579799,
0.579864,
0.580096,
0.58012 ,
0.580166,
0.580179,
0.580179,
0.580179,
0.580182,
0.580185,
0.580199,
0.5802  ,
0.580285,
0.580319,
0.580319,
0.580319,
0.580322,
0.580323,
0.580399,
0.580399,
0.580905,
0.580989,
0.581016,
0.581138,
0.581138,
0.581164,
0.581668,
0.585071,
0.585814,
0.586578,
0.586743,
0.586748,
0.586811,
0.586812,
0.587116,
0.587199,
0.590031,
0.5903  ,
0.609258,
0.60945 ,
0.609554,
0.609575,
0.610083,
0.612035,
0.612344,
0.612372,
0.612373,
0.612388,
0.612422,
0.612423,
0.61243 ,
0.61243 ,
0.612433,
0.612448,
0.612448,
0.612454,
0.612463,
0.612552,
0.612577,
0.612578,
0.612661,
0.613274,
0.613367,
0.613414,
0.613429,
0.613486,
0.61351 ,
0.613517,
0.61352 ,
0.613906,
0.616748,
0.61675 ,
0.616777,
0.617157,
0.617215,
0.617215,
0.617215,
0.617215,
0.617215,
0.617355,
0.617851,
0.617907,
0.617914,
0.617914,
0.617917,
0.617935,
0.617936,
0.618007,
0.618007,
0.618007,
0.618017,
0.618017,
0.618017,
0.618017,
0.61802 ,
0.618032,
0.618035,
0.618035,
0.618041,
0.618072,
0.618079,
0.61808 ,
0.618099,
0.618292,
0.618292,
0.618346,
0.618347,
0.61835 ,
0.618512,
0.618513,
0.619615,
0.6198  ,
0.619804,
0.620095,
0.620182,
0.620334,
0.630697,
0.632245,
0.632461,
0.632831,
0.632838,
0.632963,
0.633969,
0.640727,
	};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	const int V = int(1e7) + 2;
	std::vector<int> pfac(V+1);
	std::vector<int> primes; primes.reserve(V);
	for (int i = 2; i <= V; i++) {
		if (!pfac[i]) {
			primes.push_back(i);
			pfac[i] = i;
		}
		for (int p : primes) {
			if (int64_t(i) * int64_t(p) > V) break;
			pfac[i*p] = p;
			if (p == pfac[i]) break;
		}
	}


	auto solve_p = [&](int p) -> int {
		// solve p
		std::pair<int, int> best(2*p+1, 1);
		int z = 1;
		for (auto c : cuts) {
			z = std::max(z, int(p * c));
			for (; z <= int(p * c) + 1000; z++) {
				int ops = 2;
				int a = z, b = p - z;
				while (a != b) {
					if (a < b) {std::swap(a, b); ops++;}
					assert(a > b);
					a -= b;
					std::swap(a, b);
					ops++;
					if (ops > best.first) break;
				}
				best = std::min(best, {ops, z});
			}
		}
		std::cerr << p << ' ' << best.first << ' ' << best.second << ' ' << double(best.second) / p << '\n';
		return best.second;
	};

	//for (auto p : std::views::reverse(primes)) { solve_p(p); }
	
	const int N_MAX = 50;

	auto solve = [&](int X) -> tensor<int, 2> {
		int N = 0;
		tensor<int, 2> ans({N_MAX, N_MAX}, 0);
		if (X == 0) return ans;
		for (int v = X; v > 1; v /= pfac[v]) {
			int p = pfac[v];
			int a = solve_p(p);
			int b = p - a;

			int prv = N++;
			ans[{prv, prv}] = 1;
			while (true) {
				int cur = N++;
				assert(N < N_MAX);
				ans[{prv, cur}] = 1;
				ans[{cur, prv}] = -1;
				prv = cur;
				if (a < b) {
					std::swap(a, b);
					continue;
				}
				ans[{cur, cur}] = 1;
				if (a == b) break;
				a -= b;
				std::swap(a, b);
			}
		}
		assert(N <= N_MAX);
		while (N < N_MAX) {
			ans[{N, N}] = 1;
			N++;
		}
		return ans;
	};

	for (int i = int(1e7); i >= 1; i--) {
		solve(i);
	}

	int T; cin >> T;
	while (T--) {
		int X; cin >> X;
		auto ans = solve(X);
		cout << N_MAX << '\n';
		for (int i = 0; i < N_MAX; i++) {
			for (int j = 0; j < N_MAX; j++) {
				cout << ans[{i, j}] << " \n"[j+1==N_MAX];
			}
		}
	}

	return 0;
}
