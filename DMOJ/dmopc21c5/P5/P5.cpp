#include <bits/stdc++.h>

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

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	const int V = 100;
	std::vector<int> primes; primes.reserve(V);
	std::vector<int> pfac(V+1);
	for (int i = 2; i <= V; i++) {
		if (!pfac[i]) {
			primes.push_back(i);
			pfac[i] = i;
		}
		for (int p : primes) {
			if (int64_t(i) * int64_t(p) > V) break;
			pfac[i*p] = p;
			if (p == pfac[i]) break;
		}
	}

	auto diff_val = [&](int d) {
		d = abs(d);
		return (pfac[d] == d) ? d : 0;
	};

	auto solve = [&](int N) -> std::vector<int> {
		if (N <= 2) {
			std::vector<int> A(N);
			std::iota(A.begin(), A.end(), 0);
			return A;
		}
		if (N == 3) {
			return {0, 2, 1};
		}

		if (N <= 21) {
			tensor<int, 2> dp({1<<N, N}, -1);
			tensor<int, 2> prv({1<<N, N}, -1);
			for (int i = 0; i < N; i++) {
				dp[{1<<i,i}] = 0;
			}
			for (int msk = 1; msk < (1<<N); msk++) {
				for (int i = 0; i < N; i++) {
					if (!(msk & (1 << i))) continue;
					for (int j = 0; j < N; j++) {
						if (msk & (1 << j)) continue;
						int v = dp[{msk,i}] + diff_val(i-j);
						if (v > dp[{msk|(1<<j),j}]) {
							dp[{msk|(1<<j),j}] = v;
							prv[{msk|(1<<j),j}] = i;
						}
					}
				}
			}
			int msk = (1 << N) - 1;
			int cur = 0;
			for (int i = 1; i < N; i++) {
				if (dp[{msk, i}] > dp[{msk, cur}]) cur = i;
			}
			int optimal = 0;
			for (int i = 0; i < 2*N; i++) {
				if (i >= N) {
					optimal += i/2;
				} else {
					optimal -= i/2;
				}
			}
			cerr << "value" << ' ' << dp[{msk, cur}] << ' ' << "optimal" << ' ' << optimal << '\n';
			std::vector<int> A; A.reserve(N);
			A.push_back(cur);
			for (int i = 1; i < N; i++) {
				assert(msk & (1 << cur));
				int nxt = prv[{msk, cur}];
				msk ^= (1 << cur);
				A.push_back(nxt);
				cur = nxt;
			}
			return A;
		}

		std::vector<int> best(N);
		int best_val = N;

		std::vector<int> A(N);
		std::vector<bool> used(N);
		std::y_combinator([&](auto self, int idx) -> void {
			if (idx == N) {
				int i = A[N-1], j = A[0];
				if (2*std::max(i,j) >= N-1 && 2*std::min(i,j) <= N-1) {
					int val = abs(A[N-1] - A[0]);
					if (val < best_val) {
						best_val = val;
						best = A;
					}
				}
				return;
			}
			if (idx == 0) {
				for (int i = (N-1)/2; i >= N/2 - best_val; i--) {
					A[idx] = i;
					used[i] = true;
					self(idx+1);
					used[i] = false;
				}
			} else {
				int i = A[idx-1];
				for (int j = 0; j < N; j++) {
					if (used[j]) continue;
					if (2*std::max(i,j) >= N-1 && 2*std::min(i,j) <= N-1 && pfac[abs(i-j)] == abs(i-j)) {
						A[idx] = j;
						used[j] = true;
						self(idx+1);
						used[j] = false;
					}
				}
			}
		})(0);
		return best;
	};

	{
		int N; if (cin >> N) {
			auto A = solve(N);
			for (int i = 0; i < N; i++) {
				cout << A[i]+1 << " \n"[i+1==N];
			}
			exit(0);
		}
	}

	cout << "#include <bits/stdc++.h>" << '\n';
	cout << "const std::vector<std::string> VALS = {" << '\n';
	cout << "\"\"," << '\n';
	for (int N = 1; N <= 25; N++) {
		cerr << "solving " << N << '\n';
		auto A = solve(N);
		for (int v : A) { cerr << v << ' '; } cerr << '\n';
		for (int i = 0; i+1 < N; i++) { cerr << A[i+1] - A[i] << ' '; } cerr << '\n';

		cout << "\"";
		for (int i = 0; i < N; i++) {
			if (i) cout << ' ';
			cout << A[i]+1;
		}
		cout << "\"," << '\n';
	}
	cout << "};" << '\n';
	cout << "int main() { int N; std::cin >> N; std::cout << VALS[N] << '\\n'; }" << '\n';

	return 0;
}
