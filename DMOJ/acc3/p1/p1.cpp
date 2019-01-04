#include<bits/stdc++.h>
using namespace std;

const int MAXN = 1.1e6;
const int MAXV = MAXN;
int N;
int K;

int V;
int A[MAXN];
int cnt[MAXV];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N >> K;
	V = N;
	for (int i = 0; i < N; i++) {
		cin >> A[i];
	}
	long long res = 0;
	int num = 0;
	for (int r = 0, l = 0; r < N; r++) {
		num += !(cnt[A[r]]++);
		while (num >= K) {
			num -= !(--cnt[A[l++]]);
		}
		res += l;
	}
	cout << res << '\n';

	return 0;
}
