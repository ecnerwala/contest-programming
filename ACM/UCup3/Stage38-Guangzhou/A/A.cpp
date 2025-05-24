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

template <typename F> struct reverse_comparator_t {
	F f;
	template <typename Arg1, typename Arg2> constexpr bool operator() (Arg1&& arg1, Arg2&& arg2) & {
		return f(std::forward<Arg2>(arg2), std::forward<Arg1>(arg1));
	}
	template <typename Arg1, typename Arg2> constexpr bool operator() (Arg1&& arg1, Arg2&& arg2) const& {
		return f(std::forward<Arg2>(arg2), std::forward<Arg1>(arg1));
	}
	template <typename Arg1, typename Arg2> constexpr bool operator() (Arg1&& arg1, Arg2&& arg2) && {
		return std::move(f)(std::forward<Arg2>(arg2), std::forward<Arg1>(arg1));
	}
	template <typename Arg1, typename Arg2> constexpr bool operator() (Arg1&& arg1, Arg2&& arg2) const&& {
		return std::move(f)(std::forward<Arg2>(arg2), std::forward<Arg1>(arg1));
	}
};

template <typename F> constexpr reverse_comparator_t<std::decay_t<F>> reverse_comparator(F&& f) {
	return { std::forward<F>(f) };
}

class CartesianTree {
public:
	struct Node {
		int l, m, r; // inclusive ranges
		std::array<int, 2> c;
	};
	std::vector<Node> nodes;
	int root = -1;

	CartesianTree() {}

	Node& operator [] (int idx) { return nodes[idx]; }
	const Node& operator [] (int idx) const { return nodes[idx]; }

	int size() const { return int(nodes.size()); }

private:
	CartesianTree(std::vector<Node>&& nodes_, int root_) : nodes(std::move(nodes_)), root(root_) {}

public:

	// min-cartesian-tree, with earlier cells tiebroken earlier
	template <typename T, typename Comp = std::less<T>>
	static CartesianTree build_min_tree(const std::vector<T>& v, Comp comp = Comp()) {
		std::vector<Node> nodes(v.size()*2+1);
		std::vector<int> stk; stk.reserve(v.size());
		int root = -1;
		for (int i = 0; i <= int(v.size()); i++) {
			int cur = 2*i;
			nodes[cur].l = i;
			nodes[cur].r = i-1;
			nodes[cur].m = i-1;
			nodes[cur].c = {-1, -1};
			while (!stk.empty() && (i == int(v.size()) || comp(v[i], v[nodes[stk.back()].m]))) {
				int nxt = stk.back(); stk.pop_back();
				nodes[nxt].c[1] = cur;
				nodes[nxt].r = nodes[cur].r;
				cur = nxt;
			}
			if (i == int(v.size())) {
				root = cur;
				break;
			}
			nodes[2*i+1].l = nodes[cur].l;
			nodes[2*i+1].m = i;
			nodes[2*i+1].c[0] = cur;
			stk.push_back(2*i+1);
		}
		return {std::move(nodes), root};
	}

	// max-cartesian-tree, with earlier cells tiebroken earlier
	template <typename T, typename Comp = std::less<T>>
	static CartesianTree build_max_tree(const std::vector<T>& v, Comp comp = Comp()) {
		return build_min_tree(v, reverse_comparator(comp));
	}
};

template<class T> int sz(T&& arg) { using std::size; return int(size(std::forward<T>(arg))); }

class SuffixArray {
public:
	using index_t = int;
	int N;
	std::vector<index_t> sa;
	std::vector<index_t> rank;
	// lcp[i] = get_lcp(sa[i], sa[i+1])
	std::vector<index_t> lcp;
	//RangeMinQuery<std::pair<index_t, index_t>> rmq;

	SuffixArray() {}

	template <typename String> static SuffixArray construct(const String& S) {
		int N = sz(S);
		SuffixArray sa(N);

		sa.build_sa(S);
		sa.build_rank();
		sa.build_lcp(S);
		//sa.build_rmq();

		return sa;
	}

	template <typename String, typename F> static SuffixArray map_and_construct(const String& S, const F& f) {
		std::vector<decltype((f(S[0])))> mapped(sz(S));
		for (int i = 0; i < sz(S); i++) {
			mapped[i] = f(S[i]);
		}
		return construct(mapped);
	}

