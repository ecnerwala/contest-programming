#include <bits/stdc++.h>
#include "aerobatics.h"

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

template <typename T> void setmin(T& a, T b) { if (b < a) a = b; }
template <typename T> void setmax(T& a, T b) { if (b > a) a = b; }

int main(int argc, char** argv) {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	assert(argc >= 2);
	string casename = argv[1];

	int N; double Z0; cin >> N >> Z0;
	vector<array<int, 2>> pts(N); for (auto& p : pts) cin >> p[0] >> p[1];

	auto get_angle = [&](int p0, int p1, int p2) -> double {
		return GetAngle(pts[p1][0], pts[p1][1], pts[p0][0], pts[p0][1], pts[p2][0], pts[p2][1]);
	};
	auto get_val = [&](const vector<int>& a) {
		double ans = 180;
		for (int i = 0; i+2 < int(a.size()); i++) {
			ans = min(ans, get_angle(a[i], a[i+1], a[i+2]));
		}
		return ans;
	};

	if (N <= 15) {
		tensor<double, 3> dp({1<<N, N, N}, 0);
		tensor<int, 3> prv({1<<N, N, N}, -1);
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				dp[{(1<<i) | (1<<j), i, j}] = 180;
			}
		}
		double best_val = -1;
		array<int, 3> best_end{0,0,0};

		for (int m = 0; m < (1 << N); m++) {
			for (int i = 0; i < N; i++) {
				if (!(m & (1 << i))) continue;
				for (int j = 0; j < N; j++) {
					if (i == j) continue;
					if (!(m & (1 << j))) continue;

					if (m == (1 << N)-1) {
						if (dp[{m,i,j}] > best_val) {
							best_val = dp[{m,i,j}];
							best_end = {m,i,j};
						}
						continue;
					}

					for (int k = 0; k < N; k++) {
						if (m & (1 << k)) continue;
						double cnd = min(dp[{m,i,j}], get_angle(i,j,k));
						if (cnd > dp[{m|(1<<k),j,k}]) {
							dp[{m|(1<<k),j,k}] = cnd;
							prv[{m|(1<<k),j,k}] = i;
						}
					}
				}
			}
		}

		cerr << best_val << '\n';
		assert(best_val >= Z0);

		vector<int> ans; ans.reserve(N);
		ans.push_back(best_end[2]);
		ans.push_back(best_end[1]);
		while (int(ans.size()) < N) {
			int i = prv[best_end];
			assert(i != -1);
			ans.push_back(i);
			best_end[0] -= 1 << best_end[2];
			best_end[2] = best_end[1];
			best_end[1] = i;
		}

		for (int i = 0; i < N; i++) {
			cout << ans[i]+1 << '\n';
		}
		exit(0);
	}

	auto dump_solution = [&](const vector<int>& cur, double val) {
		char buf[100];
		std::sprintf(buf, "%08.4f", val);
		string filename = casename + "." + buf + ".out";
		cerr << "dumping to " << filename << '\n';
		ofstream fout(filename);
		for (int i = 0; i < N; i++) {
			fout << cur[i]+1 << '\n';
		}
	};

	double best_val = 0;
	vector<int> best_ans(N); iota(best_ans.begin(), best_ans.end(), 0);

	auto check_solution = [&](const vector<int>& cur, double val) {
		if (val > best_val) {
			best_val = val;
			best_ans = cur;
			dump_solution(cur, val);
		}
	};

	for (int big_iter = 0; big_iter < 100; big_iter++) {
		mt19937 mt(48 + big_iter);
		double target = max(best_val+1, 80.);
		/*
		if (casename == "input_02") {
			target = max(target, 96.);
		} else if (casename == "input_03") {
			target = max(target, 82.);
		} else if (casename == "input_04") {
			target = max(target, 90.);
		} else if (casename == "input_05") {
			target = max(target, 80.);
		} else if (casename == "input_06") {
			target = max(target, 90.);
		}
		*/
		cerr << "trying to attain " << target << '\n';

		int num_iters = 100000;
		vector<int> cur(N); iota(cur.begin(), cur.end(), 0);
		std::y_combinator([&](auto self, int i) -> bool {
			if (i == N) return true;
			if (i >= 2) {
				vector<pair<double, int>> vals; vals.reserve(N-i);
				for (int j = i; j < N; j++) {
					vals.push_back({get_angle(cur[i-2], cur[i-1], cur[j]), cur[j]});
				}
				sort(vals.begin(), vals.end());
				for (int j = i; j < N; j++) {
					cur[j] = vals[j-i].second;
				}
				for (int j = i; j < N; j += 5) {
					std::shuffle(cur.begin()+j, cur.begin()+min(j+5, N), mt);
				}
			} else {
				std::shuffle(cur.begin()+i, cur.end(), mt);
			}
			int j = i;
			while (j < N && num_iters > 0) {
				num_iters--;
				swap(cur[i], cur[j]);
				if (i < 2 || get_angle(cur[i-2], cur[i-1], cur[i]) >= target) {
					if (self(i+1)) return true;
				}
				swap(cur[i], cur[j]);
				j++;
			}
			return false;
		})(0);

		double val = get_val(cur);
		cerr << "got value " << val << '\n';
		if (val < target) continue;
		check_solution(cur, val);

		// do some local optimizations
		vector<pair<double, int>> angles;
		for (int i = 1; i+1 < N; i++) {
			angles.push_back({get_angle(cur[i-1], cur[i], cur[i+1]), i});
		}
		sort(angles.begin(), angles.end());
		for (int z = 0; z < 20; z++) {
			int x = angles[z].second;
			cerr << "moving " << x << '\n';
			vector<int> nxt_cur = cur;
			double nxt_val = val;
			// try to insert x in another location
			for (int y = 0; y < N; y++) {
				vector<int> new_cur = cur;
				new_cur.erase(new_cur.begin()+x);
				new_cur.insert(new_cur.begin()+y, cur[x]);
				double new_val = get_val(new_cur);
				if (new_val > nxt_val) {
					cerr << "Found better location" << '\n';
					nxt_val = new_val;
					nxt_cur = new_cur;
					check_solution(new_cur, new_val);
				}
			}
			cur = nxt_cur;
			val = nxt_val;
		}
	}

	cerr << "best val: " << best_val << '\n';
	for (int i = 0; i < N; i++) {
		cout << best_ans[i]+1 << '\n';
	}

	return 0;
}
