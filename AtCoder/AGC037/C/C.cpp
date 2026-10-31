#include<bits/stdc++.h>
using namespace std;

using ll = long long;

const int MAXN = 2.1e5;
int N;
int A[MAXN];
int B[MAXN];

[[noreturn]] void NOSAD() {
	cout << -1 << '\n'; exit(0);
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N;
	for (int i = 0; i < N; i++) {
		cin >> A[i];
	}
	for (int i = 0; i < N; i++) {
		cin >> B[i];
	}

	priority_queue<pair<int, int>> pq;
	for (int i = 0; i < N; i++) {
		if (B[i] < A[i]) {
			NOSAD();
		}
		if (B[i] > A[i]) {
			pq.emplace(B[i], i);
		}
	}
	ll ans = 0;
	while (!pq.empty()) {
		int i = pq.top().second; pq.pop();
		assert(B[i] > A[i]);
		int x = (i+N-1) % N;
		int y = (i+1) % N;
		assert(B[i] >= B[x]);
		assert(B[i] >= B[y]);
		if (B[i] <= B[x] + B[y]) {
			// then we're screwed, as no one can move
			NOSAD();
		}
		if (A[i] >= B[x] + B[y]) {
			// we'll go to A[i]
			if ((B[i] - A[i]) % (B[x] + B[y]) != 0) {
				NOSAD();
			}
			ans += (B[i] - A[i]) / (B[x] + B[y]);
			B[i] = A[i];
			continue;
		} else {
			ans += B[i] / (B[x] + B[y]);
			B[i] %= B[x] + B[y];
			if (B[i] < A[i]) {
				NOSAD();
			}
			if (B[i] > A[i]) {
				pq.emplace(B[i], i);
			}
		}
	}
	cout << ans << '\n';
	return 0;
}
