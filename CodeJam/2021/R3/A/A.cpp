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

template <typename T> void setmin(T& a, T b) { if (b < a) a = b; }
template <typename T> void setmax(T& a, T b) { if (b > a) a = b; }

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		string S; cin >> S;
		sort(S.begin(), S.end());

		const int64_t INF = 2e18;
		int64_t ans = INF;
		if (int(S.size()) & 1) {
			string A, B; A.reserve(S.size()), B.reserve(S.size());
			for (int i = 0; true; i++) {
				assert(i < int(S.size()));
				if (S[i] != '0') {
					A.push_back(S[i]);
					S.erase(S.begin()+i);
					break;
				}
			}
			int L = int(S.size())/2;
			A.insert(A.end(), S.begin(), S.begin()+L);
			B.insert(B.end(), S.rbegin(), S.rbegin()+L);
			assert(int(A.size()) == int(B.size())+1);
			ans = abs(stoll(A) - stoll(B));
		} else {
			string A; A.reserve(S.size());

			array<int, 10> cnt; cnt.fill(0);
			for (char c : S) { cnt[c - '0']++; }

			bool all_even = true;
			for (int i = 0; i < 10; i++) {
				all_even = all_even && (cnt[i] % 2 == 0);
			}
			if (all_even) {
				ans = 0;
			}

			for (int a = 0; a < 10; a++) {
				for (int b = a+1; b < 10; b++) {
					if (!cnt[a] || !cnt[b]) continue;
					cnt[a]--, cnt[b]--;
					{
						A.clear();
						for (int i = 0; i < 10; i++) {
							int v = cnt[i];
							while (v--) A.push_back(char('0' + i));
						}
						int L = int(A.size());
						assert(!(L & 1));
						assert(L == int(S.size())-2);

						tensor<int64_t, 4> dp({L+1, L+1, 2, 2}, INF);
						dp[{0, L, false, false}] = b - a;
						for (int l = L; l > 0; l -= 2) {
							assert(l >= 2);
							for (int i = 0, j = l; j <= L; i++, j++) {
								for (int z = 0; z < 2; z++) {
									for (int y = 0; y < 2; y++) {
										int64_t v = dp[{i, j, z, y}];
										if (v == INF) continue;
										assert(v > 0);
										if (A[i] == A[i+1]) {
											setmin(dp[{i+2, j, z || (A[i] > '0'), y || (A[i] == '0')}], v);
										}
										if (A[j-1] == A[j-2]) {
											setmin(dp[{i, j-2, z || (A[j-1] > '0'), y || (A[j-1] == '0')}], v);
										}
										setmin(dp[{i+1, j-1, z, y}], v * 10 + (A[i] - A[j-1]));
									}
								}
							}
						}
						for (int i = 0; i <= L; i++) {
							setmin(ans, dp[{i, i, 1, 1}]);
							setmin(ans, dp[{i, i, 1, 0}]);
							if (a > 0) {
								setmin(ans, dp[{i, i, 0, 0}]);
							}
						}
					}
					cnt[a]++, cnt[b]++;
				}
			}
			assert(ans != INF);
		}

		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}
