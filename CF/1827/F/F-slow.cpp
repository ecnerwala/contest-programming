#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

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

		int64_t lo_ans = 0;
		int64_t hi_ans = c2(N+1);
		int64_t mid_ans = 0;
		// mid_ans is sum over stack[:-1] of (sz if INCR/DECR else 1) (stack[-1] is guaranteed to be complete and already counted)
		// plus sum over all distinct gaps of (gap size) * max(0 if PARTIAL else sz+1)

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
		};

		std::vector<cnd_t> stk; stk.reserve(N);

		std::vector<int64_t> res; res.reserve(N+1);
		res.push_back(lo_ans + mid_ans + hi_ans);

		for (int i = 0; i < N; i++) {
			int a = A[i];

			while (!stk.empty() && (a < stk.back().lo_gap || a > stk.back().hi_gap)) {
				assert(stk.size() >= 2);
				stk.end()[-2].lo = std::min(stk.end()[-2].lo, stk.back().lo);
				stk.end()[-2].hi = std::max(stk.end()[-2].hi, stk.back().hi);

				stk.pop_back();

				stk.back().node_type = NodeType::PARTIAL;
				stk.back().node_sz = 0;
			}

			hi_ans -= c2(nxt_earlier[a] - prv_earlier[a]);
			hi_ans += c2(nxt_earlier[a] - a);
			hi_ans += c2(a - prv_earlier[a]);
			stk.push_back({i, a, prv_earlier[a]+1, a, nxt_earlier[a]-1, NodeType::PARTIAL, 0});
			//cerr << "Run " << i << ' ' << a << ' ' << lo_ans << ' ' << hi_ans << '\n';
			//for (auto it : stk) { cerr << it.left << ' ' << it.lo_gap << '-' << it.lo << '-' << it.hi << '-' << it.hi_gap << ' ' << int(it.node_type) << ' ' << it.node_sz << '\n'; }

			mid_ans = 0;
			for (int z = 0; z < int(stk.size()); z++) {
				if (stk[z].node_type == NodeType::DECR && a > stk[z].hi) {
					mid_ans += 1;
				} else if (stk[z].node_type == NodeType::INCR && a < stk[z].lo) {
					mid_ans += 1;
				} else {
					mid_ans += std::max(stk[z].node_sz, 1);
				}
			}
			//cerr << "base mid " << mid_ans << '\n';
			for (int z = 0, w = 0; z < int(stk.size()); z = w) {
				while (w < int(stk.size()) && stk[w].hi_gap == stk[z].hi_gap) {
					w++;
				}
				//cerr << "hi " << z << ' ' << w << '\n';
				int best = 0;
				for (int x = z; x < w-1; x++) {
					if (stk[x].node_type == NodeType::DECR) {
						best = std::max(best, 1);
					} else {
						best = std::max(best, stk[x].node_sz);
					}
				}
				int gap_sz = stk[w-1].hi_gap - stk[w-1].hi;
				assert(gap_sz >= 0);
				//cerr << gap_sz << ' ' << best << '\n';
				mid_ans += int64_t(gap_sz) * int64_t(best + 1);
			}
			for (int z = 0, w = 0; z < int(stk.size()); z = w) {
				while (w < int(stk.size()) && stk[w].lo_gap == stk[z].lo_gap) {
					w++;
				}
				//cerr << "lo " << z << ' ' << w << '\n';
				int best = 0;
				for (int x = z; x < w-1; x++) {
					if (stk[x].node_type == NodeType::INCR) {
						best = std::max(best, 1);
					} else {
						best = std::max(best, stk[x].node_sz);
					}
				}
				int gap_sz = stk[w-1].lo - stk[w-1].lo_gap;
				assert(gap_sz >= 0);
				//cerr << gap_sz << ' ' << best << '\n';
				mid_ans += int64_t(gap_sz) * int64_t(best + 1);
			}
			res.push_back(lo_ans + mid_ans + hi_ans);
			//cerr << "mid_ans " << mid_ans << '\n';

			stk.back().node_type = NodeType::LEAF;
			stk.back().node_sz = 1;
			lo_ans ++;

			while (stk.size() >= 2 && std::max(stk.back().hi, stk.end()[-2].hi) - std::min(stk.back().lo, stk.end()[-2].lo) == i - stk.end()[-2].left) {
				// merge these two nodes into one
				stk.end()[-2].lo = std::min(stk.end()[-2].lo, stk.back().lo);
				stk.end()[-2].hi = std::max(stk.end()[-2].hi, stk.back().hi);

				NodeType node_type;
				int node_sz;
				if (stk.end()[-2].lo == stk.end()[-1].lo) {
					node_type = NodeType::DECR;
					node_sz = (stk.end()[-2].node_type == node_type ? stk.end()[-2].node_sz : 1) + 1;
					lo_ans += node_sz - 1;
				} else if (stk.end()[-2].hi == stk.end()[-1].hi) {
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
