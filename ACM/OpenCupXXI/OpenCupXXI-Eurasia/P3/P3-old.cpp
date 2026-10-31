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

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int N, M; cin >> N >> M;

	int P, R; cin >> P >> R;
	assert(P % 2 == 0);
	assert(P + R > 0);

	vector<string> G(N); for (auto& s : G) cin >> s;

	using state = std::array<int, 3>;
	state ST{-1, -1, -1};
	state EN{-1, -1, -1};

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			if (G[i][j] == 'S') {
				ST = {false, i, j};
				G[i][j] = '.';
			} else if (G[i][j] == 'E') {
				EN = {false, i, j};
				G[i][j] = '.';
			}
		}
	}

	const int INF = 1e9;

	tensor<pair<int, state>, 3> prv({2, N, M}, {INF, {0,0,0}});
	prv[ST] = {0, {-1,-1,-1}};

	vector<state> q; q.reserve(N*M*2);
	vector<state> nq; nq.reserve(N*M*2);
	q.push_back(ST);

	tensor<bool, 2> seg1({2*N, 2*M});
	tensor<bool, 2> seg2({2*(N+M), 2*(N+M)});

	for (int d = 0; !q.empty(); d++) {
		nq.clear();
		for (int z = 0; z < int(q.size()); z++) {
			state s = q[z];
			assert(prv[s].first <= d);
			if (prv[s].first < d) continue;
			//cerr << "process " << s[0] << ' ' << s[1] << ' ' << s[2] << ' ' << d << '\n';

			if (s == EN) {
				goto found_path;
			}

			if (s[0] == 0) {
				{ // step adjacent adjacent
					const int mx[4] = {-1, 0, 1, 0};
					const int my[4] = {0, -1, 0, 1};
					for (int a = 0; a < 4; a++) {
						state ns = s;
						ns[1] += mx[a];
						ns[2] += my[a];
						if (!(0 <= ns[1] && ns[1] < N && 0 <= ns[2] && ns[2] < M)) continue;
						if (G[ns[1]][ns[2]] == '@') continue;
						if (d < prv[ns].first) {
							prv[ns] = {d, s};
							q.push_back(ns);
						}
					}
				}

				// Go to the other side
				auto mark_node = std::y_combinator([&](auto self, int ax, int ay) -> void {
					if (seg2[{ax, ay}]) return;
					seg2[{ax, ay}] = true;
					if (ax < (N+M)) {
						self(2 * ax, ay);
						self(2 * ax + 1, ay);
						return;
					} else if (ay < (N+M)) {
						self(ax, 2 * ay);
						self(ax, 2 * ay + 1);
						return;
					} else {
						ax -= (N+M);
						ay -= (N+M);
						ay -= M-1;
						// ns[1] + ns[2] = ax
						// ns[1] - ns[2] = ay
						if ((ax + ay) % 2) return;
						int i = (ax + ay) / 2;
						int j = (ax - ay) / 2;

						if (0 <= i && i < N && 0 <= j && j < M) {
							state ns = {1, i, j};
							if (d+1 < prv[ns].first) {
								prv[ns] = {d+1, s};
								nq.push_back(ns);
							}
						}
					}
				});

				auto mark_range = [&](int ax) {
					for (int
							ay = (N+M) + max(0, s[1] + M - 1 - s[2] - R),
							by = (N+M) + min(N+M-1, s[1] + M - 1 - s[2] + R) + 1;
							ay < by; ay /= 2, by /= 2) {
						if (ay & 1) { mark_node(ax, ay++); }
						if (by & 1) { mark_node(ax, --by); }
					}
				};
				for (int
						ax = (N+M) + max(0, s[1] + s[2] - R),
						bx = (N+M) + min(N+M-1, s[1] + s[2] + R) + 1;
						ax < bx; ax /= 2, bx /= 2) {
					if (ax & 1) {
						mark_range(ax++);
					}
					if (bx & 1) {
						mark_range(--bx);
					}
				}
			} else if (s[0] == 1) {
				// Go to the other side
				auto mark_node = std::y_combinator([&](auto self, int ax, int ay) -> void {
					if (seg1[{ax, ay}]) return;
					seg1[{ax, ay}] = true;
					if (ax < N) {
						self(2 * ax, ay);
						self(2 * ax + 1, ay);
						return;
					} else if (ay < M) {
						self(ax, 2 * ay);
						self(ax, 2 * ay + 1);
						return;
					} else {
						ax -= N;
						ay -= M;

						state ns = {0, ax, ay};
						if (d+1 < prv[ns].first) {
							prv[ns] = {d+1, s};
							nq.push_back(ns);
						}
					}
				});

				auto mark_range = [&](int ax) {
					for (int
							ay = M + max(0, s[2] - P/2),
							by = M + min(M-1, s[2] + P/2) + 1;
							ay < by; ay /= 2, by /= 2) {
						if (ay & 1) { mark_node(ax, ay++); }
						if (by & 1) { mark_node(ax, --by); }
					}
				};
				for (int
						ax = N + max(0, s[1] - P/2),
						bx = N + min(N-1, s[1] + P/2) + 1;
						ax < bx; ax /= 2, bx /= 2) {
					if (ax & 1) {
						mark_range(ax++);
					}
					if (bx & 1) {
						mark_range(--bx);
					}
				}
			} else assert(false);
		}

		swap(q, nq);
	}

found_path:
	assert(prv[EN].first % 2 == 0);
	cout << prv[EN].first / 2 << '\n';

	vector<pair<int, int>> path; path.reserve(N*M);

	for (auto cur = EN; true; cur = prv[cur].second) {
		if (cur[0] == 0) {
			path.push_back({cur[1],cur[2]});
		}
		if (cur == ST) break;
	}

	reverse(path.begin(), path.end());
	for (auto it : path) {
		cerr << it.first << ' ' << it.second << '\n';
	}

	return 0;
}
