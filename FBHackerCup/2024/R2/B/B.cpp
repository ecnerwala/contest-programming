#include<bits/stdc++.h>

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

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		std::array<std::string, 6> rows;
		for (auto& s : rows) cin >> s;
		cerr << "Case #" << case_num << '\n';
		for (auto s : rows) cerr << s << '\n';
		std::reverse(rows.begin(), rows.end());

		tensor<bool, 7> rev_vis({7, 7, 7, 7, 7, 7, 7});
		{
			std::vector<std::array<int, 7>> q;
			std::array<int, 7> st; st.fill(6);
			rev_vis[st] = true;
			q.push_back(st);

			std::array<bool, 2> can_win{false, false};
			for (int move = 0, idx = 0; move < 42; move++) {
				char piece = "FC"[move & 1];
				int en = int(q.size());
				for (; idx < en; idx++) {
					for (int col = 0; col < 7; col++) {
						std::array<int, 7> nxt = q[idx];
						if (nxt[col] == 0) continue;
						nxt[col]--;
						if (rows[nxt[col]][col] != piece) continue;
						if (rev_vis[nxt]) continue;
						rev_vis[nxt] = true;
						q.push_back(nxt);
					}
				}
			}
		}

		tensor<bool, 7> vis({7, 7, 7, 7, 7, 7, 7});
		std::vector<std::array<int, 7>> q;
		std::array<int, 7> st; st.fill(0);
		vis[st] = true;
		q.push_back(st);

		std::array<bool, 2> can_win{false, false};
		for (int move = 0, idx = 0; move < 42; move++) {
			char piece = "CF"[move & 1];
			int en = int(q.size());
			for (; idx < en; idx++) {
				for (int col = 0; col < 7; col++) {
					std::array<int, 7> nxt = q[idx];
					if (nxt[col] == 6) continue;
					if (rows[nxt[col]][col] != piece) continue;
					nxt[col]++;
					auto good = [&](int dc, int dr) -> bool {
						int c = col + dc;
						int r = nxt[col] - 1 + dr;
						if (!(0 <= c && c < 7)) return false;
						if (!(0 <= r && r < nxt[c])) return false;
						return rows[r][c] == piece;
					};
					assert(good(0, 0));
					if (!rev_vis[nxt]) continue;
					if (vis[nxt]) continue;
					vis[nxt] = true;
					// now, check if this is a win
					bool win = false;
					int wdc = -1, wdr = -1, wa = -1, wb = -1;
					for (auto [dc, dr] : {std::array<int, 2>{-1, 0}, std::array<int, 2>{0, 1}, std::array<int, 2>{-1, 1}, std::array<int, 2>{1, 1}}) {
						assert(good(0, 0));
						int a = 0;
						while (good(dc * a, dr * a)) a++;
						int b = 0;
						while (good(-dc * b, -dr * b)) b++;
						if (a + b - 1 >= 4) {
							win = true;
							wdc = dc, wdr = dr;
							wa = a, wb = b;
							break;
						}
					}
					if (win) {
						if (!can_win[move & 1]) {
							cerr << "Win for " << piece << '\n';
							for (int v : nxt) cerr << v << ' ';
							cerr << '\n';
							cerr << ' ' << wdc << ' ' << wdr << ' ' << wa << ' ' << wb << '\n';
						}
						can_win[move & 1] = true;
					} else {
						q.push_back(nxt);
					}
				}
			}
		}

		char res;
		if (can_win[0] && can_win[1]) res = '?';
		else if (can_win[0]) res = 'C';
		else if (can_win[1]) res = 'F';
		else res = '0';
		cout << "Case #" << case_num << ": " << res << '\n';
	}

	return 0;
}
