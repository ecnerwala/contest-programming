#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int N; cin >> N;
	int cnt[3] = {0,0,0};
	for (int i = 0; i < N; i++) {
		int a; cin >> a; a--; cnt[a]++;
	}
	cout << N-max({cnt[0],cnt[1],cnt[2]}) << '\n';

	return 0;
}
