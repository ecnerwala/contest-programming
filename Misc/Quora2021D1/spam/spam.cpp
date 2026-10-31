#include <bits/stdc++.h>

template <typename T> T sq(T a) { return a * a; }

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

#ifdef ECNERWALA
const int TESTING = true;
#else
const int TESTING = false;
#endif

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int N_TRAIN, N_TEST; cin >> N_TRAIN >> N_TEST;
	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	auto read_inp = [&]() -> vector<pair<int, int>> {
		string S; getline(cin, S); S += ' ';
		vector<pair<int, int>> res;
		int v = 0;
		for (char c : S) {
			if (c == ' ') {
				res.back().second = v;
				v = 0;
			} else if (c == ':') {
				res.push_back({v, -1});
				v = 0;
			} else if ('0' <= c && c <= '9') {
				v = 10*v + (c - '0');
			} else assert(false);
		}
		return res;
	};

	vector<vector<pair<int, int>>> train_inputs(N_TRAIN);
	vector<int> train_outputs(N_TRAIN);
	vector<vector<pair<int, int>>> test_inputs(N_TEST);

	for (int i = 0; i < N_TRAIN; i++) train_inputs[i] = read_inp();
	for (int i = 0; i < N_TRAIN; i++) cin >> train_outputs[i];
	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	for (int i = 0; i < N_TEST; i++) test_inputs[i] = read_inp();


	const int N_TYPES = 13;

	array<int, 2> num_type{0,0};

	tensor<double, 2> cnt0({2, N_TYPES}, 1);
	tensor<double, 3> cnt({2, N_TYPES, N_TYPES}, 1);
	tensor<double, 2> times({2, N_TYPES});
	tensor<double, 2> times_var({2, N_TYPES});
	tensor<int, 2> tot_cnt({2, N_TYPES});
	for (int x = 0; x < N_TRAIN; x++) {
		bool z = train_outputs[x];
		const auto& inp = train_inputs[x];

		num_type[z]++;

		cnt0[{z, inp[0].first}]++;

		for (int i = 0; i+1 < int(inp.size()); i++) {
			int t = inp[i].first, u = inp[i+1].first;
			int d = inp[i+1].second - inp[i].second;
			tot_cnt[{z, t}]++;
			cnt[{z, t, u}]++;
			times[{z, t}] += d;
			times_var[{z, t}] += d * d;
		}
	}

	for (int z = 0; z < 2; z++) {
		for (int t = 0; t < N_TYPES; t++) {
			cnt0[{z, t}] /= tot_cnt[{z, t}] + N_TYPES;
			for (int u = 0; u < N_TYPES; u++) {
				cnt[{z, t, u}] /= tot_cnt[{z, t}] + N_TYPES;
			}
			if (tot_cnt[{z, t}] > 0) {
				times[{z, t}] /= tot_cnt[{z, t}];
				times_var[{z, t}] /= tot_cnt[{z, t}];
				times_var[{z, t}] -= times[{z, t}] * times[{z, t}];
			}
		}
	}

	auto predict = [&](const auto& inp) -> bool {
		array<double, 2> weight{double(num_type[0]), double(num_type[1])};
		for (int z = 0; z < 2; z++) {
			weight[z] *= cnt0[{z, inp[0].first}];
		}
		for (int i = 0; i+1 < int(inp.size()); i++) {
			int t = inp[i].first, u = inp[i+1].first;
			int d = inp[i+1].second - inp[i].second;
			for (int z = 0; z < 2; z++) {
				weight[z] *= cnt[{z, t, u}];
				weight[z] *= exp(-0.5 * sq(d - times[{z, t}]) / times_var[{z, t}]);
			}
		}
		return weight[1] > weight[0];
	};

	if (TESTING) {
		int results[2][2] = {{0,0},{0,0}};
		for (int x = 0; x < N_TRAIN; x++) {
			int pred = predict(train_inputs[x]);
			int real = train_outputs[x];
			results[real][pred]++;
		}
		double precision = results[1][1] / double(results[0][1] + results[1][1]);
		double recall = results[1][1] / double(results[1][0] + results[1][1]);
		double my_score = 2 / (1 / precision + 1 / recall);
		double base_precision = num_type[1] / double(num_type[0] + num_type[1]);
		double base_recall = 1;
		double base_score = 2 / (1 / base_precision + 1 / base_recall);
		cerr << my_score << ' ' << base_score << '\n';
		cerr << (my_score / base_score - 1) / 0.7 << '\n';
	}

	if (!TESTING) {
		for (int x = 0; x < N_TEST; x++) {
			cout << predict(test_inputs[x]) << '\n';
		}
	}

	return 0;
}
