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

	if (false) {
		const int V = 200;
		const int INF = 1e9;
		tensor<int, 2> dp({V+1, V+1}, INF);
		double phi = (1 + sqrt(5)) / 2;
		std::vector<double> los(V+1, INF);
		std::vector<double> his(V+1, -INF);
		for (int s = 0; s <= V; s++) {
			for (int a = s, b = 0; a >= 0; a--, b++) {
				if (s <= 2) {
					dp[{a, b}] = 0;
					continue;
				}
				int v = INF;
				int best_a = -1, best_b = -1;
				for (int la = 0; la <= a; la++) {
					for (int lb = 0; lb <= b; lb++) {
						int cnd = 1 + std::max(dp[{b - lb, la + lb}], dp[{lb, (a - la) + (b - lb)}]);
						if (cnd < v) {
							v = cnd;
							best_a = la, best_b = lb;
						}
					}
				}
				dp[{a, b}] = v;
				//cerr << v << '(' << best_a << ',' << best_b << ')' << ' ';
				//cerr << v << '(' << fixed << setprecision(2) << phi * b + a << ')' << ' ';
				if (b != 0) {
					double val = phi * b + a;
					los[v] = std::min(los[v], val);
					his[v] = std::max(his[v], val);
				}
			}
		}
		for (int v = 1; v <= V; v++) {
			if (los[v] > his[v]) continue;
			cerr << v << ' ' << los[v] << ' ' << his[v] << '\n';
		}
		exit(0);
	}

	int N; cin >> N;
	std::vector<int> A; A.reserve(N);
	std::vector<int> B; B.reserve(N);
	for (int i = 0; i < N; i++) {
		B.push_back(i+1);
	}

	auto get_LS = [&](int SA, int SB) -> std::pair<int, int> {
		assert(SA + SB > 2);
		if (SA == 1 && SB == 2) {
			return {0, 2};
		}
		if (SA & 1) {
			return {SA / 2, std::min(SB, (SB + 2) / 2)};
		} else {
			return {SA / 2, (SB + 1) / 2};
		}
	};
	std::map<std::pair<int, int>, int> mem;
	auto cnt_moves = std::y_combinator([&](auto self, int sa, int sb) -> int {
		if (sa + sb <= 2) return 0;
		if (mem.count({sa, sb})) return mem[{sa, sb}];
		int v = 0;
		auto [la, lb] = get_LS(sa, sb);
		v = std::max(v, self(sb - lb, la + lb));
		v = std::max(v, self(lb, (sa - la) + (sb - lb)));
		v++;
		return mem[{sa, sb}] = v;
	});
	cerr << cnt_moves(0, 10) << '\n';
	cerr << cnt_moves(0, 100) << '\n';
	cerr << cnt_moves(0, 1000) << '\n';
	cerr << cnt_moves(0, 10000) << '\n';
	cerr << cnt_moves(0, 100000) << '\n';

	std::vector<int> query; query.reserve(N);
	std::vector<int> NA; NA.reserve(N);
	std::vector<int> NB; NB.reserve(N);
	while (int(A.size()) + int(B.size()) > 2) {
		auto [LA, LB] = get_LS(int(A.size()), int(B.size()));
		query.clear();
		for (int i = 0; i < LA; i++) {
			query.push_back(A[i]);
		}
		for (int i = 0; i < LB; i++) {
			query.push_back(B[i]);
		}
		cout << '?' << ' ' << query.size();
		for (int i = 0; i < int(query.size()); i++) {
			cout << ' ' << query[i];
		}
		cout << '\n' << flush;
		string resp; cin >> resp;
		NA.clear();
		NB.clear();
		if (resp == "YES") {
			for (int i = 0; i < LA; i++) {
				NB.push_back(A[i]);
			}
			for (int i = 0; i < LB; i++) {
				NB.push_back(B[i]);
			}
			for (int i = LB; i < int(B.size()); i++) {
				NA.push_back(B[i]);
			}
		} else if (resp == "NO") {
			for (int i = LA; i < int(A.size()); i++) {
				NB.push_back(A[i]);
			}
			for (int i = LB; i < int(B.size()); i++) {
				NB.push_back(B[i]);
			}
			for (int i = 0; i < LB; i++) {
				NA.push_back(B[i]);
			}
		} else assert(false);
		std::swap(A, NA);
		std::swap(B, NB);
	}
	std::vector<int> res; res.reserve(N);
	for (int v : A) res.push_back(v);
	for (int v : B) res.push_back(v);

	for (int v : res) {
		cout << '!' << ' ' << v << '\n' << flush;
		string resp; cin >> resp;
		if (resp == ":)") {
			break;
		} else if (resp == ":(") {
			continue;
		} else assert(false);
	}

	return 0;
}
