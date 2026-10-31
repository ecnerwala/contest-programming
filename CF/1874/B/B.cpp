#include <bits/stdc++.h>

template <typename T> void setmin(T& a, T b) { if (b < a) a = b; }

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

	int X0 = 0b01010101;
	int Y0 = 0b00110011;
	int M0 = 0b00001111;

	const int INF = 1e9;
	tensor<int, 2> dist({256, 256}, INF);
	std::vector<std::array<int, 2>> q; q.reserve(256*256);
	dist[{X0, Y0}] = 0;
	q.push_back({X0, Y0});
	for (int z = 0; z < int(q.size()); z++) {
		auto cur = q[z];
		auto [x, y] = cur;
		for (auto nxt : {std::array<int, 2>{x&y, y}, {x|y, y}, {x, x^y}, {x, y^M0}}) {
			if (dist[nxt] == INF) {
				dist[nxt] = dist[cur]+1;
				q.push_back(nxt);
			}
		}
	}

	std::vector<int> base_5(9);
	base_5[0] = 1;
	for (int i = 0; i+1 < 9; i++) {
		base_5[i+1] = base_5[i] * 5;
	}
	std::vector<int> ways(base_5[8], INF);
	for (int i = 0; i < 256; i++) {
		for (int j = 0; j < 256; j++) {
			int v = 0;
			for (int d = 0; d < 8; d++) {
				v += (((i >> d) & 1) * 2 + ((j >> d) & 1)) * base_5[d];
			}
			ways[v] = dist[{i, j}];
		}
	}
	for (int d = 0; d < 8; d++) {
		for (int hi = 0; hi < base_5[8-1-d]; hi++) {
			for (int lo = 0; lo < base_5[d]; lo++) {
				setmin(
					ways[hi * base_5[d+1] + 4 * base_5[d] + lo],
					ways[hi * base_5[d+1] + 3 * base_5[d] + lo]
				);
				setmin(
					ways[hi * base_5[d+1] + 4 * base_5[d] + lo],
					ways[hi * base_5[d+1] + 2 * base_5[d] + lo]
				);
				setmin(
					ways[hi * base_5[d+1] + 4 * base_5[d] + lo],
					ways[hi * base_5[d+1] + 1 * base_5[d] + lo]
				);
				setmin(
					ways[hi * base_5[d+1] + 4 * base_5[d] + lo],
					ways[hi * base_5[d+1] + 0 * base_5[d] + lo]
				);
			}
		}
	}

	int T; cin >> T;
	while (T--) {
		int A, B, C, D, M; cin >> A >> B >> C >> D >> M;
		cout << [&]() -> int {
			std::array<int, 8> places{4, 4, 4, 4, 4, 4, 4, 4};
			for (int i = 0; i < 30; i++) {
				int p = 7 - (((M >> i) & 1) * 4 + ((B >> i) & 1) * 2 + ((A >> i) & 1));
				//assert(((X0 >> p) & 1) == ((A >> i) & 1));
				//assert(((Y0 >> p) & 1) == ((B >> i) & 1));
				//assert(((M0 >> p) & 1) == ((M >> i) & 1));
				int v = ((C >> i) & 1) * 2 + ((D >> i) & 1);
				if (places[p] == 4) places[p] = v;
				if (places[p] != v) return -1;
			}
			int idx = 0;
			for (int d = 0; d < 8; d++) {
				idx += places[d] * base_5[d];
			}
			int res = ways[idx];
			if (res == INF) res = -1;
			return res;
		}() << '\n';
	}

	return 0;
}
