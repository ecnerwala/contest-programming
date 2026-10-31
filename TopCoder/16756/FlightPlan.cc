#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

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

class FlightPlan {

public:

	long long fly(int R, int C, vector <int> H, int cup_, int cdn_, int clr_) {
		int64_t cup = cup_;
		int64_t cdn = cdn_;
		int64_t clr = clr_;
		vector<int> heights = H;
		sort(heights.begin(), heights.end());
		heights.resize(unique(heights.begin(), heights.end()) - heights.begin());
		int S = int(heights.size());
		for (auto& h : H) h = int(lower_bound(heights.begin(), heights.end(), h) - heights.begin());

		tensor<int64_t, 3> dist({R, C, S}, -1);
		priority_queue<pair<int64_t, array<int, 3>>, vector<pair<int64_t, array<int, 3>>>, greater<pair<int64_t, array<int, 3>>>> pq;
		auto check_dist = [&](array<int, 3> pos, int64_t d) {
			auto& v = dist[pos];
			if (v == -1 || d < v) {
				v = d;
				pq.push({d, pos});
			}
		};

		check_dist({0,0,H[0]}, 0);

		while (!pq.empty()) {
			auto d = pq.top().first;
			auto cur = pq.top().second;
			pq.pop();
			if (d != dist[cur]) continue;
			// up
			if (cur[2]+1 < S) {
				auto nxt = cur; nxt[2]++;
				check_dist(nxt, d + int64_t(cup) * (heights[nxt[2]] - heights[cur[2]]));
			}
			// down if legal
			if (cur[2]-1 >= H[cur[0]*C+cur[1]]) {
				auto nxt = cur; nxt[2]--;
				check_dist(nxt, d + int64_t(cdn) * (heights[cur[2]] - heights[nxt[2]]));
			}
			// 4 directions
			for (auto dir : {array<int, 2>{0, 1}, {1, 0}, {0, -1}, {-1, 0}}) {
				auto nxt = cur; nxt[0] += dir[0]; nxt[1] += dir[1];
				if (0 <= nxt[0] && nxt[0] < R && 0 <= nxt[1] && nxt[1] < C && nxt[2] >= H[nxt[0]*C+nxt[1]]) {
					check_dist(nxt, d + clr);
				}
			}
		}
		return dist[{R-1,C-1,H[R*C-1]}];
	}
};

void my_tests() {
	mt19937 mt(48);
	vector<int> H(50*50);
	for (auto& h : H) h = mt() % int(1e6);
	cerr << FlightPlan().fly(50, 50, H, 1, 1, 1000000) << '\n';
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::FlightPlan;

// vim:ft=cpp
