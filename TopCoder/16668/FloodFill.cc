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

class FloodFill {

public:

	vector <int> getCell(vector <int> X, vector <int> Y, long long A) {
		int N = int(X.size());
		assert(N == int(Y.size()));
		A += N;

		auto get_area = [&](int L) -> int64_t {
			vector<int> x_coords; x_coords.reserve(2*N);
			vector<int> y_coords; y_coords.reserve(2*N);
			for (int i = 0; i < N; i++) {
				x_coords.push_back(X[i]-L);
				x_coords.push_back(X[i]+L+1);
				y_coords.push_back(Y[i]-L);
				y_coords.push_back(Y[i]+L+1);
			}
			sort(x_coords.begin(), x_coords.end());
			sort(y_coords.begin(), y_coords.end());
			if (x_coords.empty()) return 0;
			tensor<bool, 2> filled({int(x_coords.size())-1, int(y_coords.size())-1});
			for (int i = 0; i < N; i++) {
				int xlo = int(lower_bound(x_coords.begin(), x_coords.end(), X[i]-L) - x_coords.begin());
				int xhi = int(lower_bound(x_coords.begin(), x_coords.end(), X[i]+L+1) - x_coords.begin());
				int ylo = int(lower_bound(y_coords.begin(), y_coords.end(), Y[i]-L) - y_coords.begin());
				int yhi = int(lower_bound(y_coords.begin(), y_coords.end(), Y[i]+L+1) - y_coords.begin());
				for (int x = xlo; x < xhi; x++) {
					for (int y = ylo; y < yhi; y++) {
						filled[{x,y}] = true;
					}
				}
			}
			int64_t ans = 0;
			for (int x = 0; x+1 < int(x_coords.size()); x++) {
				for (int y = 0; y+1 < int(y_coords.size()); y++) {
					if (filled[{x,y}]) {
						ans += int64_t(x_coords[x+1] - x_coords[x]) * int64_t(y_coords[y+1] - y_coords[y]);
					}
				}
			}
			return ans;
		};

		int mi = -1;
		int ma = int(5e8);
		assert(int64_t(2 * ma + 1) * int64_t(2 * ma + 1) >= A);
		while (ma - mi > 1) {
			int md = mi + (ma - mi) / 2;
			if (get_area(md) >= A) {
				ma = md;
			} else {
				mi = md;
			}
		}

		// now we just need to patch up the rest
		{
			vector<int> x_coords; x_coords.reserve(4*N);
			vector<int> y_coords; y_coords.reserve(4*N);

			int L = mi;
			for (int i = 0; i < N; i++) {
				x_coords.push_back(X[i]-L);
				x_coords.push_back(X[i]+L+1);
				y_coords.push_back(Y[i]-L);
				y_coords.push_back(Y[i]+L+1);
			}
			L = ma;
			for (int i = 0; i < N; i++) {
				x_coords.push_back(X[i]-L);
				x_coords.push_back(X[i]+L+1);
				y_coords.push_back(Y[i]-L);
				y_coords.push_back(Y[i]+L+1);
			}

			sort(x_coords.begin(), x_coords.end());
			sort(y_coords.begin(), y_coords.end());
			assert(!x_coords.empty());
			tensor<char, 2> filled({int(x_coords.size())-1, int(y_coords.size())-1}, 0);

			L = ma;
			for (int i = 0; i < N; i++) {
				int xlo = int(lower_bound(x_coords.begin(), x_coords.end(), X[i]-L) - x_coords.begin());
				int xhi = int(lower_bound(x_coords.begin(), x_coords.end(), X[i]+L+1) - x_coords.begin());
				int ylo = int(lower_bound(y_coords.begin(), y_coords.end(), Y[i]-L) - y_coords.begin());
				int yhi = int(lower_bound(y_coords.begin(), y_coords.end(), Y[i]+L+1) - y_coords.begin());
				for (int x = xlo; x < xhi; x++) {
					for (int y = ylo; y < yhi; y++) {
						filled[{x,y}] = 2;
					}
				}
			}

			L = mi;
			for (int i = 0; i < N; i++) {
				int xlo = int(lower_bound(x_coords.begin(), x_coords.end(), X[i]-L) - x_coords.begin());
				int xhi = int(lower_bound(x_coords.begin(), x_coords.end(), X[i]+L+1) - x_coords.begin());
				int ylo = int(lower_bound(y_coords.begin(), y_coords.end(), Y[i]-L) - y_coords.begin());
				int yhi = int(lower_bound(y_coords.begin(), y_coords.end(), Y[i]+L+1) - y_coords.begin());
				for (int x = xlo; x < xhi; x++) {
					for (int y = ylo; y < yhi; y++) {
						filled[{x,y}] = 1;
					}
				}
			}

			int64_t ans = 0;
			for (int x = 0; x+1 < int(x_coords.size()); x++) {
				for (int y = 0; y+1 < int(y_coords.size()); y++) {
					if (filled[{x,y}] == 1) {
						ans += int64_t(x_coords[x+1] - x_coords[x]) * int64_t(y_coords[y+1] - y_coords[y]);
					}
				}
			}

			assert(ans < A);

			for (int x = 0; x+1 < int(x_coords.size()); x++) {
				int64_t tot_y = 0;
				for (int y = 0; y+1 < int(y_coords.size()); y++) {
					if (filled[{x,y}] == 2) {
						tot_y += int64_t(y_coords[y+1] - y_coords[y]);
					}
				}
				int64_t cur = int64_t(x_coords[x+1] - x_coords[x]) * tot_y;
				if (ans + cur < A) {
					ans += cur;
					continue;
				}
				assert(ans + cur >= A);
				// otherwise the current x coordinate is correct
				int64_t idx = (A - ans - 1);
				int64_t x_idx = idx / tot_y;
				int64_t y_idx = idx % tot_y;
				for (int y = 0; y+1 < int(y_coords.size()); y++) {
					if (filled[{x,y}] == 2) {
						int64_t cur_y = (y_coords[y+1] - y_coords[y]);
						if (y_idx < cur_y) {
							return vector<int>({int(x_coords[x] + x_idx), int(y_coords[y] + y_idx)});
						} else {
							y_idx -= cur_y;
						}
					}
				}
				assert(false);
			}
			assert(false);
		}
	}
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::FloodFill;

// vim:ft=cpp
