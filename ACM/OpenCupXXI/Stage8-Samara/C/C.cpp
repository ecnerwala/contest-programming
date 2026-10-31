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
	int tot_cc = 0;
	for (int i = 0; i < N; i++) {
		string S; cin >> S;
		for (int j = 0; j < M; j++) {
			G[{i,j}] = S[j] == '.';
			tot_cc += int(G[{i,j}]);
		}
	}
	//cerr << tot_cc << '\n';

	vector<pair<int, int>> ans; ans.reserve(N*M);
	for (int si = 0; si < N; si++) {
		vector<int> par; par.reserve(4*N);

		auto get_par = [&](int a) {
			while (par[a] >= 0) a = par[a];
			return a;
		};
		auto merge = [&](int a, int b) {
			a = get_par(a);
			b = get_par(b);
			if (a == b) return false;
			if (a > b) swap(a, b);
			assert(a < b);
			par[b] = a;
			return true;
		};

		auto expand_par = [&]() {
			assert(int(par.size()) == 2*N);
			par.insert(par.begin() + N, N, -1);
			for (int i = 2*N; i < 3*N; i++) {
				if (par[i] >= N) par[i] += N;
			}
		};

		auto compress_par = [&]() {
			for (int i = 0; i < int(par.size()); i++) {
				assert(par[i] < i);
				if (par[i] >= 0 && par[par[i]] >= 0) par[i] = par[par[i]];
			}
		};
		int cur_e;

		vector<pair<vector<int>, int>> prefs(M);
		for (int sj = 0; sj < M; sj++) {
			if (sj == 0) {
				par.assign(2*N, -1);
				for (int i = 0; i < N; i++) {
					par[N+i] = i;
				}
				cur_e = 0;
			} else {
				expand_par();
				for (int i = 0; i < N; i++) {
					if (G[{(si+i) % N, sj-1}] && G[{(si+i) % N, sj}]) {
						cur_e += merge(2*N+i, N+i);
					}
				}
				par.resize(2*N);
			}
			//cerr << "pref " << sj << ' ' << cur_e << '\n';
			assert(int(par.size()) == 2*N);
			for (int i = 0; i+1 < N; i++) {
				if (G[{(si+i) % N, sj}] && G[{(si+i+1) % N, sj}]) {
					cur_e += merge(N+i, N+i+1);
				}
			}
			compress_par();
			//cerr << "pref " << sj << ' ' << cur_e << '\n';
			prefs[sj] = {par, cur_e};
		}

		vector<pair<vector<int>, int>> suffs(M);

		for (int sj = M-1; sj >= 0; sj--) {
			if (sj == M-1) {
				par.assign(2*N, -1);
				for (int i = 0; i < N; i++) {
					par[N+i] = i;
				}
				cur_e = 0;
			} else {
				expand_par();
				for (int i = 0; i < N; i++) {
					if (G[{(si+i) % N, sj+1}] && G[{(si+i) % N, sj}]) {
						cur_e += merge(2*N+i, N+i);
					}
				}
				par.resize(2*N);
			}
			assert(int(par.size()) == 2*N);
			for (int i = 0; i+1 < N; i++) {
				if (G[{(si+i) % N, sj}] && G[{(si+i+1) % N, sj}]) {
					cur_e += merge(N+i, N+i+1);
				}
			}
			compress_par();
			suffs[sj] = {par, cur_e};
		}

		//cerr << si << ',' << 0 << ' ' << prefs[M-1].second << '\n';
		if (prefs[M-1].second == tot_cc-1) {
			ans.push_back({si, 0});
		}
		for (int sj = 1; sj < M; sj++) {
			auto [p1, c1] = prefs[sj-1];
			auto [p2, c2] = suffs[sj];
			cur_e = c1 + c2;
			par.resize(4*N);
			for (int i = 0; i < 2*N; i++) {
				par[i] = p1[i];
			}
			for (int i = 0; i < 2*N; i++) {
				par[2*N+i] = (p2[i] < 0 ? p2[i] : p2[i] + 2*N);
			}
			for (int i = 0; i < N; i++) {
				if (G[{(si+i)%N, 0}] && G[{(si+i)%N, M-1}]) {
					cur_e += merge(i, 2*N+i);
				}
			}
			//cerr << si << ',' << sj << ' ' << cur_e << '\n';
			if (cur_e == tot_cc-1) {
				ans.push_back({si, sj});
			}
		}
	}

	cout << ans.size() << '\n';
	for (auto [x, y] : ans) {
		cout << x << ' ' << y << '\n';
	}

	return 0;
}
