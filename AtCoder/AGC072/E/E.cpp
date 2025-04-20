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
#ifdef _GLIBCXX_DEBUG
		return data[flatten_index_checked(idx)];
#else
		return data[flatten_index(idx)];
#endif
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

	int CASES; cin >> CASES;
	while (CASES--) {
		int N, M; cin >> N >> M;
		int F; cin >> F;
		const int INF = 1e9;
		tensor<int, 2> dist({N, N}, INF);
		for (int e = 0; e < M; e++) {
			int a, b, c; cin >> a >> b >> c; a--, b--;
			assert(a != b);
			assert((dist[{a, b}] == INF));
			dist[{a, b}] = c;
		}
		std::vector<int> R(N);
		for (auto& r : R) cin >> r;

		for (int i = 0; i < N; i++) {
			dist[{i, i}] = 0;
		}

		// Transitive closure, which is legal
		for (int k = 0; k < N; k++) {
			for (int i = 0; i < N; i++) {
				for (int j = 0; j < N; j++) {
					dist[{i, j}] = std::min(dist[{i, j}], dist[{i, k}] + dist[{k, j}]);
				}
			}
		}

		// we get to each state with some amount of miles and some amount of money
		// If we have miles, we can redeem them all for money
		// or redeem some (nonzero) number of them, which means we must spend all our money by the next destination, leaving us with only miles
		// Thus, we have edges of the form
		// (cur, miles) -> (cur, money)
		// (cur, miles) -> (nxt, miles - dist[cur, nxt] * (F / R[cur] - 1))
		// (cur, money) -> (nxt, money - dist[cur, nxt] * F, miles = dist[cur, nxt]) -> (nxt, money - dist[cur, nxt] * (F - R[nxt]))
		// (cur, money) -> (nxt, money - dist[cur, nxt] * F, miles = dist[cur, nxt]) -> (nxtnxt, miles = dist[cur][nxt] + dist[cur, nxt] - something with rate[nxt])
		
		//cerr << fixed << setprecision(10);
		cout << fixed << setprecision(20) << [&]() -> double {
			assert((dist[{0, N-1}] != INF));
			std::vector<bool> vis_money(N, false);
			std::vector<bool> vis_miles(N, false);
			std::vector<double> min_miles(N, -1);
			std::vector<double> min_money(N, -1);
			using state_t = std::tuple<double, int, bool>;
			auto check_money = [&](int i, double money) -> void {
				if (min_money[i] < 0 || money < min_money[i]) {
					//cerr << "check money " << i << ' ' << money << '\n';
					min_money[i] = money;
				}
			};
			auto check_miles = [&](int i, double miles) -> void {
				if (min_miles[i] < 0 || miles < min_miles[i]) {
					//cerr << "check miles " << i << ' ' << miles << '\n';
					min_miles[i] = miles;
				}
			};

			check_money(N-1, 0.);
			check_miles(N-1, 0.);

			while (!vis_money[0]) {
				int cur = -1;
				bool is_money = false;
				{
					double best = 1e99;
					for (int i = 0; i < N; i++) {
						if (!vis_money[i] && min_money[i] >= 0 && min_money[i] < best) {
							best = min_money[i];
							cur = i;
							is_money = true;
						}
						if (!vis_miles[i] && min_miles[i] >= 0 && min_miles[i] < best) {
							best = min_miles[i];
							cur = i;
							is_money = false;
						}
					}
					assert(cur != -1);
				}
				if (is_money) {
					auto money = min_money[cur];
					if (vis_money[cur]) continue;
					vis_money[cur] = true;

					//cerr << "money " << cur << ' ' << money << '\n';

					if (R[cur] > 0) check_miles(cur, money / R[cur]);

					for (int prv = 0; prv < N; prv++) {
						if (dist[{prv, cur}] == INF) continue;
						int d = dist[{prv, cur}];
						check_money(prv, d * F + std::max(0., money - d * R[cur]));
					}
				} else {
					auto miles = min_miles[cur];
					if (vis_miles[cur]) continue;
					vis_miles[cur] = true;

					//cerr << "miles " << cur << ' ' << miles << '\n';

					for (int prv = 0; prv < N; prv++) {
						if (dist[{prv, cur}] == INF) continue;

						int b = dist[{prv, cur}];

						double pmiles = std::max(miles - b, 0.);

						//cerr << "intermediate " << prv << '\n';

						if (R[prv]) {
							check_miles(prv, pmiles + double(b) * (double(F) / R[prv]));
						}

						for (int prvprv = 0; prvprv < N; prvprv++) {
							if (dist[{prvprv, prv}] == INF) continue;
							// we spend all our money on (dist[{prvprv, prv}] + dist[{prv, cur}]) * F and also some of our miles from the first leg, but none of our last leg miles
							int a = dist[{prvprv, prv}];
							if (a + b >= miles) {
								double money = a * F + std::max(0., b * F - (a - pmiles) * R[prv]);
								check_money(prvprv, money);
							}
						}
					}
				}
			}

			return min_money[0];
		}() << '\n';
	}

	return 0;
}
