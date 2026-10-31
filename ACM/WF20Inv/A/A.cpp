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

const std::array<std::array<int, 2>, 4> MOVES{std::array<int, 2>{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int R, C; cin >> R >> C;
	std::vector<string> G(R);
	for (auto& r : G) cin >> r;

	auto is_valid = [&](int r, int c) -> bool {
		return 0 <= r && r < R && 0 <= c && c < C;
	};

	const int V = R*C;
	std::vector<int> cc(V, -1);
	{
		std::vector<int> q; q.reserve(V);
		for (int i = 0; i < R; i++) {
			for (int j = 0; j < C; j++) {
				if (G[i][j] == '.') continue;
				char val = G[i][j];
				G[i][j] = '.';
				q.push_back(i*C+j);
				for (int z = 0; z < int(q.size()); z++) {
					int cur = q[z];
					cc[cur] = i*C+j;
					int x = cur/C, y = cur%C;
					for (auto [dx, dy] : MOVES) {
						int nx = x + dx, ny = y + dy;
						if (!is_valid(nx, ny) || G[nx][ny] != val) continue;
						G[nx][ny] = '.';
						q.push_back(nx * C + ny);
					}
				}
				q.clear();
			}
		}
	}

	tensor<int, 2> covered({V, V}, 0);
	for (int i = 0; i < V; i++) {
		if (cc[i] == -1) continue;
		int c = cc[i];
		for (int j = 0; j < V; j++) {
			int nx = (j / C) - (c / C) + (i / C);
			int ny = (j % C) - (c % C) + (i % C);
			if (!is_valid(nx, ny) || (cc[nx*C+ny] != -1 && cc[nx*C+ny] != c)) {
				//cerr << "cover " << c << ' ' << j/C << ' ' << j%C << '\n';
				covered[{c, j}]++;
			}
		}
	}

	tensor<int, 2> prv({V, V}, -1);
	std::vector<std::array<int, 2>> q; q.reserve(V*V);
	std::vector<bool> alive(V);

	int num_alive = 0;

	for (int i = 0; i < V; i++) {
		if (cc[i] != i) continue;
		num_alive++;
		alive[i] = true;
		prv[{i, i}] = -2;
		assert((covered[{i, i}] == 0));
		q.push_back({i, i});
	}

	std::vector<std::pair<int, std::string>> ans; ans.reserve(num_alive);

	for (int iter = 0; iter < int(q.size()); iter++) {
		auto [c, l] = q[iter];
		if (!alive[c]) continue;
		int x = l / C, y = l % C;

		assert((prv[{c, l}] != -1));
		assert((covered[{c, l}] == 0));

		if (x == 0) {
			//cerr << "remove " << c << ' ' << x << ' ' << y << '\n';
			alive[c] = false;
			num_alive--;
			// remove this entirely, and uncover everything
			string r;
			for (int cur = l; true; ) {
				int o = prv[{c, cur}];
				if (o == -2) {
					r += 'S';
					break;
				} else if (o / C - cur / C == 1) {
					assert(o % C == cur % C);
					r += 'D';
				} else if (o / C - cur / C == -1) {
					assert(o % C == cur % C);
					r += 'U';
					assert(false);
				} else if (o % C - cur % C == 1) {
					assert(o / C == cur / C);
					r += 'R';
				} else if (o % C - cur % C == -1) {
					assert(o / C == cur / C);
					r += 'L';
				} else assert(false);
				cur = o;
			}
			ans.emplace_back(y, std::move(r));

			for (int i = 0; i < V; i++) {
				if (cc[i] != c) continue;
				cc[i] = -1;
				for (int j = 0; j < V; j++) {
					if (cc[j] != -1 && alive[cc[j]]) {
						// go uncover it if necessary
						int cj = cc[j];
						int nx = (i / C) - (j / C) + (cj / C);
						int ny = (i % C) - (j % C) + (cj % C);
						if (is_valid(nx, ny)) {
							//cerr << "uncover " << cj << ' ' << nx << ' ' << ny << '\n';
							assert((covered[{cj, nx*C+ny}] > 0));
							--covered[{cj, nx*C+ny}];
							if (covered[{cj, nx*C+ny}] == 0 && prv[{cj, nx*C+ny}] != -1) {
								q.push_back({cj, nx*C+ny});
							}
						}
					}
				}
			}
			continue;
		}

		//cerr << "move " << c << ' ' << x << ' ' << y << '\n';
		for (auto [dx, dy] : MOVES) {
			if (dx == 1 && dy == 0) continue;
			int nx = x + dx, ny = y + dy;
			if (!is_valid(nx, ny)) continue;
			if (prv[{c, nx * C + ny}] != -1) continue;
			prv[{c, nx*C+ny}] = l;
			//cerr << "to " << c << ' ' << nx << ' ' << ny << ' ' << covered[{c, nx*C + ny}] << '\n';
			if (covered[{c, nx*C+ny}] == 0) {
				q.push_back({c, nx*C+ny});
			}
		}
	}

	if (num_alive > 0) {
		cout << -1 << '\n';
	} else {
		std::reverse(ans.begin(), ans.end());
		cout << ans.size() << '\n';
		for (const auto& [y, v] : ans) {
			cout << y+1 << ' ' << v << '\n';
		}
	}

	return 0;
}
