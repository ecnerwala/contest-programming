#include <bits/stdc++.h>

int main() {
	using std::cin;
	using std::cout;
	using std::cerr;
	std::ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) { cin >> a; a--; }

		std::vector<int> nxt_earlier(N);
		std::vector<int> prv_earlier(N);
		for (int i = 0; i < N; i++) {
			nxt_earlier[i] = i+1;
			prv_earlier[i] = i-1;
		}
		for (int i = N-1; i >= 0; i--) {
			int a = A[i];
			int p = prv_earlier[a];
			int n = nxt_earlier[a];
			if (p != -1) nxt_earlier[p] = n;
			if (n != N) prv_earlier[n] = p;
		}

		auto c2 = [](int a) -> int64_t {
			assert(a >= 0);
			return int64_t(a) * int64_t(a-1) / 2;
		};

		// lo_ans is strictly before i, hi_ans is strictly after i
		int64_t lo_ans = 0;
		int64_t hi_ans = c2(N+1);
		// We store mid_ans (things using i) in the persistent stack

		enum class NodeType {
			LEAF,
			INCR,
			DECR,
			FULL,
			PARTIAL,
		};
		struct cnd_t {
			int left;
			int lo;
			int lo_gap;
			int hi;
			int hi_gap;
			NodeType node_type;
			int node_sz;

			int64_t mid_ans = 0;

			int lo_best = 0;
			int64_t lo_cur = 0;
			int hi_best = 0;
			int64_t hi_cur = 0;
		};

		std::vector<cnd_t> stk; stk.reserve(N);

		std::vector<int64_t> res; res.reserve(N+1);
		res.push_back(lo_ans + 0 + hi_ans);

		auto update_back = [&]() -> void {
			int z = int(stk.size()) - 1;
			assert(z >= 0);
			stk[z].mid_ans = z == 0 ? 0 : stk[z-1].mid_ans;
			stk[z].mid_ans += std::max(stk[z].node_sz, 1);
			{
				int lo_gap_sz = stk[z].lo - stk[z].lo_gap;
				assert(lo_gap_sz >= 0);
				if (z > 0 && stk[z].lo_gap == stk[z-1].lo_gap) {
					stk[z].lo_best = std::max(stk[z-1].lo_best, stk[z-1].node_sz);
					stk[z].mid_ans -= stk[z-1].lo_cur;
				} else {
					stk[z].lo_best = 0;
				}
				stk[z].lo_cur = int64_t(lo_gap_sz) * int64_t(stk[z].lo_best + 1);
				stk[z].mid_ans += stk[z].lo_cur;
			}
			{
				int hi_gap_sz = stk[z].hi_gap - stk[z].hi;
				assert(hi_gap_sz >= 0);
				if (z > 0 && stk[z].hi_gap == stk[z-1].hi_gap) {
					stk[z].hi_best = std::max(stk[z-1].hi_best, stk[z-1].node_sz);
					stk[z].mid_ans -= stk[z-1].hi_cur;
				} else {
					stk[z].hi_best = 0;
				}
				stk[z].hi_cur = int64_t(hi_gap_sz) * int64_t(stk[z].hi_best + 1);
				stk[z].mid_ans += stk[z].hi_cur;
			}
		};

		for (int i = 0; i < N; i++) {
			int a = A[i];

			while (!stk.empty() && (a < stk.back().lo_gap || a > stk.back().hi_gap)) {
				assert(stk.size() >= 2);
				if (stk.end()[-2].lo_gap == stk.end()[-1].lo_gap) {
					stk.end()[-2].lo = stk.end()[-1].lo;
				} else if (stk.end()[-2].hi_gap == stk.end()[-1].hi_gap) {
					stk.end()[-2].hi = stk.end()[-1].hi;
				}

				stk.pop_back();

				stk.back().node_type = NodeType::PARTIAL;
				stk.back().node_sz = 0;
			}

			if (!stk.empty()) {
				if (stk.back().node_type == NodeType::DECR) {
					assert(a < stk.back().lo || a > stk.back().hi);
					if (a > stk.back().hi) {
						stk.back().node_type = NodeType::FULL;
						stk.back().node_sz = 1;
					}
				} else if (stk.back().node_type == NodeType::INCR) {
					assert(a < stk.back().lo || a > stk.back().hi);
					if (a < stk.back().lo) {
						stk.back().node_type = NodeType::FULL;
						stk.back().node_sz = 1;
					}
				}
			}

			hi_ans -= int64_t(nxt_earlier[a] - a) * int64_t(a - prv_earlier[a]);

			if (!stk.empty()) {
				// Until this point, we were just merging/joining the single back node, so we don't need to call update in any other places
				update_back();
			}
			stk.push_back({i, a, prv_earlier[a]+1, a, nxt_earlier[a]-1, NodeType::PARTIAL, 0});
			update_back();
			//cerr << "Run " << i << ' ' << a << ' ' << lo_ans << ' ' << hi_ans << '\n';
			//for (auto it : stk) { cerr << it.left << ' ' << it.lo_gap << '-' << it.lo << '-' << it.hi << '-' << it.hi_gap << ' ' << int(it.node_type) << ' ' << it.node_sz << '\n'; }

			res.push_back(lo_ans + stk.back().mid_ans + hi_ans);
			//cerr << "mid_ans " << mid_ans << '\n';

			stk.back().node_type = NodeType::LEAF;
			stk.back().node_sz = 1;
			lo_ans ++;

			while (stk.size() >= 2 && std::max(stk.back().hi, stk.end()[-2].hi) - std::min(stk.back().lo, stk.end()[-2].lo) == i - stk.end()[-2].left) {
				// merge these two nodes into one
				NodeType node_type;
				int node_sz;
				if (stk.end()[-2].lo_gap == stk.end()[-1].lo_gap) {
					stk.end()[-2].lo = stk.end()[-1].lo;
					node_type = NodeType::DECR;
					node_sz = (stk.end()[-2].node_type == node_type ? stk.end()[-2].node_sz : 1) + 1;
					lo_ans += node_sz - 1;
				} else if (stk.end()[-2].hi_gap == stk.end()[-1].hi_gap) {
					stk.end()[-2].hi = stk.end()[-1].hi;
					node_type = NodeType::INCR;
					node_sz = (stk.end()[-2].node_type == node_type ? stk.end()[-2].node_sz : 1) + 1;
					lo_ans += node_sz - 1;
				} else {
					node_type = NodeType::FULL;
					node_sz = 1;
					lo_ans ++;
				}

				stk.pop_back();
				stk.back().node_type = node_type;
				stk.back().node_sz = node_sz;
			}
		}
		assert(int(res.size()) == N+1);
		for (int i = 0; i <= N; i++) {
			cout << res[i] << " \n"[i==N];
		}
	}

	return 0;
}
