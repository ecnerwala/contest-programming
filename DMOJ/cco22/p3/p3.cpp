#include <bits/stdc++.h>

template <typename T> using min_pq = std::priority_queue<T, std::vector<T>, std::greater<T>>;

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

	std::array<int, 2> N; int64_t K; cin >> N[0] >> N[1] >> K;
	std::array<std::vector<std::array<int64_t, 2>>, 2> slides;
	for (int z = 0; z < 2; z++) {
		slides[z].resize(N[z]);
		for (auto& [x, y] : slides[z]) cin >> x >> y;
		std::sort(slides[z].begin(), slides[z].end());
		for (int i = 0; i+1 < int(slides[z].size()); i++) {
			assert(slides[z][i][1] <= slides[z][i+1][0]);
		}
	}

	int ans = 0;
	tensor<int64_t, 3> dist({N[0] + N[1] + 1, 2, 2}, -1);

	int initial_cnt = N[0] > 0 && slides[0][0][0] == 0;
	min_pq<std::pair<int64_t, std::array<int, 3>>> pq;
	pq.push({dist[{initial_cnt, 0, 0}] = 0, {initial_cnt, 0, 0}});


	while (!pq.empty()) {
		int64_t t = pq.top().first;
		const auto state = pq.top().second;
		pq.pop();
		if (t != dist[state]) continue;
		//cerr << "time " << t << " seen " << state[0] << " on side " << state[1] << " seen other " << state[2] << '\n';

		ans = std::max(ans, state[0]);
		std::array<int, 2> nxt_idx;
		for (int z = 0; z < 2; z++) {
			nxt_idx[z] = int(std::lower_bound(slides[z].begin(), slides[z].end(), std::array<int64_t, 2>{t+1, -1}) - slides[z].begin());
		}
		//cerr << " nxt_idx " << nxt_idx[0] << ' ' << nxt_idx[1] << '\n';

		int cur_side = state[1];

		// we can stay on this side until the next slide
		if (nxt_idx[cur_side] < N[cur_side]) {
			int64_t nt = slides[cur_side][nxt_idx[cur_side]][0];
			std::array<int, 3> nstate{state[0] + 1, cur_side, state[2] && nt < slides[!cur_side][nxt_idx[!cur_side]-1][1]};
			if (dist[nstate] == -1 || nt < dist[nstate]) {
				pq.push({dist[nstate] = nt, nstate});
			}
		}

		// we can go to the other side
		{
			int64_t nt = t + K;
			std::array<int, 3> nstate;
			nstate[0] = state[0];
			if (state[2] && nt < slides[!cur_side][nxt_idx[!cur_side]-1][1]) {
				// we've already seen it
			} else {
				int idx = int(std::lower_bound(slides[!cur_side].begin(), slides[!cur_side].end(), std::array<int64_t, 2>{nt+1, -1}) - slides[!cur_side].begin()) - 1;
				//cerr << " idx " << idx << '\n';
				if (idx >= 0) {
					assert(nt >= slides[!cur_side][idx][0]);
					nstate[0] += nt < slides[!cur_side][idx][1];
				}
			}
			nstate[1] = !cur_side;
			nstate[2] = nxt_idx[cur_side]-1 >= 0 && nt < slides[cur_side][nxt_idx[cur_side]-1][1];
			if (dist[nstate] == -1 || nt < dist[nstate]) {
				pq.push({dist[nstate] = nt, nstate});
			}
		}
	}

	cout << ans << '\n';

	return 0;
}
