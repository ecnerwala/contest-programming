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

int64_t f(int64_t N) {
	std::cerr << "solve " << N << '\n';
	for (int steps = 0; true; steps++) {
		std::cerr << "trying steps " << steps << '\n';
		std::pair<int, int64_t> ans = {steps, N+1};
		int d = (steps + 1) / 2;
		[&](this auto&& self, int64_t a, int64_t b, int64_t ca, int64_t cb, int cur_depth) -> void {
			// TODO: Maybe we should prune a,b which are too far away
			assert(a <= b);
			assert(cb >= 0);
			if (ca < 0) {
				// ca += k * b
				int64_t k = (-ca + b-1) / b;
				ca += k * b;
				cb -= k * a;
				if (cb < 0) return;
			}
			assert(ca >= 0 && cb >= 0 && a * ca + b * cb == N);
			if (cur_depth == d) {
				if (steps & 1) {
					assert(a <= b);
					// cb * b + ca * a = cb * (b - 0.5a) + (ca + 0.5 cb) * a
					// (ca + 0.5 cb)^2 + cb^2 <= (b - 0.5a)^2 + a^2
					// We want to go half a step back/forwards
					a *= 2;
					b *= 2;
					ca *= 2;
					cb *= 2;
					b -= a / 2;
					ca += cb / 2;
					assert(a * ca + b * cb == 4 * N);
				}

				int64_t norm = a * a + b * b;
				if (norm < N) return;

				int64_t cross = ca * b - cb * a;
				{
					int64_t k = cross / norm;
					ca -= k * b;
					cb += k * a;
					cross -= k * norm;
				}
				assert(cross == ca * b - cb * a);
				if (cross < 0) {
					ca += b;
					cb -= a;
					cross += norm;
				}
				assert(cross == ca * b - cb * a);
				assert(cross >= 0);

				auto check = [&]() -> void {
					if (ca < 0 || cb < 0) return;
					if (ca * ca + cb * cb > norm) return;
					int64_t x = ca, y = cb;
					int64_t vx = a, vy = b;
					if (steps & 1) {
						// half a step forwards again
						assert(!(vx & 1));
						if (y & 1) return;
						x -= y / 2;
						vy += vx / 2;
						assert(!(vy & 1));
						if (x & 1) return;
						x /= 2, y /= 2, vx /= 2, vy /= 2;
					}
					assert(x * vx + y * vy == N);
					int ops = 0;
					for (; ops <= steps - d && x && y; ops++) {
						if (x > y) std::swap(x, y), std::swap(vx, vy);
						y -= x;
						vx += vy;
						assert(vx * x + vy * y == N);
					}
					if (ops <= steps - d) {
						assert(vx == N || vy == N);
						int64_t o = vx + vy - N;
						// Need to check the inverse too, since we could be doing it backwards.
						int64_t o2 = mod_inv_in_range(o, N);
						std::cerr << "found " << a << ' ' << b << ' ' << ca << ' ' << cb << ' ' << vx << ' ' << vy << ' ' << cur_depth << ' ' << ops << ' ' << std::min(o, N-o) << ' ' << std::min(o2, N-o2) << '\n';
						ans = std::min(ans, {cur_depth + ops, std::min({o, N-o, o2, N-o2})});
					}
				};


				check();
				ca -= b;
				cb += a;
				check();
				return;
			}

			// Check if we can hit the norm bound within d steps
			{
				int64_t x = a, y = b;
				assert(cur_depth <= steps/2);
				for (int z = cur_depth; z < steps / 2; z++) {
					assert(x <= y);
					x += y;
					std::swap(x, y);
				}
				assert(x <= y);
				if (steps & 1) {
					if (5 * y * y / 4 + x * y + x * x < N) return;
				} else {
					if (x * x + y * y < N) return;
				}
			}
			self(b, a+b, cb - ca, ca, cur_depth + 1);
			if (0 < a && a < b) self(a, a+b, ca - cb, cb, cur_depth + 1);
		}(0, 1, 0, N, 0);
		if (ans.second <= N) {
			std::cerr << "final steps " << ans.first - 1 << '\n';
			return ans.second;
		}
	}
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	const int V = int(1e7) + 2;
	std::vector<int> pfac(V+1);
	std::vector<int> primes; primes.reserve(V);
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

	auto solve_p = [&](int p) -> void {
		cerr << p << ' ' << f(p) << '\n';
		return;
		// solve p
		std::pair<int, int> best(2*p+1, 1);
		for (int z = 1; z < p-1; z++) {
			int ops = 2;
			int a = z, b = p - z;
			while (a != b) {
				if (a < b) {std::swap(a, b); ops++;}
				assert(a > b);
				a -= b;
				std::swap(a, b);
				ops++;
			}
			best = std::min(best, {ops, z});
		}
		std::cerr << p << ' ' << best.second << ' ' << best.first << '\n';
	};
	for (auto p : primes) {
		solve_p(p);
	}

	int T; cin >> T;
	while (T--) {
		int X; cin >> X;
		const int N_MAX = 50;
		int N = 0;
		tensor<int, 2> ans({N_MAX, N_MAX}, 0);
		for (int v = X; v > 1; v /= pfac[v]) {
			int p = pfac[v];
		}
		assert(N <= N_MAX);
		cout << N << '\n';
	}

	return 0;
}
