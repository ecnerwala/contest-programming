#include <bits/extc++.h>

struct splitmix64_hash {
	static uint64_t splitmix64(uint64_t x) {
		// http://xorshift.di.unimi.it/splitmix64.c
		x += 0x9e3779b97f4a7c15;
		x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
		x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
		return x ^ (x >> 31);
	}

	size_t operator()(uint64_t x) const {
		static const uint64_t FIXED_RANDOM = std::chrono::steady_clock::now().time_since_epoch().count();
		return splitmix64(x + FIXED_RANDOM);
	}
};

template <typename K, typename V, typename Hash = splitmix64_hash>
using hash_map = __gnu_pbds::gp_hash_table<K, V, Hash>;

template <typename K, typename Hash = splitmix64_hash>
using hash_set = hash_map<K, __gnu_pbds::null_type, Hash>;

/** Binary-indexed tree
 *
 *  A binary indexed tree with N nodes of type T provides the
 *  following two functions for 0 <= i <= N:
 *
 *      prefix(int i) -> prefix_iterator<T>
 *      suffix(int i) -> suffix_iterator<T>
 *
 *  such that size(suffix(i) intersect prefix(j)) = (1 if i < j else 0).
 *  Furthermore, the resulting lists always have size at most log_2(N).
 *
 *  This can be used to implement either point-update/(prefix|suffix)-query or
 *  (prefix|suffix)-update/point-query over a virtual array of size N of a
 *  commutative monoid. This can be generalized to implement
 *  point-update/range-query or range-update/point-query over a virtual array
 *  of size N of a commutative group.
 *
 *  With 0-indexed data, prefixes are more natural:
 *   * For range update/query, use for_prefix for the ranges and for_suffix for the points.
 *   * For prefix update/query, no change.
 *   * For suffix update/query, use for_prefix(point + 1); 1-index the data.
 */
