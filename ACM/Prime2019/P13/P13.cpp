#include<bits/stdc++.h>
using namespace std;

using ll = long long;

const int MAXN = 1.1e5;
const int MAXM = 1.1e5;
int N, M;
int A[MAXN];
int B[MAXM];
int C, D;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N >> M;
	for (int i = 0; i < N; i++) {
		cin >> A[i];
	}
	for (int j = 0; j < M; j++) {
		cin >> B[j];
	}
	C = A[0], D = B[0];
	sort(A, A + N);
	sort(B, B + M);
	C = int(lower_bound(A, A + N, C) - A);
	D = int(lower_bound(B, B + M, D) - B);

	int mi = A[0] + B[0] - 1;
	int ma = A[C] + B[D];

	while (ma - mi > 1) {
		int md = (mi + ma) / 2;

		ll curSum = 0;
		bool bestUnchanged = false;

		int x1 = N, y1 = 0;
		int x2 = 0, y2 = M;

		for (int t = 0; t < N+M; t++) {
			if (x1 == 0) {
				y1++;
			} else if (y1 == M) {
				x1--;
			} else if (A[x1 - 1] + B[y1] <= md) {
				y1++;
			} else {
				x1--;
			}

			if (y2 == 0) {
				x2++;
			} else if (x2 == N) {
				y2--;
			} else if (A[x2] + B[y2 - 1] <= md) {
				x2++;
			} else {
				y2 --;
			}

			assert((N-x1) - x2 == (M-y2) - y1);
			curSum += x2 - (N-x1);
			if (x2 == (N-x1)) {
				if (curSum > 0) {
					curSum = 0;
					bestUnchanged = false;
				}
				if (curSum == 0 && !(x2 <= C && y1 <= D)) {
					bestUnchanged = true;
				}
			}
		}

		assert(x1 == 0 && y1 == M);
		assert(x2 == N && y2 == 0);

		if (bestUnchanged) {
			// flow changes by removing this
			// first player wins
			ma = md;
		} else {
			mi = md;
		}
	}

	cout << ma << '\n';

	return 0;
}
