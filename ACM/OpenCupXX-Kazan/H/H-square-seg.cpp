#pragma GCC optimize("Ofast", "unroll-loops")
#pragma GCC target("sse,sse2,sse3,ssse3,popcnt,abm,mmx")

#include<bits/stdc++.h>
using namespace std;

const int MAXN = 40000;

int N;
int A[MAXN];

int V[MAXN * 2];
int nxt[MAXN * 2];
int prv[MAXN * 2];

pair<pair<int, int>, pair<int, int>> insertions[MAXN];

const int S = 1 << 16;
struct seg_single_node {
	int bestSt;
	int bestEn;
	int bestRange;
} seg_single[2*S];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int Q;
	cin >> N >> Q;
	for (int i = 0; i < N; i++) {
		cin >> A[i];
	}

	V[0] = 0;
	for (int i = 1; i < 2 * N; i++) {
		V[i] = -V[i-1];
		if (i & 1) {
			V[i] += A[i/2];
		}
	}

	for (int i = 0; i < N; i++) {
		seg_single[S+i].bestSt = V[2*i+0];
		seg_single[S+i].bestEn = V[2*i+1];
		seg_single[S+i].bestRange = A[i];
	}
	for (int i = S-1; i > 0; i--) {
		seg_single[i].bestSt = max(seg_single[2*i].bestSt, seg_single[2*i+1].bestSt);
		seg_single[i].bestEn = max(seg_single[2*i].bestEn, seg_single[2*i+1].bestEn);
		seg_single[i].bestRange = max(max(seg_single[2*i].bestRange, seg_single[2*i+1].bestRange),
				seg_single[2*i].bestSt + seg_single[2*i+1].bestEn);
	}

	auto query_seg_single_start = [](int l, int r) {
		int ans = V[2*l];
		for (l += S, r += S; l < r; l /= 2, r /= 2) {
			if (l & 1) {
				ans = max(ans, seg_single[l].bestSt);
				++l;
			}
			if (r & 1) {
				--r;
				ans = max(ans, seg_single[r].bestSt);
			}
		}
		return ans;
	};
	auto query_seg_single_end = [](int l, int r) {
		int ans = V[2*r-1];
		for (l += S, r += S; l < r; l /= 2, r /= 2) {
			if (l & 1) {
				ans = max(ans, seg_single[l].bestEn);
				++l;
			}
			if (r & 1) {
				--r;
				ans = max(ans, seg_single[r].bestEn);
			}
		}
		return ans;
	};
	auto query_seg_single_range = [](int l, int r) {
		assert(l < r);
		int bestL = V[2*l];
		int bestR = V[2*r-1];
		int ans = bestL + bestR;
		for (l += S, r += S; l < r; l /= 2, r /= 2) {
			if (l & 1) {
				ans = max(ans, seg_single[l].bestRange);
				ans = max(ans, bestL + seg_single[l].bestEn);
				bestL = max(bestL, seg_single[l].bestSt);
				++l;
			}
			if (r & 1) {
				--r;
				ans = max(ans, seg_single[r].bestRange);
				ans = max(ans, seg_single[r].bestSt + bestR);
				bestR = max(bestR, seg_single[r].bestEn);
			}
		}
		ans = max(ans, bestL + bestR);
		return ans;
	};

	for (int i = 0; i < 2 * N; i++) {
		nxt[i] = i+1;
		prv[i] = i-1;
	}
	set<pair<int, int>> evts;
	for (int i = 0; i+1 < 2 * N; i++) {
		evts.emplace(V[i] + V[i+1], i);
	}
	for (int t = 0; t < N; t++) {
		auto evt = *evts.begin();
		evts.erase(evt);

		int i = evt.second;
		int j = nxt[i];
		insertions[N-1-t] = {{i, V[i]}, {j, V[j]}};

		if (prv[i] != -1) {
			evts.erase(pair<int, int>(V[prv[i]] + V[i], prv[i]));
			nxt[prv[i]] = nxt[j];
		}
		if (nxt[j] != 2 * N) {
			evts.erase(pair<int, int>(V[j] + V[nxt[j]], j));
			prv[nxt[j]] = prv[i];
		}

		if (prv[i] != -1 && nxt[j] != 2 * N) {
			evts.emplace(V[prv[i]] + V[nxt[j]], prv[i]);
		}
	}
	//assert(evts.empty());

	while (Q--) {
		int L, R, K; cin >> L >> R >> K; L--;

		int cnt = 0;
		int sumVal = 0;

		int minPt = 2*N;
		int minPtV = 0;
		int maxPt = -1;
		int maxPtV = 0;

		int ans = -2e9;

		if (K == 1) {
			ans = query_seg_single_range(L, R);
			cout << ans << '\n';
			continue;
		}

		for (int t = 0; t < N; t++) {
			const auto& it = insertions[t];
			bool changed = false;
			if (2 * L <= it.first.first && it.first.first < 2 * R) {
				if (it.first.first < minPt) {
					minPt = it.first.first;
					minPtV = it.first.second;
				}
				if (it.first.first > maxPt) {
					maxPt = it.first.first;
					maxPtV = it.first.second;
				}
				cnt++;
				sumVal += it.first.second;
				changed = true;
			}
			if (2 * L <= it.second.first && it.second.first < 2 * R) {
				if (it.second.first < minPt) {
					minPt = it.second.first;
					minPtV = it.second.second;
				}
				if (it.second.first > maxPt) {
					maxPt = it.second.first;
					maxPtV = it.second.second;
				}
				cnt++;
				sumVal += it.second.second;
				changed = true;
			}

			if (!changed) {
				continue;
			}

			if (cnt + 2 < 2*K) continue;
			if (cnt - 2 > 2*K) break;

			bool leftBad = minPt & 1;
			bool rightBad = !(maxPt & 1);

			assert((cnt + leftBad + rightBad) % 2 == 0);

			if (cnt + leftBad + rightBad < 2*K) continue;
			if (cnt - leftBad - rightBad > 2*K) break;

			if (cnt - leftBad - rightBad == 2*K) {
				ans = max(ans, sumVal - (leftBad ? minPtV : 0) - (rightBad ? maxPtV : 0));
				break;
			}
			if (leftBad && cnt + leftBad - rightBad == 2*K) {
				int lbest = query_seg_single_start(L, (minPt + 1) / 2);
				ans = max(ans, sumVal - (rightBad ? maxPtV : 0) + lbest);
			}
			if (rightBad && cnt - leftBad + rightBad == 2*K) {
				int rbest = query_seg_single_end(maxPt / 2, R);
				ans = max(ans, sumVal - (leftBad ? minPtV : 0) + rbest);
			}
			if (leftBad && rightBad && cnt + leftBad + rightBad == 2*K) {
				int lbest = query_seg_single_start(L, (minPt + 1) / 2);
				int rbest = query_seg_single_end(maxPt / 2, R);
				ans = max(ans, sumVal + lbest + rbest);
			}
		}
		cout << ans << '\n';
	}

	return 0;
}
