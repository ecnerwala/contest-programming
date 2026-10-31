#include <bits/stdc++.h>

template <typename T> void setmax(T& a, T b) { if (b > a) a = b; }
template <typename T> void setmin(T& a, T b) { if (b < a) a = b; }

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T, P; cin >> T >> P;
	if (P == 0) {
		while (T--) {
			int N, M; cin >> N >> M;
			vector<array<int, 2>> ranges(M);
			for (auto& r : ranges) { cin >> r[0] >> r[1]; r[0]--; }
			// Every range is a permutation; if you're not in a
			vector<int> cover_cnt(N+1);
			for (auto [l, r] : ranges) {
				cover_cnt[l]++;
				cover_cnt[r]--;
			}
			vector<int> A(N);
			for (int i = 0; i < N; i++) {
				cover_cnt[i+1] += cover_cnt[i];
				A[i] = cover_cnt[i] ? i : 0;
			}
			for (int i = 0; i < N; i++) {
				cout << A[i]+1 << " \n"[i+1==N];
			}
		}
	} else if (P == 1) {
		while (T--) {
			int N, M; cin >> N >> M;
			vector<array<int, 2>> ranges(M);
			for (auto& r : ranges) { cin >> r[0] >> r[1]; r[0]--; }
			vector<int> A(N, N-1);
			vector<array<int, 2>> seg(2*N, {-1, N+1});
			for (auto& [l, r] : ranges) {
				for (int a = N+l, b = N+r; a < b; a /= 2, b /= 2) {
					if (a & 1) {
						setmax(seg[a][0], l);
						setmin(seg[a][1], r);
						a++;
					}
					if (b & 1) {
						--b;
						setmax(seg[b][0], l);
						setmin(seg[b][1], r);
					}
				}
			}
			for (int a = 2; a < 2*N; a++) {
				setmax(seg[a][0], seg[a/2][0]);
				setmin(seg[a][1], seg[a/2][1]);
			}

			set<int> alive;
			for (int i = 0; i < N; i++) {
				alive.insert(i);
			}

			vector<pair<array<int, 2>, int>> bounds(N);
			for (int i = 0; i < N; i++) {
				bounds[i].first = {seg[N+i][1] - seg[N+i][0], seg[N+i][0]};
				bounds[i].second = i;
			}
			sort(bounds.begin(), bounds.end());
			for (int i = 0, j; i < N; i = j) {
				j = i;
				while (j < N && bounds[j].first == bounds[i].first) j++;
				auto [r, l] = bounds[i].first;
				r += l;
				if (l == -1) {
					for (int k = j-1; k >= i; k--) {
						A[bounds[k].second] = N-1;
					}
					assert(j == N);
					break;
				}
				auto it = alive.lower_bound(l);
				for (int k = j-1; k >= i; k--) {
					A[bounds[k].second] = *it;
					it = alive.erase(it);
				}
			}

			for (int i = 0; i < N; i++) {
				cout << A[i]+1 << " \n"[i+1==N];
			}
		}
	} else assert(false);

	return 0;
}
