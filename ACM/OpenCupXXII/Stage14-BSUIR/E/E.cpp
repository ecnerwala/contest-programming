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

const std::array<int, 4> mx{0, 0, -1, 1};
const std::array<int, 4> my{-1, 1, 0, 0};
const std::array<char, 4> move_ch{'L', 'R', 'U', 'D'};

int signum(int a) {
	return (a > 0) - (a < 0);
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	tensor<bool, 2> G({N, M});

	std::array<int, 2> cat{-1, -1};
	std::array<int, 2> dog{-1, -1};
	std::array<int, 2> kitten{-1, -1};

	for (int i = 0; i < N; i++) {
		string S; cin >> S;
		for (int j = 0; j < M; j++) {
			char c = S[j];
			if (c == '.') {
			} else if (c == '#') {
				G[{i, j}] = true;
			} else if (c == 'C') {
				assert((cat == std::array<int, 2>{-1, -1}));
				cat = {i, j};
			} else if (c == 'K') {
				assert((kitten == std::array<int, 2>{-1, -1}));
				kitten = {i, j};
			} else if (c == '@') {
				assert((dog == std::array<int, 2>{-1, -1}));
				dog = {i, j};
			} else assert(false);
		}
	}
	assert(cat[0] != -1);
	assert(kitten[0] != -1);

	bool has_dog = dog[0] != -1;
	if (!has_dog) dog = {0, 0};

	tensor<std::pair<char, std::array<int, 4>>, 4> prv({N, M, N, M}, {'\0', {-1, -1, -1, -1}});
	std::array<int, 4> start{cat[0], cat[1], dog[0], dog[1]};
	prv[start] = {'X', start};
	std::vector<std::array<int, 4>> q; q.push_back(start);

	for (int z = 0; z < int(q.size()); z++) {
		for (int d = 0; d < 4; d++) {
			auto state = q[z];
			state[0] += mx[d];
			state[1] += my[d];
			if (!(0 <= state[0] && state[0] < N && 0 <= state[1] && state[1] < M)) continue;
			if (G[{state[0], state[1]}]) continue;
			if (has_dog && state[0] == state[2] && state[1] == state[3]) continue;
			if (state[0] == kitten[0] && state[1] == kitten[1]) {
				std::string moves;
				moves += move_ch[d];
				for (auto cur = q[z]; cur != start; cur = prv[cur].second) {
					moves += prv[cur].first;
				}

				std::reverse(moves.begin(), moves.end());
				cout << "Yes" << '\n';
				cout << moves << '\n';
				exit(0);
			}

			// now the dog moves twice
			for (int iter = 0; has_dog && iter < 2; iter++) {
				int dx = signum(state[0] - state[2]);
				int dy = signum(state[1] - state[3]);
				if (dy != 0 && !G[{state[2], state[3] + dy}]) {
					state[3] += dy;
				} else if (dx != 0 && !G[{state[2] + dx, state[3]}]) {
					state[2] += dx;
				}
				if (state[0] == state[2] && state[1] == state[3]) {
					goto bad;
				}
			}
			if (prv[state].first == '\0') {
				prv[state] = {move_ch[d], q[z]};
				q.push_back(state);
			}
bad:
			;
		}
	}
	cout << "No" << '\n';

	return 0;
}
