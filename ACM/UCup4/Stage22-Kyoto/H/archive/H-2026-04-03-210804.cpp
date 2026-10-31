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

namespace tourist {

using namespace std;

template <typename T>
	T inverse(T a, T m) {
		T u = 0, v = 1;
		while (a != 0) {
			T t = m / a;
			m -= t * a; swap(a, m);
			u -= t * v; swap(u, v);
		}
		assert(m == 1);
		return u;
	}

template <typename T>
	class Modular {
	public:
		using Type = typename decay<decltype(T::value)>::type;

		constexpr Modular() : value() {}
		template <typename U>
			Modular(const U& x) {
				value = normalize(x);
			}

		template <typename U>
			static Type normalize(const U& x) {
				Type v;
				if (-mod() <= x && x < mod()) v = static_cast<Type>(x);
				else v = static_cast<Type>(x % mod());
				if (v < 0) v += mod();
				return v;
			}

		const Type& operator()() const { return value; }
		template <typename U>
			explicit operator U() const { return static_cast<U>(value); }
		constexpr static Type mod() { return T::value; }

		Modular& operator+=(const Modular& other) { value += other.value; value -= (value >= mod()) * mod(); return *this; }
		Modular& operator-=(const Modular& other) { value -= other.value; value += (value < 0) * mod(); return *this; }
		template <typename U> Modular& operator+=(const U& other) { return *this += Modular(other); }
		template <typename U> Modular& operator-=(const U& other) { return *this -= Modular(other); }
		Modular& operator++() { return *this += 1; }
		Modular& operator--() { return *this -= 1; }
		Modular operator++(int) { Modular result(*this); *this += 1; return result; }
		Modular operator--(int) { Modular result(*this); *this -= 1; return result; }
		Modular operator-() const { return Modular(-value); }

		template <typename U = T>
			typename enable_if<is_same<typename Modular<U>::Type, int>::value, Modular>::type& operator*=(const Modular& rhs) {
				value = normalize(static_cast<int64_t>(value) * static_cast<int64_t>(rhs.value));
				return *this;
			}
		template <typename U = T>
			typename enable_if<is_same<typename Modular<U>::Type, int64_t>::value, Modular>::type& operator*=(const Modular& rhs) {
				__int128_t res = __int128_t(value) * __int128_t(rhs.value) % __int128_t(mod());
				value = int64_t(res);
				return *this;
			}
		template <typename U = T>
			typename enable_if<!is_integral<typename Modular<U>::Type>::value, Modular>::type& operator*=(const Modular& rhs) {
				value = normalize(value * rhs.value);
				return *this;
			}

		Modular& operator/=(const Modular& other) { return *this *= Modular(inverse(other.value, mod())); }

		friend const Type& abs(const Modular& x) { return x.value; }

		template <typename U>
			friend bool operator==(const Modular<U>& lhs, const Modular<U>& rhs);

		template <typename U>
			friend bool operator<(const Modular<U>& lhs, const Modular<U>& rhs);

		template <typename V, typename U>
			friend V& operator>>(V& stream, Modular<U>& number);

