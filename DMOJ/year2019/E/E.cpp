#include<bits/stdc++.h>
using namespace std;

const int MAXN = 2.1e5;
int N;
int H[MAXN];
int A[MAXN];
int B[MAXN];

int cnt[MAXN];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N;
	for (int i = 0; i < N; i++) {
		cin >> H[i];
		cin >> A[i] >> B[i];
		A[i] --, B[i] --;
		cnt[A[i]]++;
		cnt[B[i]+1]--;
	}
	for (int i = 0; i <= N; i++) {
		cnt[i+1] += cnt[i];
	}
	bool good = bool(cnt[0]);
	for (int i = 0; i < N; i++) {
		if (!cnt[H[i]]) {
			good = false;
		}
	}
	cout << (good ? "yes" : "no") << '\n';

	return 0;
}
