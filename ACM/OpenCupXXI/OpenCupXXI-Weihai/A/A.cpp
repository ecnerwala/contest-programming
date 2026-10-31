#include<bits/extc++.h>

struct splitmix64_hash {
	static uint64_t splitmix64(uint64_t x) {
		// http://xorshift.di.unimi.it/splitmix64.c
		x += 0x9e3779b97f4a7c15;
		x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
		x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
		return x ^ (x >> 31);
	}

	size_t operator()(uint64_t x) const {
		static const uint64_t FIXED_RANDOM = std::chrono::steady_clock::now().time_since_epoch().count();
		return splitmix64(x + FIXED_RANDOM);
	}
};

template <typename K, typename V, typename Hash = splitmix64_hash>
using hash_map = __gnu_pbds::gp_hash_table<K, V, Hash>;

template <typename K, typename Hash = splitmix64_hash>
using hash_set = hash_map<K, __gnu_pbds::null_type, Hash>;

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

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int N, M; cin >> N >> M;
	vector<vector<int>> adj(N);
	for (int e = 0; e < M; e++) {
		int u, v; cin >> u >> v; u--, v--;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}

	// Identify a min-degree vertex
	int ROOT = 0;
	for (int i = 1; i < N; i++) {
		if (int(adj[i].size()) < int(adj[ROOT].size())) ROOT = i;
	}

	int best_frames = 1;
	tensor<int, 2> best_output({1, N});
	for (int i = 0; i < N; i++) best_output[{0, i}] = i;

	for (int ROOT_1 : adj[ROOT]) {
		[&]() -> bool {
			vector<array<int, 2>> label(N, {-1, -1});
			vector<int> dist(N, -1);
			vector<int> q; q.reserve(N);
			vector<int> nq; nq.reserve(N);

			label[ROOT] = {0,0};
			dist[ROOT] = 0;

			int T = 2;
			int S = 1;
			for (int a : adj[ROOT]) {
				if (a == ROOT_1) {
					label[a] = {1,0};
				} else {
					label[a] = {0,S++};
				}
				dist[a] = 1;
				q.push_back(a);
			}

			for (int d = 1; !q.empty(); d++) {
				nq.clear();
				for (int cur : q) {
					assert(dist[cur] == d);
					for (int nxt : adj[cur]) {
						if (dist[nxt] == -1) {
							dist[nxt] = d+1;
							nq.push_back(nxt);
						}
					}
				}

				for (int cur : q) {
					for (int nxt : adj[cur]) {
						if (dist[nxt] != d+1) continue;
						label[nxt][0] = max(label[nxt][0], label[cur][0]);
					}
				}

				for (int cur : q) {
					for (int nxt : adj[cur]) {
						if (dist[nxt] != d+1) continue;
						if (label[nxt][0] == label[cur][0]) {
							// same level
						} else if (label[nxt][0] == label[cur][0]+1) {
							if (label[nxt][1] != -1) return false;
							label[nxt][1] = label[cur][1];
						} else {
							// that's wrong too
							return false;
						}
					}
				}

				int unmatched_id = -1;
				for (int nxt : nq) {
					if (label[nxt][1] == -1) {
						if (label[nxt][0] == 0) {
							label[nxt][1] = S++;
						} else {
							// this guy is unmatched
							if (unmatched_id != -1) return false;
							unmatched_id = nxt;
							if (label[nxt][0] != T-1) return false;
							label[nxt][0]++;
							T++;
							label[nxt][1] = 0;
						}
					}
				}

				if (unmatched_id != -1) {
					for (int cur : q) {
						for (int nxt : adj[cur]) {
							if (dist[nxt] != d+1) continue;
							if (nxt == unmatched_id) {
								if (label[cur][1] != label[nxt][1]) return false;
								if (label[cur][0] + 1 != label[nxt][0]) return false;
								// otherwise this matching is correct
							}
						}
					}
				}

				std::swap(q, nq);
			}

			// check this early so we can avoid later stuff
			if (T <= best_frames) return false;

			if (int64_t(T) * int64_t(S) != N) return false;
			for (int a = 0; a < N; a++) {
				// first, let's check for exactly 1 edge going down and one edge going up
				int num_prev = 0;
				int num_next = 0;
				for (int b : adj[a]) {
					if (label[b][0] != label[a][0]) {
						if (label[b][1] != label[a][1]) return false;
						if (label[b][0] == label[a][0] - 1) {
							num_prev++;
						} else if (label[b][0] == label[a][0] + 1) {
							num_next++;
						} else return false;
					} else {
						if (label[b][1] == label[a][1]) return false;
					}
				}
				if (num_prev != (label[a][0] == 0 ? 0 : 1)) return false;
				if (num_next != (label[a][0] == T-1 ? 0 : 1)) return false;
			}

			hash_map<int64_t, int> top_edges;
			for (int a = 0; a < N; a++) {
				for (int b : adj[a]) {
					if (label[a][0] != label[b][0]) continue;
					if (label[a][1] > label[b][1]) continue;
					int x = label[a][1], y = label[b][1];
					int64_t key = int64_t(x) * S + y;
					top_edges[key]++;
				}
			}
			for (const auto& it : top_edges) {
				if (it.second != T) return false;
			}

			assert(T * S == N);
			best_frames = T;
			best_output = tensor<int, 2>({T, S});
			for (int i = 0; i < N; i++) {
				best_output[label[i]] = i;
			}
			return true;
		}();
	}

	cout << best_frames << ' ' << N / best_frames << '\n';
	for (int t = 0; t < best_frames; t++) {
		for (int s = 0; s < N / best_frames; s++) {
			if (s) cout << ' ';
			cout << best_output[{t, s}]+1;
		}
		cout << '\n';
	}

	return 0;
}