	private:
		Type value;
	};

template <typename T> bool operator==(const Modular<T>& lhs, const Modular<T>& rhs) { return lhs.value == rhs.value; }
template <typename T, typename U> bool operator==(const Modular<T>& lhs, U rhs) { return lhs == Modular<T>(rhs); }
template <typename T, typename U> bool operator==(U lhs, const Modular<T>& rhs) { return Modular<T>(lhs) == rhs; }

template <typename T> bool operator!=(const Modular<T>& lhs, const Modular<T>& rhs) { return !(lhs == rhs); }
template <typename T, typename U> bool operator!=(const Modular<T>& lhs, U rhs) { return !(lhs == rhs); }
template <typename T, typename U> bool operator!=(U lhs, const Modular<T>& rhs) { return !(lhs == rhs); }

template <typename T> bool operator<(const Modular<T>& lhs, const Modular<T>& rhs) { return lhs.value < rhs.value; }

template <typename T> Modular<T> operator+(const Modular<T>& lhs, const Modular<T>& rhs) { return Modular<T>(lhs) += rhs; }
template <typename T, typename U> Modular<T> operator+(const Modular<T>& lhs, U rhs) { return Modular<T>(lhs) += rhs; }
template <typename T, typename U> Modular<T> operator+(U lhs, const Modular<T>& rhs) { return Modular<T>(lhs) += rhs; }

template <typename T> Modular<T> operator-(const Modular<T>& lhs, const Modular<T>& rhs) { return Modular<T>(lhs) -= rhs; }
template <typename T, typename U> Modular<T> operator-(const Modular<T>& lhs, U rhs) { return Modular<T>(lhs) -= rhs; }
template <typename T, typename U> Modular<T> operator-(U lhs, const Modular<T>& rhs) { return Modular<T>(lhs) -= rhs; }

template <typename T> Modular<T> operator*(const Modular<T>& lhs, const Modular<T>& rhs) { return Modular<T>(lhs) *= rhs; }
template <typename T, typename U> Modular<T> operator*(const Modular<T>& lhs, U rhs) { return Modular<T>(lhs) *= rhs; }
template <typename T, typename U> Modular<T> operator*(U lhs, const Modular<T>& rhs) { return Modular<T>(lhs) *= rhs; }

template <typename T> Modular<T> operator/(const Modular<T>& lhs, const Modular<T>& rhs) { return Modular<T>(lhs) /= rhs; }
template <typename T, typename U> Modular<T> operator/(const Modular<T>& lhs, U rhs) { return Modular<T>(lhs) /= rhs; }
template <typename T, typename U> Modular<T> operator/(U lhs, const Modular<T>& rhs) { return Modular<T>(lhs) /= rhs; }

template<typename T, typename U>
	Modular<T> power(const Modular<T>& a, const U& b) {
		assert(b >= 0);
		Modular<T> x = a, res = 1;
		U p = b;
		while (p > 0) {
			if (p & 1) res *= x;
			x *= x;
			p >>= 1;
		}
		return res;
	}

template <typename T>
	bool IsZero(const Modular<T>& number) {
		return number() == 0;
	}

template <typename T>
	string to_string(const Modular<T>& number) {
		return to_string(number());
	}

// U == std::ostream? but done this way because of fastoutput
template <typename U, typename T>
	U& operator<<(U& stream, const Modular<T>& number) {
		return stream << number();
	}

// U == std::istream? but done this way because of fastinput
template <typename U, typename T>
	U& operator>>(U& stream, Modular<T>& number) {
		typename common_type<typename Modular<T>::Type, int64_t>::type x;
		stream >> x;
		number.value = Modular<T>::normalize(x);
		return stream;
	}

// using ModType = int;

// struct VarMod { static ModType value; };
// ModType VarMod::value;
// ModType& md = VarMod::value;
// using Mint = Modular<VarMod>;

constexpr int64_t md = (int64_t(1) << 61) - 1;
using Mint = Modular<std::integral_constant<decay<decltype(md)>::type, md>>;

}

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	using num = tourist::Mint;

	auto query_tree = [&](std::vector<std::array<int, 2>> edges) -> std::vector<num> {
		int N = int(edges.size()) + 1;
		tensor<int, 2> dist({N, N}, N+1);
		for (int i = 0; i < N; i++) dist[{i, i}] = 0;
		for (auto [u, v] : edges) { dist[{u, v}] = dist[{v, u}] = 1; }
		for (int k = 0; k < N; k++) {
			for (int i = 0; i < N; i++) {
				for (int j = 0; j < N; j++) {
					dist[{i, j}] = std::min(dist[{i, j}], dist[{i, k}] + dist[{k, j}]);
				}
			}
		}

		tensor<num, 2> mat({N, N}, 0);
		for (int i = 0; i < N; i++) mat[{i, i}] = 1;
		std::vector<num> res(N+1);
		for (int l = 0; l <= N; l++) {
			for (int i = 0; i < N; i++) {
				res[l] += mat[{i, i}];
			}
			{
				tensor<num, 2> mat2({N, N}, 0);
				for (int i = 0; i < N; i++) {
					for (int j = 0; j < N; j++) {
						for (int k = 0; k < N; k++) {
							mat2[{i, k}] += mat[{i, j}] * num(dist[{j, k}]);
						}
					}
				}
				std::swap(mat, mat2);
			}
		}
		return res;
	};

	auto newton_to_elem = [&](std::vector<num> pows) -> std::vector<num> {
		int N = int(pows.size()) - 1;
		for (int i = 0; i <= N; i++) {
			if (i & 1) pows[i] = -pows[i];
		}

		std::vector<num> elems(N+1, num(0));
		elems[0] = 1;
		for (int i = 1; i <= N; i++) {
			for (int j = 0; j < i; j++) {
				elems[i] -= elems[j] * pows[i-j];
			}
			elems[i] /= num(i);
		}
		return elems;
	};

	std::mt19937 mt(48);
	auto gen_tree = [&](int N) -> std::vector<std::array<int, 2>> {
		std::vector<std::array<int, 2>> edges; edges.reserve(N);
		for (int i = 1; i < N; i++) {
			edges.push_back({std::uniform_int_distribution<int>(0, i-1)(mt), i});
		}
		return edges;
	};

	for (int N = 2; N <= 10; N++) {
		num tgt = -num(N-1) * power(num(-2), N-2);
		for (int z = 0; z <= 20; z++) {
			std::cerr << N << ' ' << newton_to_elem(query_tree(gen_tree(N))).back() << ' ' << tgt << '\n';
		}
	}

	int N; std::cin >> N;

	return 0;
}