	template <typename String> static SuffixArray compress_and_construct(const String& S) {
		using std::begin;
		using std::end;
		using value_type = typename std::iterator_traits<decltype(begin(S))>::value_type;
		std::vector<value_type> vals(begin(S), end(S));
		std::sort(vals.begin(), vals.end());
		vals.resize(unique(vals.begin(), vals.end()) - vals.begin());
		using compressed_value_type = typename std::conditional<
			sizeof(value_type) < sizeof(index_t),
			value_type,
			index_t
		>::type;
		std::vector<compressed_value_type> compressed_s(sz(S));
		for (int i = 0; i < sz(S); i++) {
			compressed_s[i] = compressed_value_type(index_t(lower_bound(vals.begin(), vals.end(), S[i]) - vals.begin()));
		}
		return construct(compressed_s);
	}

	static SuffixArray construct_lower_alpha(const std::string& s) {
		return SuffixArray::map_and_construct(s, [](char c) -> char { return char(c - 'a'); });
	}

	static SuffixArray construct_upper_alpha(const std::string& s) {
		return SuffixArray::map_and_construct(s, [](char c) -> char { return char(c - 'A'); });
	}

private:
	explicit SuffixArray(int N_) : N(N_) {}

	template <typename String> void build_sa(const String& S) {
		sa = std::vector<index_t>(N+1);
		for (auto s : S) assert(index_t(s) >= 0);
		int sigma = N ? *max_element(S.begin(), S.end())+1 : 0;
		std::vector<index_t> tmp(std::max(N, sigma * 2));
		SuffixArray::sais<String>(N, S, sa.data(), sigma, tmp.data());
	}

