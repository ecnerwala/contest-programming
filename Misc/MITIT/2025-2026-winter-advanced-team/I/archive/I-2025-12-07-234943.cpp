#include <bits/stdc++.h>
using namespace std;

template <typename T> void set_min(T& a, const T& b){
	if(b < a) a = b;
}
template <typename T> void set_max(T& a, const T& b){
	if(b > a) a = b;
}

#ifdef LOCAL
#define TEST 1
#else
#define TEST 0
#endif

template <typename T, class Compare = std::less<T>> class RangeMinQuery : private Compare {
	static const int BUCKET_SIZE = 32;
	static const int BUCKET_SIZE_LOG = 5;
	static_assert(BUCKET_SIZE == (1 << BUCKET_SIZE_LOG), "BUCKET_SIZE should be a power of 2");
	static const int CACHE_LINE_ALIGNMENT = 64;
	int n = 0;
	std::vector<T> data;
	std::vector<T> pref_data;
	std::vector<T> suff_data;
	std::vector<T> sparse_table;
	std::vector<uint32_t> range_mask;

private:
	int num_buckets() const {
		return n >> BUCKET_SIZE_LOG;
	}
	int num_levels() const {
		return num_buckets() ? 32 - __builtin_clz(num_buckets()) : 0;
	}
	int sparse_table_size() const {
		return num_buckets() * num_levels();
	}
private:
	const T& min(const T& a, const T& b) const {
		return Compare::operator()(a, b) ? a : b;
	}
	void setmin(T& a, const T& b) const {
		if (Compare::operator()(b, a)) a = b;
	}

	template <typename Vec> static int get_size(const Vec& v) { using std::size; return int(size(v)); }

public:
	RangeMinQuery() {}
	template <typename Vec> explicit RangeMinQuery(const Vec& data_, const Compare& comp_ = Compare())
		: Compare(comp_)
		  , n(get_size(data_))
		  , data(n)
		  , pref_data(n)
		  , suff_data(n)
		  , sparse_table(sparse_table_size())
		  , range_mask(n)
	{
		for (int i = 0; i < n; i++) data[i] = data_[i];
		for (int i = 0; i < n; i++) {
			if (i & (BUCKET_SIZE-1)) {
				uint32_t m = range_mask[i-1];
				while (m && !Compare::operator()(data[(i | (BUCKET_SIZE-1)) - __builtin_clz(m)], data[i])) {
					m -= uint32_t(1) << (BUCKET_SIZE - 1 - __builtin_clz(m));
				}
				m |= uint32_t(1) << (i & (BUCKET_SIZE - 1));
				range_mask[i] = m;
			} else {
				range_mask[i] = 1;
			}
		}
		for (int i = 0; i < n; i++) {
			pref_data[i] = data[i];
			if (i & (BUCKET_SIZE-1)) {
				setmin(pref_data[i], pref_data[i-1]);
			}
		}
		for (int i = n-1; i >= 0; i--) {
			suff_data[i] = data[i];
			if (i+1 < n && ((i+1) & (BUCKET_SIZE-1))) {
				setmin(suff_data[i], suff_data[i+1]);
			}
		}
		for (int i = 0; i < num_buckets(); i++) {
			sparse_table[i] = data[i * BUCKET_SIZE];
			for (int v = 1; v < BUCKET_SIZE; v++) {
				setmin(sparse_table[i], data[i * BUCKET_SIZE + v]);
			}
		}
		for (int l = 0; l+1 < num_levels(); l++) {
			for (int i = 0; i + (1 << (l+1)) <= num_buckets(); i++) {
				sparse_table[(l+1) * num_buckets() + i] = min(sparse_table[l * num_buckets() + i], sparse_table[l * num_buckets() + i + (1 << l)]);
			}
		}
	}

	T query(int l, int r) const {
		assert(l <= r);
		int bucket_l = (l >> BUCKET_SIZE_LOG);
		int bucket_r = (r >> BUCKET_SIZE_LOG);
		if (bucket_l == bucket_r) {
			uint32_t msk = range_mask[r] & ~((uint32_t(1) << (l & (BUCKET_SIZE-1))) - 1);
			int ind = (l & ~(BUCKET_SIZE-1)) + __builtin_ctz(msk);
			return data[ind];
		} else {
			T ans = min(suff_data[l], pref_data[r]);
			bucket_l++;
			if (bucket_l < bucket_r) {
				int level = (32 - __builtin_clz(bucket_r - bucket_l)) - 1;
				setmin(ans, sparse_table[level * num_buckets() + bucket_l]);
				setmin(ans, sparse_table[level * num_buckets() + bucket_r - (1 << level)]);
			}
			return ans;
		}
	}
};

template <typename T> using RangeMaxQuery = RangeMinQuery<T, std::greater<T>>;

