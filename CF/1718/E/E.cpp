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

	int X, Y; cin >> X >> Y;
	int V = X+Y;
	const int MAXC = int(2e5) + 10;

	std::mt19937_64 mt(48);
	std::vector<uint64_t> color_id(MAXC);
	for (auto& c : color_id) c = mt();

	std::vector<uint64_t> A_color_tot(MAXC);
	std::vector<uint64_t> B_color_tot(MAXC);

	std::vector<std::vector<std::pair<int, int>>> A_adj(V);
	std::vector<std::vector<std::pair<int, int>>> B_adj(V);
	for (int i = 0; i < X; i++) {
		for (int j = X; j < V; j++) {
			int c; cin >> c;
			if (c) {
				A_color_tot[i] ^= color_id[c];
				A_color_tot[j] ^= color_id[c];
				A_adj[i].push_back({c, j});
				A_adj[j].push_back({c, i});
			}
		}
	}
	for (int i = 0; i < X; i++) {
		for (int j = X; j < V; j++) {
			int c; cin >> c;
			if (c) {
				B_color_tot[i] ^= color_id[c];
				B_color_tot[j] ^= color_id[c];
				B_adj[i].push_back({c, j});
				B_adj[j].push_back({c, i});
			}
		}
	}

	for (auto& l : A_adj) std::sort(l.begin(), l.end());
	for (auto& l : B_adj) std::sort(l.begin(), l.end());

	std::vector<std::pair<int, int>> matches; matches.reserve(V);


	std::vector<std::pair<uint64_t, int>> A_by_hash(V);
	for (int v = 0; v < V; v++) A_by_hash[v] = {A_color_tot[v], v};
	std::sort(A_by_hash.begin(), A_by_hash.end());
	std::vector<std::pair<uint64_t, int>> B_by_hash(V);
	for (int v = 0; v < V; v++) B_by_hash[v] = {B_color_tot[v], v};
	std::sort(B_by_hash.begin(), B_by_hash.end());

	std::vector<bool> verts;

	for (int i = 0; i < V; i++) {
		if (A_by_hash[i].first != B_by_hash[i].first || ((A_by_hash[i].second < X) != (B_by_hash[i].second < X))) {
			cout << -1 << '\n';
			exit(0);
		}
	}
	for (int i = 0; i < V; i++) {
		int j = i;
		while (j < V && A_by_hash[j] == B_by_hash[j]) j++;
	}

	std::vector<uint8_t> vis();
	auto dfs = std::y_combinator([&](auto self, int A_cur, int B_cur) -> bool {
		// First do quick O(1) sanity checks
		if (A_color_tot[A_cur] != B_color_tot[B_cur]) return false;
		if ((A_cur < X) != (B_cur < X)) return false;
		if (A_adj[A_cur].size() != B_adj[B_cur].size()) return false;

		int64_t key = int64_t(A_cur) * int64_t(V) + int64_t(B_cur);
		{
			auto it = vis.find(key);
			if (it != vis.end()) return it->second;
		}

		vis.insert({key, true});
		matches.push_back({A_cur, B_cur});
		bool is_good = true;
		for (int i = 0; i < int(A_adj[A_cur].size()); i++) {
			auto [A_color, A_nxt] = A_adj[A_cur][i];
			auto [B_color, B_nxt] = B_adj[B_cur][i];
			if (A_color != B_color) {
				is_good = false;
				break;
			}
			if (!self(A_nxt, B_nxt)) {
				is_good = false;
				break;
			}
		}
		if (!is_good) {
			vis[key] = false;
		}
		return is_good;
	});

	return 0;
}