	template <typename String> static void sais(int N, const String& S, index_t* sa, int sigma, index_t* tmp) {
		if (N == 0) {
			sa[0] = 0;
			return;
		} else if (N == 1) {
			sa[0] = 1;
			sa[1] = 0;
			return;
		}

		// Phase 1: Initialize the frequency array, which will let us lookup buckets.
		index_t* freq = tmp; tmp += sigma;
		memset(freq, 0, sizeof(*freq) * sigma);
		for (int i = 0; i < N; i++) {
			++freq[index_t(S[i])];
		}
		auto build_bucket_start = [&]() {
			int cur = 1;
			for (int v = 0; v < sigma; v++) {
				tmp[v] = cur;
				cur += freq[v];
			}
		};
		auto build_bucket_end = [&]() {
			int cur = 1;
			for (int v = 0; v < sigma; v++) {
				cur += freq[v];
				tmp[v] = cur;
			}
		};

		int num_pieces = 0;

		int first_endpoint = 0;
		// Phase 2: find the right-endpoints of the pieces
		{
			build_bucket_end();

			// Initialize the final endpoint out-of-band this way so that we don't try to look up tmp[-1].
			// This doesn't count towards num_pieces.
			sa[0] = N;

			index_t c0 = S[N-1], c1 = -1; bool isS = false;
			for (int i = N-2; i >= 0; i--) {
				c1 = c0;
				c0 = S[i];
				if (c0 < c1) {
					isS = true;
				} else if (c0 > c1 && isS) {
					isS = false;
					// insert i+1
					sa[first_endpoint = --tmp[c1]] = i+1;
					++num_pieces;
				}
			}
		}

		// If num_pieces <= 1, we don't need to actually run the recursion, it's just sorted automatically
		// Otherwise, we're going to rebucket
		if (num_pieces > 1) {
			// Remove the first endpoint, we don't need to run the IS on this
			sa[first_endpoint] = 0;

			// Run IS for L-type
			{
				build_bucket_start();
				for (int z = 0; z <= N; z++) {
					int v = sa[z];
					if (!v) continue;

					// Leave for the S-round
					if (v < 0) continue;

					// clear out our garbage
					sa[z] = 0;

					--v;
					index_t c0 = S[v-1], c1 = S[v];
					sa[tmp[c1]++] = (c0 < c1) ? ~v : v;
				}
			}

			index_t* const sa_end = sa + N + 1;

			index_t* pieces = sa_end;
			// Run IS for S-type and compactify
			{
				build_bucket_end();
				for (int z = N; z >= 0; z--) {
					int v = sa[z];
					if (!v) continue;

					// clear our garbage
					sa[z] = 0;

					if (v > 0) {
						*--pieces = v;
						continue;
					}

					v = ~v;

					--v;
					index_t c0 = S[v-1], c1 = S[v];
					sa[--tmp[c1]] = (c0 > c1) ? v : ~v;
				}
			}

			// Compute the lengths of the pieces in preparation for equality
			// comparison, and store them in sa[v/2]. We set the length of the
			// final piece to 0; it compares unequal to everything because of
			// the sentinel.
			{
				int prv_start = N;
				index_t c0 = S[N-1], c1 = -1; bool isS = false;
				for (int i = N-2; i >= 0; i--) {
					c1 = c0;
					c0 = S[i];
					if (c0 < c1) {
						isS = true;
					} else if (c0 > c1 && isS) {
						isS = false;

						// insert i+1
						int v = i+1;
						sa[v>>1] = prv_start == N ? 0 : prv_start - v;
						prv_start = v;
					}
				}
			}

			// Compute the alphabet, storing the result into sa[v/2].
			int next_sigma = 0;
			{
				int prv_len = -1, prv_v = 0;
				for (int i = 0; i < num_pieces; i++) {
					int v = pieces[i];
					int len = sa[v>>1];

					bool eq = prv_len == len;
					for (int a = 0; eq && a < len; ++a) {
						eq = S[v+a] == S[prv_v+a];
					}
					if (!eq) {
						next_sigma++;
						prv_len = len;
						prv_v = v;
					}

					sa[v>>1] = next_sigma; // purposely leave this 1 large to check != 0
				}
			}

			if (next_sigma == num_pieces) {
				sa[0] = N;
				memcpy(sa+1, pieces, sizeof(*sa) * num_pieces);
			} else {
				index_t* next_S = sa_end;

				// Finally, pack the input to the SA
				{
					for (int i = (N-1)>>1; i >= 0; i--) {
						int v = sa[i];
						if (v) *--next_S = v-1;
						sa[i] = 0;
					}
				}

				memset(sa, 0, sizeof(*sa) * (num_pieces+1));
				sais<const index_t*>(num_pieces, next_S, sa, next_sigma, tmp);

				{ // Compute the piece start points again and use those to map up the suffix array
					next_S = sa_end;
					index_t c0 = S[N-1], c1 = -1; bool isS = false;
					for (int i = N-2; i >= 0; i--) {
						c1 = c0;
						c0 = S[i];
						if (c0 < c1) {
							isS = true;
						} else if (c0 > c1 && isS) {
							isS = false;

							int v = i+1;
							*--next_S = v;
						}
					}
					sa[0] = N;
					for (int i = 1; i <= num_pieces; i++) {
						sa[i] = next_S[sa[i]];
					}
				}
			}

			// zero everything else
			memset(sa+num_pieces+1, 0, sizeof(*sa) * (N - num_pieces));

			{
				// Scatter the finished pieces
				build_bucket_end();
				for (int i = num_pieces; i > 0; i--) {
					int v = sa[i];
					sa[i] = 0;

					index_t c1 = S[v];
					sa[--tmp[c1]] = v;
				}
			}
		}

		// Home stretch! Just finish out with the L-type and then S-type
		{
			build_bucket_start();
			for (int z = 0; z <= N; z++) {
				int v = sa[z];
				if (v <= 0) continue;
				--v;
				index_t c1 = S[v];
				index_t c0 = v ? S[v-1] : c1; // if v = 0, we don't want to invert
				sa[tmp[c1]++] = (c0 < c1) ? ~v : v;
			}
		}

		// This just aggressively overwrites our original scattered pieces with the correct values
		{
			build_bucket_end();
			for (int z = N; z >= 0; z--) {
				int v = sa[z];
				if (v >= 0) continue;
				sa[z] = v = ~v;
				--v;
				index_t c1 = S[v];
				index_t c0 = v ? S[v-1] : c1+1;
				sa[--tmp[c1]] = (c0 > c1) ? v : ~v;
			}
		}
	}

	void build_rank() {
		rank = std::vector<index_t>(N+1);
		for (int i = 0; i <= N; i++) rank[sa[i]] = i;
	}

	template <typename String> void build_lcp(const String& S) {
		assert(sz(S) == N);
		lcp = std::vector<index_t>(N);
		for (int i = 0, k = 0; i < N - 1; i++) {
			int j = sa[rank[i]-1];
			while (k < N - std::max(i, j) && S[i+k] == S[j+k]) k++;
			lcp[rank[i]-1] = k;
			if (k) --k;
		}
	}

