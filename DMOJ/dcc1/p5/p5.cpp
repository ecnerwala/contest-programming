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

struct union_find {
	std::vector<int> par;
	union_find(int N) : par(N, -1) {}
	int get_par(int a) {
		while (par[a] >= 0) {
			if (par[par[a]] >= 0) par[a] = par[par[a]];
			a = par[a];
		}
		return a;
	}
	bool merge(int a, int b) {
		a = get_par(a), b = get_par(b);
		if (a == b) return false;
		if (par[a] > par[b]) std::swap(a, b);
		par[a] += par[b];
		par[b] = a;
		return true;
	}
	int cnt_roots() {
		int res = 0;
		for (auto v : par) res += (v < 0);
		return res;
	}
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, K; cin >> N >> K;

	tensor<int, 2> A({N, N});
	for (int i = 0; i < N; i++) {
		std::string s; cin >> s;
		assert(int(s.size()) == N);
		for (int j = 0; j < N; j++) {
			A[{i, j}] = s[j] - '0';
		}
	}
	tensor<int, 2> B = A;

	bool possible = [&]() -> bool {;
		if (K == 1) {
			for (int i = 0; i < N; i++) {
				for (int j = i+1; j < N; j++) {
					if (!A[{i, j}]) {
						// break i using blue edges
						for (int k = 0; k < N; k++) {
							if (A[{i, k}]) B[{i, k}] = B[{k, i}] = 2;
						}
						return true;
					}
				}
			}
			return false;
		} else if (K == 2) {
			// we need 2 vertices with disjoint neighbor sets which don't cover the whole thing
			// or we need a 4-cycle in the complement

			// pick the unused vertex, then pick 2 non-neighbors. If those 2 non-neighbors have a common non-neighbor
			// first, let's try to find any 4 cycle in the complement
			tensor<int, 2> cycs({N, N}, -1);
			std::vector<int> buf; buf.reserve(N);
			for (int i = 0; i < N; i++) {
				buf.clear();
				for (int j = 0; j < N; j++) {
					if (!A[{i, j}] && i != j) buf.push_back(j);
				}
				for (int z = 0; z < int(buf.size()); z++) {
					for (int w = z+1; w < int(buf.size()); w++) {
						int a = buf[z], b = buf[w];
						assert(a < b);
						int j = std::exchange(cycs[{a, b}], i);
						if (j != -1) {
							// we have a 4-cycle in the complement
							for (int k = 0; k < N; k++) {
								if (A[{i, k}]) B[{i, k}] = B[{k, i}] = 2;
								if (A[{j, k}]) B[{j, k}] = B[{k, j}] = 2;
							}
							return true;
						}
					}
				}
			}
			// otherwise, there's no 4-cycle, we need to find 2 vertices with disjoint neighbor sets
			// these 2 vertices have at most 1 common non-neighbor, so their hashes really should be complements
			std::mt19937_64 mt(48);
			std::vector<uint64_t> val(N);
			for (auto& v : val) v = mt();

			uint64_t tot_val = 0;
			for (auto v : val) tot_val ^= v;

			std::vector<uint64_t> neighbors(N);
			for (int i = 0; i < N; i++) {
				uint64_t v = 0;
				for (int j = 0; j < N; j++) {
					if (A[{i, j}] || j == i) v |= val[j];
				}
				neighbors[i] = v;
			}

			for (int i = 0; i < N; i++) {
				for (int j = i+1; j < N; j++) {
					if (cycs[{i, j}] != -1) {
						uint64_t a = neighbors[i] ^ neighbors[j] ^ val[cycs[{i, j}]];
						if (a == tot_val) {
							for (int k = 0; k < N; k++) {
								if (A[{i, k}]) B[{i, k}] = B[{k, i}] = 2;
								if (A[{j, k}]) B[{j, k}] = B[{k, j}] = 2;
							}
							return true;
						}
					}
				}
			}

			return false;
		} else assert(false);
	}();

	if (possible) {
		cout << "POSSIBLE" << '\n';
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				cout << B[{i,j}];
			}
			cout << '\n';
		}
		union_find uf1(N), uf2(N);
		for (int i = 0; i < N; i++) {
			assert((B[{i, i}] == 0));
			for (int j = 0; j < N; j++) {
				assert((B[{i, j}] == B[{j, i}]));
				if (B[{i, j}] == 1) {
					uf1.merge(i, j);
				} else if (B[{i, j}] == 2) {
					uf2.merge(i, j);
				} else assert((B[{i, j}] == 0));
			}
		}
		assert(uf1.cnt_roots() > K);
		assert(uf2.cnt_roots() > K);
	} else {
		cout << "IMPOSSIBLE" << '\n';
	}

	return 0;
}

// if K == 1, find any non-edge, and connect those two monochromatically
// if K == 2, we want to find 3 subsets
// if we pick 2 disconnected vertices, then we want their neighbor set not to be the whole graph
// maybe look at the complement? we want to find something very dense in the complement
//
// If the graph is disconnected, then we just need any missing edge in 1 component, and we do K = 1
// If the graph is connected, then

// At this point, every two vertices covers everything except possibly 1 vertex
