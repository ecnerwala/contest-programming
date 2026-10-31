#include <bits/stdc++.h>

namespace std {

template<class Fun>
class y_combinator_result {
	Fun fun_;
public:
	template<class T>
	explicit y_combinator_result(T &&fun): fun_(std::forward<T>(fun)) {}

	template<class ...Args>
	decltype(auto) operator()(Args &&...args) {
		return fun_(std::ref(*this), std::forward<Args>(args)...);
	}
};

template<class Fun>
decltype(auto) y_combinator(Fun &&fun) {
	return y_combinator_result<std::decay_t<Fun>>(std::forward<Fun>(fun));
}

} // namespace std

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
	tensor<bool, 2> G({N, M});
	for (int i = 0; i < N; i++) {
		string S; cin >> S;
		for (int j = 0; j < M; j++) {
			G[{i,j}] = S[j] == '.';
		}
	}

	tensor<bool, 2> ans({N, M}, false);
	const int V = N*M;
	vector<int> par(V, -1);
	int num_cc = 0;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			num_cc += int(G[{i,j}]);
		}
	}

	vector<pair<int*, int>> undo_stack; undo_stack.reserve(2*V);
	auto get_par = [&](int a) {
		while (par[a] != -1) a = par[a];
		return a;
	};
	auto merge = [&](int a, int b) {
		a = get_par(a);
		b = get_par(b);
		if (a == b) return false;
		if (par[a] > par[b]) swap(a, b);
		undo_stack.push_back({&par[a], par[a]});
		undo_stack.push_back({&par[b], par[b]});
		par[a] += par[b];
		par[b] = a;
		return true;
	};
	auto check_edge = [&](int x, int y) -> void {
		if (G[{x,y}] && G[{(x+1)%N, (y+1)%M}]) {
			int a = x * M + y;
			int b = (x+1)%N * M + (y+1)%M;
			merge(a, b);
		}
	};
	auto checkpoint = [&]() -> int {
		return int(undo_stack.size());
	};
	auto restore = [&](int a) -> void {
		while (int(undo_stack.size()) > a) {
			*undo_stack.back().first = undo_stack.back().second;
			undo_stack.pop_back();
		}
	};

	std::y_combinator([&](auto self, int xlo, int xhi, int ylo, int yhi) -> void {
		assert(xlo < xhi && ylo < yhi);
		if (xhi - xlo == 1 && yhi - ylo == 1) {
			ans[{N-1-xlo, M-1-ylo}] = (num_cc == 1);
			return;
		}
		if (xhi - xlo <= yhi - ylo) {
			int ym = (ylo + yhi) / 2;
			{
				int cp = checkpoint();
				for (int i = 0; i < xlo; i++) {
					for (int j = ym; j < yhi; j++) {
						check_edge(i, j);
					}
				}
				for (int i = xhi; i < N; i++) {
					for (int j = ym; j < yhi; j++) {
						check_edge(i, j);
					}
				}
				self(xlo, xhi, ylo, ym);
				restore(cp);
			}
			{
				int cp = checkpoint();
				for (int i = 0; i < xlo; i++) {
					for (int j = ylo; j < ym; j++) {
						check_edge(i, j);
					}
				}
				for (int i = xhi; i < N; i++) {
					for (int j = ylo; j < ym; j++) {
						check_edge(i, j);
					}
				}
				self(xlo, xhi, ym, yhi);
				restore(cp);
			}
		} else {
			int xm = (xlo + xhi) / 2;
		}
	})(0, N, 0, M);

	return 0;
}