	void build_rmq() {
		std::vector<std::pair<index_t, index_t>> lcp_idx(N);
		for (int i = 0; i < N; i++) {
			lcp_idx[i] = {lcp[i], i+1};
		}
		//rmq = RangeMinQuery<std::pair<index_t, index_t>>(std::move(lcp_idx));
	}
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		std::string S; cin >> S;
		cout << [&]() -> int64_t {
			int N = int(S.size());
			auto sa = SuffixArray::construct_lower_alpha(S);
			auto ct = CartesianTree::build_min_tree(sa.lcp);
			int64_t ans = 0;
			struct dat_t {
				std::map<int, int> alive;
				std::set<std::pair<int, int>> gaps_by_time;
				std::set<std::pair<int, int>> gaps_by_index;
			};
			auto ins_gaps = [&](dat_t& d, auto it) -> void {
				auto [a, g] = *it;
				if (g == -1) {
					return;
				}
				d.gaps_by_index.insert({g, a});
				d.gaps_by_time.insert({g-a, a});
			};
			auto rem_gaps = [&](dat_t& d, auto it) -> void {
				auto [a, g] = *it;
				if (g == -1) {
					return;
				}
				d.gaps_by_index.erase({g, a});
				d.gaps_by_time.erase({g-a, a});
			};
			auto build_gaps = [&](dat_t& d) -> void {
				d.gaps_by_index = {};
				d.gaps_by_time = {};
				for (auto it = d.alive.begin(); it != d.alive.end(); ++it) {
					ins_gaps(d, it);
				}
			};
			dat_t d_init;
			for (int i = 0; i < N; i++) {
				d_init.alive.insert({i, i+1});
			}
			d_init.alive.insert({N, -1});
			build_gaps(d_init);

			std::y_combinator([&](auto self, int depth, int cur, dat_t d) -> void {
				{
					int ndepth = 0;
					if (cur & 1) {
						ndepth = sa.lcp[cur/2];
					} else {
						ndepth = N - sa.sa[cur/2];
					}
					assert(ndepth >= depth);
					//cerr << ct.nodes[cur].l << '-' << ct.nodes[cur].r+1 << ' ' << depth << '-' << ndepth << '\n';
					while (!d.gaps_by_time.empty() && ndepth >= d.gaps_by_time.begin()->first) {
						int gt = d.gaps_by_time.begin()->first;
						assert(gt >= depth);
						depth = gt;
						int a = d.gaps_by_time.begin()->second;
						auto it = d.alive.find(a);
						assert(it != d.alive.end());
						assert(it->second - a == gt);
						rem_gaps(d, it);
						it->second = -1;
						// noop
						ins_gaps(d, it);
					}
					if (d.gaps_by_time.empty()) {
						//cerr << "good at time " << depth << '\n';
						for (int z = ct.nodes[cur].l; z <= ct.nodes[cur].r+1; z++) {
							int i = sa.sa[z];
							ans += (N - i) - depth + 1;
						}
						return;
					}
					depth = ndepth;
				}

				if (cur & 1) {
					bool smaller_right = ct.nodes[cur].m >= ct.nodes[cur].l + ct.nodes[cur].r;
					int sm = ct.nodes[cur].c[smaller_right];
					int lg = ct.nodes[cur].c[!smaller_right];
					//cerr << "smaller size " << ct.nodes[sm].r - ct.nodes[sm].l + 2 << '\n';
					//cerr << "larger size " << ct.nodes[lg].r - ct.nodes[lg].l + 2 << '\n';

					//cerr << "smaller range " << ct.nodes[sm].l << ' ' << ct.nodes[sm].r + 1 << '\n';
					std::vector<int> buf; buf.reserve(ct.nodes[sm].r - ct.nodes[sm].l + 2);
					buf.insert(buf.end(), sa.sa.begin() + ct.nodes[sm].l, sa.sa.begin() + (ct.nodes[sm].r + 2));
					assert(!buf.empty());
					std::sort(buf.begin(), buf.end());

					[&]() -> bool {
						// First build the smaller side
						dat_t d2;

						auto get_before = [&](int a) -> int {
							auto it = d.gaps_by_index.upper_bound({a, N+1});
							if (it == d.gaps_by_index.begin()) {
								return -1;
							}
							return std::prev(it)->first;
						};
						if (get_before(buf[0]) != -1) {
							return false;
						}
						for (int z = 0; z < int(buf.size()); z++) {
							int g = get_before(z+1 == int(buf.size()) ? N : buf[z+1]);
							d2.alive.insert({buf[z], g});
						}
						build_gaps(d2);
						self(depth, sm, std::move(d2));
						return true;
					}();

					[&]() -> bool {
						for (auto a : buf) {
							auto it = d.alive.find(a);
							assert(it != d.alive.end());
							if (it->second != -1) {
								rem_gaps(d, it);
								if (it != d.alive.begin()) {
									auto pit = std::prev(it);
									rem_gaps(d, pit);
									pit->second = it->second;
									ins_gaps(d, pit);
								} else {
									return false;
								}
							}
							d.alive.erase(it);
						}
						self(depth, lg, std::move(d));
						return true;
					}();
				} else {
					// we're always dead
				}
			})(0, ct.root, d_init);
			return ans;
		}() << '\n';
	}

	return 0;
}
