#include<bits/stdc++.h>

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
	std::enable_if_t<(0 < D), tensor_view<T, NDIMS-1>> operator[] (int idx) const {
		std::array<int, NDIMS-1> nshape; std::copy(shape.begin()+1, shape.end(), nshape.begin());
		std::array<int, NDIMS-1> nstrides; std::copy(strides.begin()+1, strides.end(), nstrides.begin());
		T* ndata = data + (strides[0] * idx);
		return tensor_view<T, NDIMS-1>(nshape, nstrides, ndata);
	}
	template <int D = NDIMS>
	std::enable_if_t<(0 < D), tensor_view<T, NDIMS-1>> at(int idx) const {
		assert(0 <= idx && idx < shape[0]);
		return operator[](idx);
	}

	template <int D = NDIMS>
	std::enable_if_t<(0 == D), T&> operator * () const {
		return *data;
	}

	template <typename U, int D> friend struct tensor_view;
	template <typename U, int D> friend struct tensor;
};

template <typename T, int NDIMS> struct tensor {
	static_assert(NDIMS >= 0, "NDIMS must be nonnegative");

	std::array<int, NDIMS> shape;
protected:
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
	std::enable_if_t<(0 < D), tensor_view<T, NDIMS-1>> operator[] (int idx) {
		return view()[idx];
	}
	template <int D = NDIMS>
	std::enable_if_t<(0 < D), tensor_view<T, NDIMS-1>> at(int idx) {
		return view().at(idx);
	}

	template <int D = NDIMS>
	std::enable_if_t<(0 < D), tensor_view<const T, NDIMS-1>> operator[] (int idx) const {
		return view()[idx];
	}
	template <int D = NDIMS>
	std::enable_if_t<(0 < D), tensor_view<const T, NDIMS-1>> at(int idx) const {
		return view().at(idx);
	}

	template <int D = NDIMS>
	std::enable_if_t<(0 == D), T&> operator * () {
		return *view();
	}
	template <int D = NDIMS>
	std::enable_if_t<(0 == D), const T&> operator * () const {
		return *view();
	}
};

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

const int mx[4] = {-1, 0, 1, 0};
const int my[4] = {0, -1, 0, 1};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int N, M; cin >> N >> M;

	int P, R; cin >> P >> R;
	assert(P % 2 == 0);
	assert(P + R > 0);

	vector<string> G(N); for (auto& s : G) cin >> s;
	vector<pair<int, int>> path; path.reserve(N*M);

	using state = std::array<int, 2>;
	state ST{-1, -1};
	state EN{-1, -1};

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			if (G[i][j] == 'S') {
				ST = {i, j};
				G[i][j] = '.';
			} else if (G[i][j] == 'E') {
				EN = {i, j};
				G[i][j] = '.';
			}
		}
	}

	const int INF = 1e9;

	tensor<pair<int, state>, 2> prv({N, M}, {INF, {-1,-1}});

	vector<state> q; q.reserve(N*M);

	prv[ST] = {0, {-1,-1}};
	q.push_back(ST);

	int idx = 0;
	for (int d = 0; idx < int(q.size()); d++) {
		int first_idx = idx;
		while (idx < int(q.size())) {
			state s = q[idx++];
			state p = prv[s].second;
			for (int a = 0; a < 4; a++) {
				state ns = s;
				ns[0] += mx[a];
				ns[1] += my[a];
				if (!(0 <= ns[0] && ns[0] < N && 0 <= ns[1] && ns[1] < M)) continue;
				if (G[ns[0]][ns[1]] == '@') continue;
				if (d < prv[ns].first) {
					prv[ns] = {d, p};
					q.push_back(ns);
				}
			}
		}
		idx = first_idx;

		bool is_first = true;
		int mid_idx = idx;
		for (int iter = 0; iter < P/2; iter++) {
			for (int z = int(q.size()) - idx; z --> 0; ) {
				state s = q[idx++];
				state p = is_first ? s : prv[s].second;
				for (int dx : {-1, 1}) {
					state ns = s;
					ns[0] += dx;
					if (!(0 <= ns[0] && ns[0] < N && 0 <= ns[1] && ns[1] < M)) continue;
					if (d+1 < prv[ns].first) {
						prv[ns] = {d+1, p};
						q.push_back(ns);
					}
				}
			}
			swap(idx, mid_idx);
			for (int z = int(q.size()) - idx; z --> 0; ) {
				state s = q[idx++];
				state p = (is_first && prv[s].first == d) ? s : prv[s].second;
				for (int dy : {-1, 1}) {
					state ns = s;
					ns[1] += dy;
					if (!(0 <= ns[0] && ns[0] < N && 0 <= ns[1] && ns[1] < M)) continue;
					if (d+1 < prv[ns].first) {
						prv[ns] = {d+1, p};
						q.push_back(ns);
					}
				}
			}
			swap(idx, mid_idx);
			is_first = false;
		}

		for (int iter = 0; iter < R; iter++) {
			for (int z = int(q.size()) - idx; z --> 0; ) {
				state s = q[idx++];
				state p = is_first ? s : prv[s].second;
				for (int a = 0; a < 4; a++) {
					state ns = s;
					ns[0] += mx[a];
					ns[1] += my[a];
					if (!(0 <= ns[0] && ns[0] < N && 0 <= ns[1] && ns[1] < M)) continue;
					if (d+1 < prv[ns].first) {
						prv[ns] = {d+1, p};
						q.push_back(ns);
					}
				}
			}
			is_first = false;
		}
	}

	std::vector<state> bombs; bombs.reserve(prv[EN].first);
	for (state cur = prv[EN].second; cur != state{-1, -1}; cur = prv[cur].second) {
		bombs.push_back(cur);
	}
	assert(int(bombs.size()) == prv[EN].first);
	reverse(bombs.begin(), bombs.end());
	cout << bombs.size() << '\n';
	for (auto it : bombs) {
		cout << it[0]+1 << ' ' << it[1]+1 << '\n';
	}

	return 0;
}
