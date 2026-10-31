#include <bits/stdc++.h>

template <typename T> void setmin(T& a, T b) { if (b < a) a = b; }
template <typename T> void setmax(T& a, T b) { if (b > a) a = b; }

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

struct state_t {
	bool is_full;
	int num_filled;
	int root_val;
	int stk_idx;
	int height;

	friend bool operator < (const state_t& a, const state_t& b) {
		return std::tie(a.is_full, a.num_filled, a.root_val, a.stk_idx, a.height) < std::tie(b.is_full, b.num_filled, b.root_val, b.stk_idx, b.height);
	}
};

const int64_t INF = int64_t(2e18);
struct sat_num {
	int64_t v;
	sat_num(int64_t v_ = 0) : v(v_) {}
	explicit operator int64_t () const { return v; }
	sat_num& operator += (sat_num o) {
		v += o.v;
		if (v > INF) v = INF;
		return *this;
	}
	sat_num& operator *= (sat_num o) {
		if (o.v > INF / v) v = INF;
		else v *= o.v;
		return *this;
	}
	friend sat_num operator + (sat_num a, sat_num b) { return sat_num(a) += b; }
	friend sat_num operator * (sat_num a, sat_num b) { return sat_num(a) *= b; }
};

bool is_balanced(const std::vector<int>& P) {
	int N = int(P.size());
	struct tree_node {
		int val;
		int min_height;
		int max_height;
	};
	std::vector<tree_node> stk; stk.reserve(N+2);
	stk.push_back({N+1, -1, -1});
	for (int z = 0; z <= N; z++) {
		int v = z < N ? P[z] : N;
		int cur_min_height = 0;
		int cur_max_height = 0;
		while (stk.back().val < v) {
			cur_min_height++;
			cur_max_height++;
			setmin(cur_min_height, stk.back().min_height);
			setmax(cur_max_height, stk.back().max_height);
			// illegal
			if (cur_max_height - cur_min_height > 1) return false;
			stk.pop_back();
		}
		cur_min_height++;
		cur_max_height++;
		stk.push_back({v, cur_min_height, cur_max_height});
	}
	assert(stk.size() == 2);
	return true;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::array<int64_t, 2> K; cin >> K[0] >> K[1];

	int L = 0;
	while (N >= (1 << L)) L++;
	assert(N >= (1 << (L-1)) && N <= (1 << L) - 1);
	// the root should be max_height == L
	// for a node, we define height - 1 <= min_height <= max_height <= height
	// root should have height L

	std::vector<std::vector<sat_num>> choose;
	choose.push_back({1});

	std::vector<sat_num> tree_ways;
	tree_ways.push_back(1);
	std::vector<std::array<int, 2>> tree_height;
	tree_height.push_back({0, 0});

	auto cnt_ways = [&](const std::vector<int>& pref) -> sat_num {
		std::vector<bool> used(N+1, false);

		struct tree_node {
			int val;
			int min_height;
			int max_height;
		};
		std::vector<tree_node> stk; stk.reserve(N+1);
		stk.push_back({N, L, L+1});
		used[N] = true;

		for (int v : pref) {
			if (used[v]) return 0;
			used[v] = true;
			int cur_min_height = 0;
			int cur_max_height = 0;
			while (stk.back().val < v) {
				cur_min_height++;
				cur_max_height++;
				setmin(cur_min_height, stk.back().min_height);
				setmax(cur_max_height, stk.back().max_height);
				// illegal
				if (cur_max_height - cur_min_height > 1) return 0;
				stk.pop_back();
			}
			cur_min_height++;
			cur_max_height++;
			stk.push_back({v, cur_min_height, cur_max_height});
		}

		//for (auto s : stk) { cerr << s.val << ' ' << s.min_height << '-' << s.max_height << '\n'; }

		std::vector<int> nxt_unused(N+2);
		nxt_unused[N+1] = N+1;
		for (int i = N; i >= 0; i--) {
			if (!used[i]) nxt_unused[i] = i;
			else nxt_unused[i] = nxt_unused[i+1];
		}

		std::vector<int> unused_geq(N+2);
		unused_geq[N+1] = 0;
		for (int i = N; i >= 0; i--) {
			unused_geq[i] = unused_geq[i+1] + !used[i];
		}

		int max_extra = N - int(pref.size());
		while (max_extra >= int(choose.size())) {
			int n = int(choose.size());
			choose.emplace_back(n+1);
			for (int i = 0; i <= n-1; i++) {
				choose[n][i] += choose[n-1][i];
				choose[n][i+1] += choose[n-1][i];
			}
		}
		while (max_extra >= int(tree_ways.size())) {
			int n = int(tree_ways.size());
			tree_ways.push_back(0);
			tree_height.push_back({-1, -1});
			for (int a = 0, b = n-1; a <= n-1 && b >= 0; a++, b--) {
				if (std::max(tree_height[a][1], tree_height[b][1]) - std::min(tree_height[a][0], tree_height[b][0]) <= 1) {
					tree_height[n][1] = std::max(tree_height[a][1], tree_height[b][1]) + 1;
					tree_height[n][0] = std::min(tree_height[a][0], tree_height[b][0]) + 1;
					tree_ways[n] += tree_ways[a] * tree_ways[b] * choose[n-1][a];
				}
			}
		}

		std::map<state_t, sat_num> memo;
		auto solve = std::y_combinator([&](auto self, const state_t s) -> sat_num {
			if (memo.count(s)) return memo[s];
			sat_num res = 0;
			if (s.stk_idx == 0 && s.is_full) {
				assert(s.root_val == N);
				assert(s.height == L+1);
				if (s.num_filled == N) {
					res = 1;
				}
			} else if (!s.is_full) {
				// push right
				assert(s.height >= 1);
				int min_cnt = (s.height == 1) ? 0 : ((1 << (s.height - 2)) - 1);
				int max_cnt = (1 << (s.height - 1)) - 1;
				int num_choices = N - s.num_filled - unused_geq[s.root_val+1];
				max_cnt = std::min(max_cnt, num_choices);
				for (int cnt = min_cnt; cnt <= max_cnt; cnt++) {
					if (s.num_filled + cnt > N) continue;
					state_t ns = s;
					ns.is_full = true;
					ns.num_filled = s.num_filled + cnt;
					sat_num ways = choose[num_choices][cnt] * tree_ways[cnt];
					res += ways * self(ns);
				}
			} else {
				assert(s.stk_idx > 0);
				assert(s.root_val < stk[s.stk_idx - 1].val);
				// first, we can go right
				if (s.num_filled < N && s.height + 1 <= stk[s.stk_idx - 1].min_height) {
					for (int v = nxt_unused[s.root_val+1]; v < stk[s.stk_idx-1].val; v = nxt_unused[v+1]) {
						state_t ns = s;
						ns.is_full = false;
						ns.num_filled++;
						ns.root_val = v;
						ns.height++;
						res += self(ns);
					}
				}
				// otherwise, we can take from the stack
				if (s.height <= stk[s.stk_idx - 1].min_height && stk[s.stk_idx - 1].max_height <= s.height + 1) {
					state_t ns = s;
					ns.root_val = stk[s.stk_idx - 1].val;
					ns.stk_idx--;
					ns.height++;
					res += self(ns);
				}
			}
			//cerr << "state " << s.is_full << ' ' << s.num_filled << ' ' << s.root_val << ' ' << s.stk_idx << ' ' << s.height << " => " << res.v << '\n';
			return memo[s] = res;
		});

		sat_num ans = 0;
		ans += solve(state_t{false, int(pref.size()), stk.back().val, int(stk.size()) - 1, stk.back().max_height});
		if (N != (1 << L) - 1 && stk.back().min_height == stk.back().max_height) {
			ans += solve(state_t{false, int(pref.size()), stk.back().val, int(stk.size()) - 1, stk.back().max_height + 1});
		}
		return ans;
	};

	if (false) {
		auto cnt_ways_slow = [&](const std::vector<int>& pref) -> sat_num {
			std::vector<int> P; P.reserve(N);
			std::vector<bool> used(N);
			for (int v : pref) {
				P.push_back(v);
				used[v] = true;
			}
			for (int v = 0; v < N; v++) {
				if (!used[v]) P.push_back(v);
			}
			sat_num ans = 0;
			do {
				ans += is_balanced(P);
			} while (std::next_permutation(P.begin() + int(pref.size()), P.end()));
			return ans;
		};
		std::vector<int> A(N);
		std::iota(A.begin(), A.end(), 0);
		std::set<std::vector<int>> done;
		do {
			if (is_balanced(A)) {
				auto P = A;
				while (true) {
					if (done.count(P)) break;
					done.insert(P);
					auto slo = cnt_ways_slow(P);
					auto fast = cnt_ways(P);
					if (slo.v != fast.v) {
						cerr << "P = "; for (int v : P) { cerr << v << ' '; } cerr << '\n';
						cerr << slo.v << ' ' << fast.v << '\n';
					}
					if (P.empty()) break;
					P.pop_back();
				}
			}
		} while (std::next_permutation(A.begin(), A.end()));
	}

	for (int dir = 0; dir < 2; dir++) {
		assert(K[dir] < INF);
		assert(K[dir] > 0);
		K[dir]--;
		std::vector<int> P(N, -1);
		if (dir == 0) {
			int num_delete = (1 << L) - 1 - N;
			int idx = 0;
			int val = 0;
			int cur_delete = 0;
			std::y_combinator([&](auto self, int h) -> void {
				if (h == 0) return;
				if (h == 1) {
					if (cur_delete > 0) {
						cur_delete --;
						return;
					}
					if (num_delete > 0) {
						cur_delete = 1;
						while (2 * cur_delete + 1 <= num_delete) {
							cur_delete = 2 * cur_delete + 1;
						}
						num_delete -= cur_delete;
						// keep going on this node
					}
				}
				self(h-1);
				int cur_idx = idx++;
				self(h-1);
				P[cur_idx] = val++;
			})(L);
			assert(idx == N && val == N);
		} else {
			int idx = 0;
			int val = 0;
			std::y_combinator([&](auto self, int n) -> void {
				if (n > N) return;
				self(2*n);
				int cur_idx = idx++;
				self(2*n+1);
				P[cur_idx] = val++;
			})(1);
			std::reverse(P.begin(), P.end());
		}

		std::vector<int> unused; unused.reserve(N);
		while (cnt_ways(P).v <= K[dir]) {
			if (P.empty()) {
				goto bad;
			}
			unused.push_back(P.back());
			P.pop_back();
		}

		std::sort(unused.begin(), unused.end());
		if (dir == 1) {
			std::reverse(unused.begin(), unused.end());
		}
		while (int(P.size()) < N) {
			for (int v : unused) {
				P.push_back(v);
				int64_t a = cnt_ways(P).v;
				if (a > K[dir]) {
					break;
				} else {
					K[dir] -= a;
					P.pop_back();
					continue;
				}
			}
			unused.erase(std::find(unused.begin(), unused.end(), P.back()));
		}
		for (int i = 0; i < N; i++) {
			cout << P[i]+1 << " \n"[i+1==N];
		}
		continue;
bad:
		cout << -1 << '\n';
	}

	return 0;
}
