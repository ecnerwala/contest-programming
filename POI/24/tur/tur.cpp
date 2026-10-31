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

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	tensor<bool, 2> adj({N, N});
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < i; j++) {
			bool d; cin >> d;
			adj[{j, i}] = d;
			adj[{i, j}] = !d;
		}
	}

	vector<pair<int, int>> verts(N);
	for (int i = 0; i < N; i++) {
		int outDeg = 0;
		for (int j = 0; j < N; j++) {
			outDeg += adj[{i,j}];
		}
		verts[i] = {N - 1 - outDeg, i};
	}
	sort(verts.begin(), verts.end());

	vector<int> backEdgeCnt(N);
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < i; j++) {
			if (adj[{verts[i].second, verts[j].second}]) {
				backEdgeCnt[j]++;
				backEdgeCnt[i]--;
			}
		}
	}
	for (int i = 1; i < N; i++) backEdgeCnt[i] += backEdgeCnt[i-1];

	vector<vector<int>> ans(N);
	for (int i = 0; i < N; i++) ans[i].reserve(N);

	for (int st = 0, en; st < N; st = en) {
		en = st;
		while (backEdgeCnt[en] != 0) en++;
		en++;

		// Now find a Ham cycle for the vertices from st to en
		vector<int> cycle;
		for (int z = st; z < en; z++) {
			int cur = verts[z].second;
			if (cur == -1) continue;

			if (cycle.empty()) {
				cycle.push_back(cur);
				continue;
			}

			vector<int> cur_path({cur});
			while (true) {
				// Insert cur
				for (int i = 0; i < int(cycle.size()); i++) {
					if (adj[{cycle[i], cur_path.front()}] && adj[{cur_path.back(), cycle[(i+1)%int(cycle.size())]}]) {
						// insert it
						cycle.insert(cycle.begin()+i+1, cur_path.begin(), cur_path.end());
						goto done;
					}
				}

				// Otherwise, this vertex points all to or all away from the cycle
				// Keep extending the path until it meets it fully
				bool dir = adj[{cycle[0], cur}];
				for (int a = z+1; true; a++) {
					assert(a < en);
					int nxt = verts[a].second;
					if (nxt == -1) continue;
					if (dir ? adj[{cur_path.back(), nxt}] : adj[{nxt, cur_path.front()}]) {
						verts[a].second = -1;
						if (dir) cur_path.push_back(nxt);
						else cur_path.insert(cur_path.begin(), nxt);
						break;
					}
				}
			}
done:
			;
		}

		assert(int(cycle.size()) == en-st);

		for (auto& v : ans) {
			if (!v.empty()) v.insert(v.end(), cycle.begin(), cycle.end());
		}

		for (int i = 0; i < int(cycle.size()); i++) {
			auto& v = ans[cycle[i]];
			v.insert(v.end(), cycle.begin()+i, cycle.end());
			v.insert(v.end(), cycle.begin(), cycle.begin()+i);
		}
	}

	for (int i = 0; i < N; i++) {
		cout << ans[i].size();
		for (int v : ans[i]) cout << ' ' << v+1;
		cout << '\n';
	}

	return 0;
}
