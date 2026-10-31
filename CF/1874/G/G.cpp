#include <bits/stdc++.h>

template <typename T> void setmax(T& a, T b) { if (b > a) a = b; }

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

	int N, M; cin >> N >> M;
	const int V = 200;
	struct op_t {
		int type = 0;
		int x = 0, y = 0;
	};
	std::vector<op_t> ops(N);
	for (auto& o : ops) {
		cin >> o.type;
		if (o.type == 0) {
		} else if (o.type == 1) {
			cin >> o.x >> o.y;
		} else if (o.type == 2) {
			cin >> o.x;
		} else if (o.type == 3) {
			cin >> o.y;
		} else if (o.type == 4) {
			// whatever
			cin >> o.x;
		} else assert(false);
	}

	std::vector<std::vector<int>> adj(N);
	for (int e = 0; e < M; e++) {
		int u, v; cin >> u >> v; u--, v--;
		adj[u].push_back(v);
	}

	const int FINAL = int(1e9);
	const int INF = int(2e9);

	// lots of different DP's
	// we have small/big/

	// first coord is need, second coord is the coeff of other == final amt of it (same side)
	tensor<int, 3> need_hp({N, V+1, V+1}, -INF);
	tensor<int, 3> need_dmg({N, V+1, V+1}, -INF);
	need_hp[{0, 0, 0}] = 0;
	need_dmg[{0, 0, 0}] = 0;

	std::vector<std::vector<std::tuple<int, int, int>>> pareto(N);
	for (auto& v : pareto) v.reserve(V * (N + 1) + 1);
	std::vector<std::tuple<int, int, int>> buf; buf.reserve(V * (N + 1) + 1);

	auto merge_into = [&](std::vector<std::tuple<int, int, int>>& a, const std::vector<std::tuple<int, int, int>>& b) {
		buf.clear();
		for (int i = 0, j = 0; i < int(a.size()) || j < int(b.size()); ) {
			std::tuple<int, int, int> cur;
			if (i < int(a.size()) && j < int(b.size()) && std::get<0>(a[i]) == std::get<0>(b[j])) {
				cur = std::max(a[i++], b[j++]);
			} else if (i < int(a.size()) && (j == int(b.size()) || std::get<0>(a[i]) < std::get<0>(b[j]))) {
				cur = a[i++];
			} else {
				cur = b[j++];
			}
			// get<0>(cur) is strictly increasing already
			while (!buf.empty()) {
				assert(std::get<0>(cur) > std::get<0>(buf.back()));
				if (std::get<1>(cur) >= std::get<1>(buf.back())) {
					buf.pop_back();
				} else {
					break;
				}
			}
			buf.push_back(cur);
		}
		std::swap(a, buf);
	};

	std::vector<std::tuple<int, int, int>> buf2; buf2.reserve(1);

	for (int i = 0; i < N; i++) {
		/*
		cerr << "before " << i << '\n';
		cerr << " pareto" << '\n';
		for (auto [a, b, c] : pareto[i]) {
			cerr << " " << a << ' ' << b << ' ' << c << '\n';
		}
		*/

		// apply the op to this guy
		if (ops[i].type == 0) {
			// do nothing
		} else if (ops[i].type == 1) {
			int hp = ops[i].x;
			int dmg = ops[i].y;
			// everyone get the base value
			for (int v = V; v >= 0; v--) {
				for (int u = V; u >= 0; u--) {
					if (need_hp[{i, v, u}] >= 0) need_hp[{i, v, u}] += hp * dmg;
					if (need_dmg[{i, v, u}] >= 0) need_dmg[{i, v, u}] += hp * dmg;
				}
			}
			for (auto& [a, b, c] : pareto[i]) {
				c += hp * dmg;
			}
			int best_constant = -INF;
			for (int v = V; v >= 1; v--) {
				for (int u = V; u >= 0; u--) {
					int n_hp = need_hp[{i, v, u}];
					int n_dmg = need_dmg[{i, v, u}];
					if (n_hp >= 0) setmax(need_hp[{i, v, std::max(u, hp)}], n_hp);
					if (n_dmg >= 0) setmax(need_dmg[{i, v, std::max(u, dmg)}], n_dmg);
				}
			}
			{
				int v = 0;
				for (int u = V; u >= 0; u--) {
					int n_hp = need_hp[{i, v, u}];
					int n_dmg = need_dmg[{i, v, u}];
					setmax(best_constant, n_hp);
					setmax(best_constant, n_dmg);
					for (int w = V; w >= 0; w--) {
						if (n_hp >= 0 && w + hp <= V) setmax(need_hp[{i, w, u}], n_hp + w * dmg);
						if (n_dmg >= 0 && w + dmg <= V) setmax(need_dmg[{i, w, u}], n_dmg + hp * w);
					}
				}
			}
			assert(best_constant >= 0);
			if (best_constant >= 0) {
				for (int w = 0; w <= V; w++) {
					if (w + hp <= V) {
						setmax(need_hp[{i, w, w + hp}], best_constant + w * dmg);
					}
					if (w + dmg <= V) {
						setmax(need_dmg[{i, w, w + dmg}], best_constant + hp * w);
					}
				}
				// put it into the pareto
				buf2.clear();
				buf2.push_back({hp, dmg, best_constant - hp * dmg});
				merge_into(pareto[i], buf2);
			}
		} else if (ops[i].type == 2) {
			// add hp
			int diff = ops[i].x;
			assert(diff > 0);
			for (int v = 0; v <= V; v++) {
				for (int u = 0; u <= V; u++) {
					if (need_dmg[{i, v, u}] >= 0) need_dmg[{i, v, u}] += u * diff;
				}
			}
			for (int v = diff; v <= V; v++) {
				for (int u = 0; u <= V; u++) {
					setmax(need_hp[{i, v-diff, u}], need_hp[{i, v, u}]);
				}
			}
			for (auto& [a, b, c] : pareto[i]) {
				a += diff;
			}
		} else if (ops[i].type == 3) {
			// add dmg
			int diff = ops[i].y;
			assert(diff > 0);
			for (int v = 0; v <= V; v++) {
				for (int u = 0; u <= V; u++) {
					if (need_hp[{i, v, u}] >= 0) need_hp[{i, v, u}] += u * diff;
				}
			}
			for (int v = diff; v <= V; v++) {
				for (int u = 0; u <= V; u++) {
					setmax(need_dmg[{i, v-diff, u}], need_dmg[{i, v, u}]);
				}
			}
			for (auto& [a, b, c] : pareto[i]) {
				b += diff;
			}
		} else if (ops[i].type == 4) {
			// add flat value
			int w = ops[i].x;
			assert(w > 0);
			for (int v = 0; v <= V; v++) {
				for (int u = 0; u <= V; u++) {
					need_hp[{i, v, u}] += w;
					need_dmg[{i, v, u}] += w;
				}
			}
			for (auto& [a, b, c] : pareto[i]) {
				c += w;
			}
		} else assert(false);

		/*
		cerr << "after " << i << '\n';
		cerr << " pareto" << '\n';
		for (auto [a, b, c] : pareto[i]) {
			cerr << " " << a << ' ' << b << ' ' << c << '\n';
		}
		*/

		if (i == N-1) {
			int64_t ans = 0;
			{
				int v = 0;
				for (int u = 0; u <= V; u++) {
					setmax<int64_t>(ans, need_hp[{i, v, u}]);
					setmax<int64_t>(ans, need_dmg[{i, v, u}]);
				}
			}
			for (auto [a, b, c] : pareto[i]) {
				assert(c >= 0);
				setmax(ans, int64_t(FINAL) * int64_t(a) * int64_t(b) + c);
			}
			cout << ans << '\n';
			break;
		}
		for (auto j : adj[i]) {
			assert(j > i);
			merge_into(pareto[j], pareto[i]);
			for (int v = 0; v <= V; v++) {
				for (int u = 0; u <= V; u++) {
					setmax(need_hp[{j, v, u}], need_hp[{i, v, u}]);
					setmax(need_dmg[{j, v, u}], need_dmg[{i, v, u}]);
				}
			}
		}
	}

	return 0;
}
