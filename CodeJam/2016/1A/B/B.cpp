#include<bits/stdc++.h>
using namespace std;

int cnt[4000];

int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;

	for(int case_num = 1; case_num <= T; case_num++) {
		int N; cin >> N;
		memset(cnt, 0, sizeof(cnt));
		for(int i = 0; i < (2 * N - 1) * N; i++) {
			int v; cin >> v;
			assert(0 <= v && v < 4000);
			cnt[v] ^= 1;
		}
		cout << "Case #" << case_num << ":";
		for(int i = 0; i < 4000; i++) if(cnt[i]) cout << ' ' << i;
		cout << '\n';
	}

	return 0;
}
