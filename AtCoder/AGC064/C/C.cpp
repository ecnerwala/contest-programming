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

// the state includes your current depth and a current suffix which matches some things
struct state_t {
	int depth;
	int suff_depth;
	int64_t cur_suff;

	friend auto operator <=> (state_t a, state_t b) = default;
	friend bool operator == (state_t a, state_t b) = default;
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		const int MAXB = 60;
		// bucket of size 2^b
		std::vector<std::vector<int64_t>> buckets(MAXB);

		int B = 0;

		int N; cin >> N;
		for (int i = 0; i < N; i++) {
			int64_t L, R; cin >> L >> R;
			R++;
			for (int b = 0; L < R; b++, L >>= 1, R >>= 1) {
				if (L & 1) {
					buckets[b].push_back(L);
					//cerr << "Bucket " << b << ' ' << L << '\n';
					B = std::max(B, b);
					L++;
				}
				if (R & 1) {
					R--;
					buckets[b].push_back(R);
					//cerr << "Bucket " << b << ' ' << L << '\n';
					B = std::max(B, b);
				}
			}
		}
		assert(B < MAXB);
		buckets.resize(B+1);

		for (auto& b : buckets) {
			for (auto& v : b) {
				int64_t nv = 0;
				for (int i = 0; i < 62; i++) {
					nv <<= 1;
					nv |= (v >> i) & 1;
				}
				v = nv;
			}
			std::sort(b.begin(), b.end());
		}

		std::map<state_t, bool> mem;

		// returns true if winning
		bool ans = std::y_combinator([&](auto self, state_t cur) -> bool {
			assert(cur.suff_depth <= cur.depth);
			if (cur.suff_depth > B) {
				return false;
			}
			{
				auto it = mem.find(cur);
				if (it != mem.end()) return it->second;
			}
			std::array<state_t, 2> nxt_states;
			for (int z = 0; z < 2; z++) {
				state_t nxt = cur;
				nxt.depth++;
				nxt.cur_suff += int64_t(z) << (62 - (nxt.depth - nxt.suff_depth));
				while (nxt.suff_depth < nxt.depth && (
						nxt.suff_depth > B ||
						std::lower_bound(buckets[nxt.suff_depth].begin(), buckets[nxt.suff_depth].end(), nxt.cur_suff) ==
						std::lower_bound(buckets[nxt.suff_depth].begin(), buckets[nxt.suff_depth].end(), nxt.cur_suff + (int64_t(1) << (62 - (nxt.depth - nxt.suff_depth))))
				)) {
					nxt.suff_depth++;
					nxt.cur_suff = (nxt.cur_suff << 1) & ((int64_t(1) << 62) - 1);
				}
				nxt_states[z] = nxt;
				//cerr << cur.depth << ',' << cur.suff_depth << ',' << cur.cur_suff << " + " << z << " => ";
				//cerr << nxt.depth << ',' << nxt.suff_depth << ',' << nxt.cur_suff << '\n';
			}
			bool winning = (nxt_states[0].suff_depth > B || nxt_states[1].suff_depth > B);
			if (!winning) {
				winning = !self(nxt_states[0]) || !self(nxt_states[1]);
			}
			//cerr << cur.depth << ',' << cur.suff_depth << ',' << cur.cur_suff << ' ' << winning << '\n';
			return mem[cur] = winning;
		})(state_t{0, 0, 0});
		cout << (ans ? "Takahashi" : "Aoki") << '\n';
	}

	return 0;
}
