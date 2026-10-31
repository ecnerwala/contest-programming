#include <bits/stdc++.h>

// Monotonicity of decision point! The matrix is totally monotone, so we can use SMAWK and the like.
template <typename T> using min_pq = std::priority_queue<T, std::vector<T>, std::greater<T>>;
template <typename T> using max_pq = std::priority_queue<T, std::vector<T>, std::less<T>>;

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, Q; cin >> N >> Q;
	struct task_t {
		int a, b;
	};
	std::vector<task_t> tasks(N);
	for (auto& [a, b] : tasks) cin >> a >> b;
	std::sort(tasks.begin(), tasks.end(), [&](auto x, auto y) { return (x.a + x.b) > (y.a + y.b); });
	// higher k has higher prefix size
	std::vector<int64_t> suffA(N+1);
	for (int i = N-1; i >= 0; i--) suffA[i] = suffA[i+1] + tasks[i].a;

	std::vector<int> opt(N+1);
	std::vector<int64_t> ans(N+1);
	opt[0] = 0;
	ans[0] = suffA[0];
	for (int b = 31 - __builtin_clz(N); b >= 0; b--) {
		int cur_cut = 0;
		int64_t sum_small_b = 0;
		max_pq<int> small_b;
		min_pq<int> big_b;

		for (int i = (1 << b); i < N; i += (2 << b)) {
			// solve guy i
			int lo = std::max(i, opt[i - (1 << b)]);
			int hi = (i + (1 << b)) <= N ? opt[i + (1 << b)] : N;
			// lo..hi inclusive
			opt[i] = -1;
			for (int tgt_cut = lo; tgt_cut <= hi; tgt_cut++) {
				assert(cur_cut <= tgt_cut);
				for (; cur_cut < tgt_cut; cur_cut++) {
					int new_b = tasks[cur_cut].b;
					// insert new_b in the correct place
					if (small_b.empty() || new_b > small_b.top())  {
						big_b.push(new_b);
					} else {
						sum_small_b += new_b;
						small_b.push(new_b);
					}
				}
				while (int(small_b.size()) > i) {
					sum_small_b -= small_b.top();
					big_b.push(small_b.top());
					small_b.pop();
				}
				while (int(small_b.size()) < i) {
					assert(!big_b.empty());
					sum_small_b += big_b.top();
					small_b.push(big_b.top());
					big_b.pop();
				}

				assert(cur_cut == tgt_cut);
				int64_t cnd = suffA[cur_cut] - sum_small_b;
				cerr << i << ' ' << cur_cut << ' ' << cnd << '\n';
				if (cur_cut == lo || cnd > ans[i]) {
					ans[i] = cnd;
					opt[i] = cur_cut;
				}
			}
		}
	}

	while (Q--) {
		int k; cin >> k; cout << ans[k] << '\n';
	}

	return 0;
}