vector<int> gen(int N, int d) {
	mt19937_64 rng(59);
	vector<int> values(N);
	for (int i = 0; i < N; ++i) values[i] = 1 + rng() % (1'000'000'000 - N);
	sort(values.begin(), values.end());
	for (int i = 0; i < N; ++i) values[i] += i;
	vector<int> perm(N, -1);
	{
		int x = 0;
		for (int i = 0, j = N - 1; j - i >= d; ) {
			if (rng() & 1) {
				perm[i + d] = x++;
				i += (d + 1);
			} else {
				perm[j - d] = x++;
				j -= (d + 1);
			}
		}
		vector<int> xs;
		for (; x < N; ++x) xs.push_back(x);
		for (int i = 0; i < (int)xs.size(); ++i) swap(xs[rng() % (i + 1)], xs[i]);
		for (int i = 0, j = 0; i < N; ++i) if (!~perm[i]) perm[i] = xs[j++];
	}
	vector<int> A(N);
	for (int i = 0; i < N; ++i) A[i] = values[perm[i]];
	return A;
}


using ll = int64_t;
using ld = long double;

int main(){
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int N;
	if(!TEST){
		cin >> N;
	} else {
		N = int(1e5);
	}

	int D = 1;

	vector<int> ANS(N);
	for(int i = 0; i < N; i++) ANS[i] = i;
	if(TEST){
		mt19937 mt_test(48);
		// shuffle(ANS.begin(), ANS.end(), mt_test);

		ANS = gen(N, D);
	}

	RangeMinQuery<int> rmq(ANS);

	ld STOP_COST = 1 - 1e-9;
	bool STOP = false;

	ld USED_COST = 0;

	map<pair<int,int>, int> known;

	auto query = [&](int l, int r){
		// inclusive

		if(!known.count({l, r})){
			if(USED_COST + 1 / ld(r-l+1) > STOP_COST){
				STOP = true;
				return -1;
			}
			USED_COST += 1 / ld(r-l+1);

			assert(0 <= l && r <= N-1);
			assert(l <= r);
			int a;
			if(!TEST){
				cout << "? " << (l+1) << ' ' << (r+1) << '\n';
				cout << flush;
				cin >> a;
			} else {
				a = rmq.query(l, r);
			}
			known[{l, r}] = a;
		}
		return known[{l, r}];
	};

	map<int, vector<int> > outputs;

	auto key_in = [&](int lmin, int lmax, int rmin, int rmax, int a) -> void {
		if(TEST){
			assert(rmq.query(lmax, rmin) == a);
			assert(rmq.query(lmin, rmax) == a);
		}
		if(!outputs.count(a)){
			outputs[a] = {lmin, lmax, rmin, rmax};
		}
		set_min(outputs[a][0], lmin);
		set_max(outputs[a][1], lmax);
		set_min(outputs[a][2], rmin);
		set_max(outputs[a][3], rmax);
	};

	mt19937 mt(48);

	auto solve = [&](int l, int r) -> int {
		int a = query(l, r);
		if(STOP) return -1;
		int rand_cycle = mt() % 2;
		for(int it = 0; it < 2; it++){
			if(it == rand_cycle){
				int d = query(l, r-1);
				if(STOP) return -1;
				if(d != a){
					key_in(l, r, r, r, a);
					return r;
				}
			} else {
				int d = query(l+1, r);
				if(STOP) return -1;
				if(d != a){
					key_in(l, l, l, r, a);
					return l;
				}
			}
		}
		int failed_d = 1;
		int d;
		for(d = 2; ; d++){
			if(l+d > r-d) break;
			int b = query(l+d, r-d);
			if(STOP) return -1;
			if(b != a) break;
			failed_d = d;
		}
		int la = l+failed_d;
		int rb = r-failed_d;
		int lb = min(l+d, rb);
		int ra = max(r-d, la);
		// la,lb   ra,rb
		if(lb > ra){
			int md = (l + r) / 2;
			lb = md;
			ra = md+1;
		}

		if(query(lb+1, r) == a){
			if(STOP) return -1;
			int s = ra-1;
			int e = rb;
			while(s+1 < e){
				int m = (s+e)/2;
				if(query(l, m) == a){
					e = m;
				} else {
					s = m;
				}
				if(STOP) return -1;
			}
			key_in(l, e, e, r, a);
			return e;
		} else {
			if(STOP) return -1;
			int s = la;
			int e = lb+1;
			while(s+1 < e){
				int m = (s+e)/2;
				if(query(m, r) == a){
					s = m;
				} else {
					e = m;
				}
				if(STOP) return -1;
			}
			key_in(l, s, s, r, a);
			return s;
		}
	};

	vector<vector<pair<int,int> >> intervals_by_len(N+1);
	intervals_by_len[N].push_back({0, N-1});

	auto add_interval = [&](int a, int b) {
		intervals_by_len[b-a+1].push_back({a, b});
	};
	for(int len = N; len >= 1; len--){
		for(auto [x, y] : intervals_by_len[len]){
			int m = solve(x, y);
			if(m == -1) break;
			add_interval(x, m-1);
			add_interval(m+1, y);
		}
	}

	for(auto [ival, a] : known){
		auto [l, r] = ival;
		key_in(l, l, r, r, a);
	}
	ll found_answers = 0;
	ll max_answers = N * ll(N+1) / 2;
	for(auto [a, info] : outputs){
		found_answers += (info[1] - info[0] + 1) * (info[3] - info[2] + 1);
	}
	ld ratio = ld(found_answers) / ld(max_answers);
	cerr << "D=" << D << ' ' << found_answers << "/" <<  max_answers << ' ' << "ratio " << ratio << ' ' << "USED_COST " << USED_COST << '\n';
	if(!TEST){
		cout << "! " << outputs.size() << '\n';
		for(auto [a, info] : outputs){
			for(int i = 0; i < 4; i++){
				cout << (info[i] + 1) << ' ';
			}
			cout << a << '\n';
		}
	}
}
