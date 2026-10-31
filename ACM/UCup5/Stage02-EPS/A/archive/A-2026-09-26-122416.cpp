#pragma GCC optimize("Ofast", "unroll-loops")
//#pragma GCC target("sse,sse2,sse3,ssse3,popcnt,abm,mmx")

#include<bits/extc++.h>
using namespace std;

const int MAXN = 210000;

int N;
int A[MAXN];

int64_t V[MAXN * 2];

pair<pair<int, int>, pair<int, int>> insertions[MAXN];
int insertionTime[2*MAXN];
namespace insertion_builder {

int nxt[MAXN * 2];
int prv[MAXN * 2];

void build() {
	for (int i = 0; i < 2 * N; i++) {
		nxt[i] = i+1;
		prv[i] = i-1;
	}

	__gnu_pbds::priority_queue<pair<int, int>, std::greater<pair<int, int>>, __gnu_pbds::pairing_heap_tag> evts;
	vector<decltype(evts)::point_iterator> its(2*N-1);

	for (int i = 0; i+1 < 2 * N; i++) {
		its[i] = evts.push({V[i] + V[i+1], i});
	}
	for (int t = 0; t < N; t++) {
		auto evt = evts.top(); evts.pop();

		int i = evt.second;
		int j = nxt[i];
		insertionTime[i] = insertionTime[j] = N-1-t;
		insertions[N-1-t] = {{i,V[i]},{j,V[j]}};

		if (prv[i] != -1) {
			assert(its[prv[i]] != evts.end());
			evts.erase(its[prv[i]]);
			nxt[prv[i]] = nxt[j];
		}
		if (nxt[j] != 2 * N) {
			assert(its[j] != evts.end());
			evts.erase(its[j]);
			prv[nxt[j]] = prv[i];
		}

		if (prv[i] != -1 && nxt[j] != 2 * N) {
			its[prv[i]] = evts.push({V[prv[i]] + V[nxt[j]], prv[i]});
		}
	}
	//assert(evts.empty());
}
} // namespace insertion_builder

namespace seg_static {
const int S = 1 << 18;
static_assert(S >= MAXN + 1);
struct node {
	int64_t bestSt;
	int64_t bestEn;
	int64_t bestRange;
} seg[2*S];

void init() {
	for (int i = 0; i < N; i++) {
		seg[S+i].bestSt = V[2*i+0];
		seg[S+i].bestEn = V[2*i+1];
		seg[S+i].bestRange = A[i];
	}
	for (int i = S-1; i > 0; i--) {
		seg[i].bestSt = max(seg[2*i].bestSt, seg[2*i+1].bestSt);
		seg[i].bestEn = max(seg[2*i].bestEn, seg[2*i+1].bestEn);
		seg[i].bestRange = max(max(seg[2*i].bestRange, seg[2*i+1].bestRange),
				seg[2*i].bestSt + seg[2*i+1].bestEn);
	}
}

int64_t query_start(int l, int r) {
	int64_t ans = V[2*l];
	for (l += S, r += S; l < r; l /= 2, r /= 2) {
		if (l & 1) {
			ans = max(ans, seg[l].bestSt);
			++l;
		}
		if (r & 1) {
			--r;
			ans = max(ans, seg[r].bestSt);
		}
	}
	return ans;
}
int64_t query_end(int l, int r) {
	int64_t ans = V[2*r-1];
	for (l += S, r += S; l < r; l /= 2, r /= 2) {
		if (l & 1) {
			ans = max(ans, seg[l].bestEn);
			++l;
		}
		if (r & 1) {
			--r;
			ans = max(ans, seg[r].bestEn);
		}
	}
	return ans;
}
int64_t query_range(int l, int r) {
	assert(l < r);
	int64_t bestL = V[2*l];
	int64_t bestR = V[2*r-1];
	int64_t ans = bestL + bestR;
	for (l += S, r += S; l < r; l /= 2, r /= 2) {
		if (l & 1) {
			ans = max(ans, seg[l].bestRange);
			ans = max(ans, bestL + seg[l].bestEn);
			bestL = max(bestL, seg[l].bestSt);
			++l;
		}
		if (r & 1) {
			--r;
			ans = max(ans, seg[r].bestRange);
			ans = max(ans, seg[r].bestSt + bestR);
			bestR = max(bestR, seg[r].bestEn);
		}
	}
	ans = max(ans, bestL + bestR);
	return ans;
}
} // namespace seg_static

namespace seg_time {
const int LG = 18;
const int S = 1 << LG;
static_assert(S >= MAXN + 1);
struct node {
	int ch[2];
	int prefCnt;
	int64_t prefSum;

