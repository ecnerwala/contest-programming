#include <bits/stdc++.h>
using namespace std;

int main() {
	int N, K; cin >> N >> K;
	string S; cin >> S;
	vector<int> black; black.reserve(N);
	for (int i = 0; i < N; i++)  if (S[i] == 'B') black.push_back(i);
	int ans = 0;
	for (int i = 0; i <= K; i++) {
		int j = K-i;
		ans = max(ans, *(black.begin()+i) + N-1 - *(black.end()-1-j)- K);
	}
	cout << ans << '\n';
}