template <typename T> class binary_indexed_tree {
private:
	std::vector<T> dat;
public:
	binary_indexed_tree() {}
	explicit binary_indexed_tree(size_t N) : dat(N) {}
	binary_indexed_tree(size_t N, const T& t) : dat(N, t) {}

	size_t size() const { return dat.size(); }
	const std::vector<T>& data() const { return dat; }
	std::vector<T>& data() { return dat; }

private:
	template <typename I, typename S = I> struct iterator_range {
	private:
		I begin_;
		S end_;
	public:
		iterator_range() : begin_(), end_() {}
		iterator_range(const I& begin__, const S& end__) : begin_(begin__), end_(end__) {}
		iterator_range(I&& begin__, S&& end__) : begin_(begin__), end_(end__) {}
		I begin() const { return begin_; }
		S end() const { return end_; }
	};

public:
	class const_suffix_iterator {
	private:
		const T* dat;
		int a;
		const_suffix_iterator(const T* dat_, int a_) : dat(dat_), a(a_) {}
		friend class binary_indexed_tree;
	public:
		friend bool operator != (const const_suffix_iterator& i, const const_suffix_iterator& j) {
			assert(j.dat == nullptr);
			return i.a < j.a;
		}
		const_suffix_iterator& operator ++ () {
			a |= a+1;
			return *this;
		}
		const T& operator * () const {
			return dat[a];
		}
	};
	using const_suffix_range = iterator_range<const_suffix_iterator>;
	const_suffix_range suffix(int a) const {
		assert(0 <= a && a <= int(dat.size()));
		return const_suffix_range{const_suffix_iterator{dat.data(), a}, const_suffix_iterator{nullptr, int(dat.size())}};
	}

	class suffix_iterator {
	private:
		T* dat;
		int a;
		suffix_iterator(T* dat_, int a_) : dat(dat_), a(a_) {}
		friend class binary_indexed_tree;
	public:
		friend bool operator != (const suffix_iterator& i, const suffix_iterator& j) {
			assert(j.dat == nullptr);
			return i.a < j.a;
		}
		suffix_iterator& operator ++ () {
			a |= a+1;
			return *this;
		}
		T& operator * () const {
			return dat[a];
		}
	};
	using suffix_range = iterator_range<suffix_iterator>;
	suffix_range suffix(int a) {
		assert(0 <= a && a <= int(dat.size()));
		return suffix_range{suffix_iterator{dat.data(), a}, suffix_iterator{nullptr, int(dat.size())}};
	}

	class const_prefix_iterator {
	private:
		const T* dat;
		int a;
		const_prefix_iterator(const T* dat_, int a_) : dat(dat_), a(a_) {}
		friend class binary_indexed_tree;
	public:
		friend bool operator != (const const_prefix_iterator& i, const const_prefix_iterator& j) {
			assert(j.dat == nullptr);
			return i.a > 0;
		}
		const_prefix_iterator& operator ++ () {
			a &= a-1;
			return *this;
		}
		const T& operator * () const {
			return dat[a-1];
		}
	};
	using const_prefix_range = iterator_range<const_prefix_iterator>;
	const_prefix_range prefix(int a) const {
		return const_prefix_range{const_prefix_iterator{dat.data(), a}, const_prefix_iterator{nullptr, 0}};
	}

	class prefix_iterator {
	private:
		T* dat;
		int a;
		prefix_iterator(T* dat_, int a_) : dat(dat_), a(a_) {}
		friend class binary_indexed_tree;
	public:
		friend bool operator != (const prefix_iterator& i, const prefix_iterator& j) {
			assert(j.dat == nullptr);
			return i.a > 0;
		}
		prefix_iterator& operator ++ () {
			a &= a-1;
			return *this;
		}
		T& operator * () const {
			return dat[a-1];
		}
	};
	using prefix_range = iterator_range<prefix_iterator>;
	prefix_range prefix(int a) {
		return prefix_range{prefix_iterator{dat.data(), a}, prefix_iterator{nullptr, 0}};
	}
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int N, M, Q; cin >> N >> M >> Q;
	vector<int> A(N);
	struct dat_t {
		int cnt = 0;
		int64_t sum = 0;
	};
	binary_indexed_tree<hash_map<int, dat_t>> bits(N);
	for (int i = 0; i < N; i++) {
		cin >> A[i];
		for (auto& mp : bits.suffix(i)) {
			for (int a = M - A[i]; ; a += a & -a) {
				auto& it = mp[a];
				it.cnt++;
				it.sum += M - 1 - A[i];
				if (a == (1 << 30)) break;
			}
		}
	}
	struct op_t {
		int op;
		int a, b, c;
	};
	vector<op_t> ops(Q);
	for (int q = 0; q < Q; q++) {
		int op; cin >> op;
		if (op == 1) {
			int l, r, k; cin >> l >> r >> k; l--;
			ops[q] = {op, l, r, k}:
		} else if (op == 1) {
			int i, v; cin >> i >> v; i--;
			ops[q] = {op, i, v, 0};
		} else assert(false);
	}

	auto solve_slow = [&](int l, int r, int k) {
		vector<int> locs; locs.reserve(r-l);
		for (int i = l; i < r; i++) {
			locs.push_back(M - A[i]);
		}
		assert(int(locs.size()) == r-l);
		assert(k < int(locs.size()));
		int ans = 0;
		while (true) {
			nth_element(locs.begin(), locs.begin()+k, locs.end());
			for (int i = k; i < int(locs.size()); i++) {
				locs[i]--;
				if (locs[i] == 0) return ans;
			}
			ans++;
		}
	};

	while (Q--) {
		int op; cin >> op;
		if (op == 1) {
			// find the kth guy
			int l, r, k; cin >> l >> r >> k; l--;
			assert(1 <= k && k < (r-l));
			{
				// check if 1 << 30 is okay
				int tot_cnt = 0;
				int64_t tot_sum = 0;
				for (auto& mp : bits.prefix(l)) {
					auto it = mp.find(1 << 30);
					if (it != mp.end()) {
						tot_cnt -= it->second.cnt;
						tot_sum -= it->second.sum;
					}
				}
				for (auto& mp : bits.prefix(r)) {
					auto it = mp.find(1 << 30);
					if (it != mp.end()) {
						tot_cnt += it->second.cnt;
						tot_sum += it->second.sum;
					}
				}
				assert(tot_cnt == r-l);
				if ((1 << 30) * int64_t(tot_cnt - k) <= tot_sum) {
					cout << tot_sum / (tot_cnt - k) << '\n';
					continue;
				}
			}
			int cur = 0;
			int cur_cnt = 0;
			int64_t cur_sum = 0;
			for (int i = 29; i >= 0; i--) {
				int cnd = cur + (1 << i);
				int cnd_cnt = cur_cnt;
				int64_t cnd_sum = cur_sum;

				for (auto& mp : bits.prefix(l)) {
					auto it = mp.find(cnd);
					if (it != mp.end()) {
						cnd_cnt -= it->second.cnt;
						cnd_sum -= it->second.sum;
					}
				}
				for (auto& mp : bits.prefix(r)) {
					auto it = mp.find(cnd);
					if (it != mp.end()) {
						cnd_cnt += it->second.cnt;
						cnd_sum += it->second.sum;
					}
				}
				if (cnd_cnt <= k || cnd * int64_t(cnd_cnt - k) <= cnd_sum) {
					cur = cnd;
					cur_cnt = cnd_cnt;
					cur_sum = cnd_sum;
				}
			}
			//int o = solve_slow(l, r, k);
			//assert(o == cur);
			cout << cur << '\n';
		} else if (op == 2) {
			int i, v; cin >> i >> v; i--;
			for (auto& mp : bits.suffix(i)) {
				for (int a = M - A[i]; ; a += a & -a) {
					auto& it = mp[a];
					it.cnt--;
					it.sum -= M - 1 - A[i];
					if (a == (1 << 30)) break;
				}
				for (int a = M - v; ; a += a & -a) {
					auto& it = mp[a];
					it.cnt++;
					it.sum += M - 1 - v;
					if (a == (1 << 30)) break;
				}
			}
			A[i] = v;
		} else assert(false);
	}

	return 0;
}
