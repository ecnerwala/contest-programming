#include<bits/stdc++.h>
using namespace std;

typedef long long ll;
const ll INF = 1e9 + 200;

const int MAXN = 1e3 + 200;
int N;
ll A[MAXN][MAXN];
ll R[MAXN];

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N;
	for (int i = 0; i < N; i++) {
		R[i] = 0;
		for (int j = 0; j < N; j++) {
			cin >> A[i][j];
			R[i] += A[i][j];
		}
	}
	ll sum = 1e18;
	for (int z = 0; z < 2; z++) {
		ll dsum = 0;
		ll dcnt = 0;
		for (int i = 0; i < N; i++) {
			ll cur = A[i][z ? (N-1-i) : i];
			if (cur == 0) {
				dsum -= R[i];
				dcnt ++;
			} else {
				dsum += cur;
			}
		}
		if (dcnt == 1) continue;
		sum = dsum / (1 - dcnt);
		break;
	}
	for (int i = 0; i < N; i++) {
		assert(R[i] < sum);
		for (int j = 0; j < N; j++) {
			if (A[i][j] == 0) A[i][j] = sum - R[i];
		}
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (j) cout << ' ';
			cout << A[i][j];
		}
		cout << '\n';
	}
	return 0;
}
