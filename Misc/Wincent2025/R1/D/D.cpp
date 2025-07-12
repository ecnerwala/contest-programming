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

	int T; cin >> T;
	while (T--) {
		int R, C; cin >> R >> C;
		tensor<bool, 2> G({R, C});
		for (int i = 0; i < R; i++) {
			std::string S; cin >> S;
			assert(int(S.size()) == C);
			for (int j = 0; j < C; j++) {
				G[{i, j}] = S[j] == '#';
			}
		}
		cout << [&]() -> int {
			{
				int num_runs = 0;
				bool prv_empty = true;
				for (int i = 0; i < R; i++) {
					bool is_empty = true;
					for (int j = 0; j < C; j++) {
						if (G[{i, j}]) is_empty = false;
					}
					if (i == 0 || i == R-1) { assert(is_empty); }
					if (!is_empty && prv_empty) {
						num_runs++;
					}
					prv_empty = is_empty;
				}
				if (num_runs >= 3) {
					// transpose it
					tensor<bool, 2> G_({C, R});
					for (int i = 0; i < R; i++) {
						for (int j = 0; j < C; j++) {
							G_[{j, i}] = G[{i, j}];
						}
					}
					std::swap(G, G_);
					std::swap(R, C);
				}
			}

			std::vector<std::array<int, 2>> j_ranges;
			{
				bool prv_empty = true;
				for (int j = 0; j < C; j++) {
					bool is_empty = true;
					for (int i = 0; i < R; i++) {
						if (G[{i, j}]) is_empty = false;
					}
					if (!is_empty && prv_empty) {
						j_ranges.push_back({j, -1});
					}
					if (is_empty && !prv_empty) {
						j_ranges.back()[1] = j;
					}
					prv_empty = is_empty;
				}
			}
			int num_letters = int(j_ranges.size());
			assert(num_letters == 3 || num_letters == 4 || num_letters == 5);

			std::vector<int> has_loop(num_letters, 0);

			{
				tensor<bool, 2> is_outside({R, C}, false);
				{
					std::vector<std::array<int, 2>> q; q.reserve(R*C);
					is_outside[{0, 0}] = true;
					q.push_back({0, 0});
					for (int z = 0; z < int(q.size()); z++) {
						auto cur = q[z];
						auto check = [&](std::array<int, 2> nxt) {
							if (G[nxt]) return;
							if (is_outside[nxt]) return;
							is_outside[nxt] = true;
							q.push_back(nxt);
						};
						if (cur[0]-1 >= 0) check({cur[0]-1, cur[1]});
						if (cur[0]+1 <  R) check({cur[0]+1, cur[1]});
						if (cur[1]-1 >= 0) check({cur[0], cur[1]-1});
						if (cur[1]+1 <  C) check({cur[0], cur[1]+1});
					}
				}

				for (int z = 0; z < num_letters; z++) {
					for (int i = 0; i < R; i++) {
						for (int j = j_ranges[z][0]; j < j_ranges[z][1]; j++) {
							if (!G[{i, j}] && !is_outside[{i, j}]) has_loop[z] = true;
						}
					}
				}
			}

			int num_comps = 0;
			{
				tensor<bool, 2> vis({R, C}, false);
				std::vector<std::array<int, 2>> q; q.reserve(R*C);
				for (int si = 0; si < R; si++) {
					for (int sj = 0; sj < C; sj++) {
						if (!G[{si, sj}]) continue;
						if (vis[{si, sj}]) continue;
						num_comps++;
						vis[{si, sj}] = true;
						q.push_back({si, sj});
						for (int z = 0; z < int(q.size()); z++) {
							auto cur = q[z];
							auto check = [&](std::array<int, 2> nxt) {
								if (!G[nxt]) return;
								if (vis[nxt]) return;
								vis[nxt] = true;
								q.push_back(nxt);
							};
							if (cur[0]-1 >= 0) check({cur[0]-1, cur[1]});
							if (cur[0]+1 <  R) check({cur[0]+1, cur[1]});
							if (cur[1]-1 >= 0) check({cur[0], cur[1]-1});
							if (cur[1]+1 <  C) check({cur[0], cur[1]+1});
						}
					}
				}
			}

			int num_loops = std::accumulate(has_loop.begin(), has_loop.end(), 0);
			int num_i = num_comps - num_letters;
			if (num_letters == 3) {
				if (num_loops == 0) {
					assert(num_i == 1);
					return 6;
				} else if (num_loops == 1) {
					return 2;
				} else if (num_loops == 2) {
					return 1;
				} else assert(false);
			} else if (num_letters == 4) {
				if (num_loops == 2) {
					return 0;
				} else if (num_loops == 1) {
					if (num_i == 1) {
						int num_runs = 0;
						{
							bool prv_empty = true;
							for (int i = 0; i < R; i++) {
								bool is_empty = true;
								for (int j = 0; j < C; j++) {
									if (G[{i, j}]) is_empty = false;
								}
								if (!is_empty && prv_empty) {
									num_runs++;
								}
								prv_empty = is_empty;
							}
						}
						if (num_runs == 2) {
							return 9;
						} else if (num_runs == 1) {
							return 5;
						} else assert(false);
					} else if (num_i == 0) {
						return 4;
					} else assert(false);
				} else assert(false);
			} else if (num_letters == 5) {
				if (num_i == 1) {
					return 8;
				} else {
					assert(num_loops == 2);
					if (has_loop[0] || has_loop[4]) return 3;
					else if (has_loop[1] && has_loop[3]) return 7;
				}
			} else assert(false);
		}() << '\n';
	}

	return 0;
}