	int nxtPt;
	int prvPt;
} pool[2 * S + (2 * MAXN) * (LG+1)];
int pool_sz = 0;

int roots[2*MAXN];

int init_tree() {
	pool_sz = 2 * S - 1;
	for (int i = 0; i < 2 * S - 1; i++) {
		if (i < S-1) {
			pool[i].ch[0] = 2*i+1;
			pool[i].ch[1] = 2*i+2;
		} else {
			pool[i].ch[0] = -1;
			pool[i].ch[1] = -1;
		}
		pool[i].prefCnt = 0;
		pool[i].prefSum = 0;
		pool[i].nxtPt = 2*N;
		pool[i].prvPt = -1;
	}
	return 0;
}

int insert_time(int node, int nodeL, int nodeR, int loc, int ind, int64_t sumVal) {
	assert(nodeL <= loc && loc < nodeR);

	int newNode = pool_sz++;
	pool[newNode] = pool[node];

	pool[newNode].prvPt = ind;
	pool[node].nxtPt = ind;

	pool[newNode].prefCnt++;
	pool[newNode].prefSum += sumVal;

	if (nodeR - nodeL > 1) {
		int nodeM = (nodeL + nodeR) / 2;
		if (loc < nodeM) {
			pool[newNode].ch[0] = insert_time(pool[newNode].ch[0], nodeL, nodeM, loc, ind, sumVal);
		} else {
			pool[newNode].ch[1] = insert_time(pool[newNode].ch[1], nodeM, nodeR, loc, ind, sumVal);
		}
	}

	return newNode;
}

void init() {
	roots[0] = init_tree();
	for (int i = 0; i < 2*N; i++) {
		roots[i+1] = insert_time(roots[i], 0, S, insertionTime[i], i, V[i]);
	}
}

// returns cnt, sum, leftPt, rightPt
struct range_data {
	int cnt; int64_t sum; int leftPt, rightPt;
};
pair<range_data, range_data> get_range_with_cnt(int LL, int RR, int cnt) {
	assert(cnt <= RR - LL);
	assert(cnt > 0);

	int lcur = roots[LL];
	int rcur = roots[RR];
	int leftPt = 2*N;
	int rightPt = -1;
	int64_t sum = 0;

	int remCnt = cnt;
	for (int z = 0; z < LG; z++) {
		int cndCnt = pool[pool[rcur].ch[0]].prefCnt - pool[pool[lcur].ch[0]].prefCnt;
		if (cndCnt <= remCnt) {
			remCnt -= cndCnt;
			leftPt = min(leftPt, pool[pool[lcur].ch[0]].nxtPt);
			rightPt = max(rightPt, pool[pool[rcur].ch[0]].prvPt);
			sum += pool[pool[rcur].ch[0]].prefSum - pool[pool[lcur].ch[0]].prefSum;
			lcur = pool[lcur].ch[1];
			rcur = pool[rcur].ch[1];
		} else {
			lcur = pool[lcur].ch[0];
			rcur = pool[rcur].ch[0];
		}
	}
	return {
		{cnt - remCnt, sum, leftPt, rightPt},
		{cnt - remCnt + pool[rcur].prefCnt - pool[lcur].prefCnt, sum + pool[rcur].prefSum - pool[lcur].prefSum, min(leftPt, pool[lcur].nxtPt), max(rightPt, pool[rcur].prvPt)}
	};
}

} // namespace seg_time

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	auto clk = chrono::system_clock();
	auto start = clk.now();
	auto print_time = [&](string s) {
		return;
		auto end = clk.now();
		auto diff = end - start;
		cerr << "time for " << s << ": " << double(chrono::nanoseconds(diff).count()) / 1e9 << '\n';
		start = clk.now();
	};

	int Q;
	int K_global;
	cin >> N >> K_global >> Q;
	int N_orig = N;
	{
		std::vector<int> vals(2*N);
		for (int i = 0; i < N; i++) {
			std::cin >> vals[i];
			vals[N+i] = vals[i];
		}
		for (auto& v : vals) std::cin >> v;
		N = 2 * N - 1;
		for (int i = 0; i < N; i++) {
			A[i] = vals[i+1] - vals[i];
		}
	}
	if (N == 0) {
		for (int q = 0; q < Q; q++) std::cout << 0 << '\n';
		exit(0);
	}
	std::vector<int> pref_pos(N+1);
	for (int i = 0; i < N; i++) pref_pos[i+1] = pref_pos[i] + (A[i] >= 0);

	V[0] = 0;
	for (int i = 1; i < 2 * N; i++) {
		V[i] = -V[i-1];
		if (i & 1) {
			V[i] += A[i/2];
		}
	}
	print_time("input read");

	seg_static::init();
	print_time("seg_static");
	insertion_builder::build();
	print_time("builder");
	seg_time::init();
	print_time("seg_time");

	while (Q--) {
		int L, R; cin >> L >> R; L--, R--;
		if (L > R) L += N_orig;
		int K = K_global;
		K = std::min(K, pref_pos[R] - pref_pos[L]);
		int LL = L<<1, RR = R<<1, KK = K<<1;

		if (K == 0) {
			std::cout << 0 << '\n';
			continue;
		} else if (K == 1) {
			cout << seg_static::query_range(L, R) << '\n';
			continue;
		}

		int64_t ans;

		const auto r = seg_time::get_range_with_cnt(LL, RR, KK-1);
		if (r.second.cnt == KK) {
			const auto& rmid = r.second;
			if (!(rmid.leftPt & 1)) {
				ans = rmid.sum;
			} else {
				int64_t lbest = seg_static::query_start(L, (rmid.leftPt+1)/2);
				int64_t rbest = seg_static::query_end((rmid.rightPt) / 2, R);
				ans = max(rmid.sum + lbest - V[rmid.rightPt], rmid.sum - V[rmid.leftPt] + rbest);

				const auto& rlo = r.first;
				if (rlo.cnt == KK-2) {
					ans = max(ans, rlo.sum + lbest + rbest);
				}
				auto rhi = seg_time::get_range_with_cnt(LL, RR, KK).second;
				if (rhi.cnt == KK+2) {
					ans = max(ans, rhi.sum - V[rhi.leftPt] - V[rhi.rightPt]);
				}
			}
		} else {
			assert(r.first.cnt == KK-1);
			assert(r.second.cnt == KK+1);
			ans = max(
				r.first.sum + (r.first.leftPt & 1 ? seg_static::query_start(L, (r.first.leftPt+1)/2) : 0) + (r.first.rightPt & 1 ? 0 : seg_static::query_end(r.first.rightPt / 2, R)),
				r.second.sum - (r.second.leftPt & 1 ? V[r.second.leftPt] : 0) - (r.second.rightPt & 1 ? 0 : V[r.second.rightPt])
			);
		}

		cout << ans << '\n';
	}
	print_time("queries");

	return 0;
}
