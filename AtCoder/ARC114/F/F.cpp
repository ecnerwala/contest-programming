#include <bits/extc++.h>
#define sealpups main

template <typename K, typename V, typename Comp = std::less<K>>
using order_statistic_map = __gnu_pbds::tree<
	K, V, Comp,
	__gnu_pbds::rb_tree_tag,
	__gnu_pbds::tree_order_statistics_node_update
>;

template <typename K, typename Comp = std::less<K>>
using order_statistic_set = order_statistic_map<K, __gnu_pbds::null_type, Comp>;

// Supports
//  auto iterator = order_statistic_set().find_by_order(idx); // (0-indexed)
//  int num_strictly_smaller = order_statistic_set().order_of_key(key);

int sealpups() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, K; cin >> N >> K;
	vector<int> P(N); for (auto& p : P) { cin >> p; p--; }
	vector<int> invP(N);
	for (int i = 0; i < N; i++) invP[P[i]] = i;

	vector<bool> shouldCut(N);

	// We take a decreasing subsequence of cuts from 0 to a point, and then take the remaining lowest ones right/down of this point
	vector<int> decLen(N, -1);
	vector<int> decPrv(N, -1);
	vector<int> stk; stk.push_back(P[0]);
	decLen[0] = 1;
	for (int i = 1; i < N; i++) {
		if (P[i] > P[0]) continue;
		int mi = 0;
		int ma = int(stk.size());
		while (ma - mi > 1) {
			int md = (mi + ma) / 2;
			if (P[i] < stk[md]) mi = md;
			else ma = md;
		}

		decPrv[i] = invP[stk[mi]];
		decLen[i] = ma+1;

		if (ma == int(stk.size())) {
			stk.push_back(P[i]);
		} else {
			stk[ma] = P[i];
		}
	}

	pair<pair<int, int>, int> bestLoc{{0, -K}, -1};
	order_statistic_set<int> aliveLocs;
	for (int v = 0; v < N; v++) {
		int i = invP[v];
		if (decLen[i] != -1) {
			if (decLen[i] > K) {
				// just use the earlier guy
			} else if (decLen[i] == K) {
				// just take this guy
				bestLoc = {{N, -0}, i};
			} else {
				int num_after = K-decLen[i];
				if (num_after <= int(aliveLocs.size())) {
					int j = *aliveLocs.find_by_order(int(aliveLocs.size()) - num_after);
					if (j > i) {
						bestLoc = max(bestLoc, {{j, -num_after}, i});
					}
				}
			}
		}
		aliveLocs.insert(i);
	}

	// now we have bestLoc, now we just take that stuff
	for (int cur = bestLoc.second; cur != -1; cur = decPrv[cur]) shouldCut[cur] = true;

	int j = bestLoc.first.first;
	int c = -bestLoc.first.second;
	for (int v = 0; c > 0; v++) {
		if (invP[v] >= j) {
			c--;
			shouldCut[invP[v]] = true;
		}
	}
	assert(c == 0);

	vector<int> cuts; cuts.reserve(K);
	for (int i = 0; i < N; i++) {
		if (shouldCut[i]) cuts.push_back(P[i]);
	}
	sort(cuts.begin(), cuts.end());
	reverse(cuts.begin(), cuts.end());

	vector<int> Q; Q.reserve(N);
	for (int s : cuts) {
		Q.push_back(s);
		for (int i = invP[s]+1; i < N && !shouldCut[i]; i++) {
			Q.push_back(P[i]);
		}
	}

	for (int i = 0; i < N; i++) {
		cout << Q[i]+1 << " \n"[i+1==N];
	}

	return 0;
}
