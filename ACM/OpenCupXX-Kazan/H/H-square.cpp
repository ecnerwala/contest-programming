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
			ans = A[L];
			int cur = A[L];
			for (int i = L+1; i < R; i++) {
				cur = max(cur, 0) + A[i];
				ans = max(ans, cur);
			}
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
				int lbest = V[2*L];
				for (int i = 2 * L; i < minPt; i += 2) {
					lbest = max(lbest, V[i]);
				}
				ans = max(ans, sumVal - (rightBad ? maxPtV : 0) + lbest);
			}
			if (rightBad && cnt - leftBad + rightBad == 2*K) {
				int rbest = V[2*R-1];
				for (int i = 2*R - 1; i > maxPt; i -= 2) {
					rbest = max(rbest, V[i]);
				}
				ans = max(ans, sumVal - (leftBad ? minPtV : 0) + rbest);
			}
			if (leftBad && rightBad && cnt + leftBad + rightBad == 2*K) {
				int lbest = V[2*L];
				for (int i = 2 * L; i < minPt; i += 2) {
					lbest = max(lbest, V[i]);
				}
				int rbest = V[2*R-1];
				for (int i = 2*R - 1; i > maxPt; i -= 2) {
					rbest = max(rbest, V[i]);
				}
				ans = max(ans, sumVal + lbest + rbest);
			}
		}
		cout << ans << '\n';
	}


	return 0;
}
