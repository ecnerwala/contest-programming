#include<bits/stdc++.h>
using namespace std;

const int MAXN = 3000;
int N;
int F[MAXN];
int indeg[MAXN];
int cnt[MAXN];

int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;

	for(int case_num = 1; case_num <= T; case_num++) {
		cin >> N;
		assert(3 <= N && N < MAXN);
		for(int i = 0; i < N; i++) {
			cin >> F[i]; F[i] --;
			assert(0 <= F[i] && F[i] < N);
			assert(i != F[i]);
		}

		int maxC = 0;
		int tot2 = 0;

		for(int i = 0; i < N; i++) indeg[i] = 0;
		for(int i = 0; i < N; i++) indeg[F[i]] ++;

		for(int i = 0; i < N; i++) cnt[i] = 1;

		queue<int> q;
		for(int i = 0; i < N; i++) if(!indeg[i]) q.push(i);

		while(!q.empty()) {
			int i = q.front(); q.pop();
			if(!(--indeg[F[i]])) q.push(F[i]);
			cnt[F[i]] = max(cnt[F[i]], cnt[i] + 1);
		}

		for(int i = 0; i < N; i++) {
			if(F[i] > i && F[F[i]] == i) {
				tot2 += cnt[i] + cnt[F[i]];
			}
		}

		for(int i = 0; i < N; i++) assert(indeg[i] <= 1);
		for(int i = 0; i < N; i++) {
			int len = 0;
			for(int j = i; indeg[j]; j = F[j]) {
				assert(indeg[j] == 1);
				indeg[j] --;
				assert(!indeg[j]);
				len++;
			}
			maxC = max(maxC, len);
		}

		int res = max(maxC, tot2);
		cout << "Case #" << case_num << ": " << res << '\n';
	}

	return 0;
}
