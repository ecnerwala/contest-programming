#include<bits/stdc++.h>
using namespace std;

const int MAXN = 200;
int N, K, M, A;
pair<int, int> cnt[MAXN];
int main() {
	ios_base::sync_with_stdio(0);
	cin >> N >> K >> M >> A;
	for (int i = 0; i < A; i++) {
		cnt[i].first++;
		cnt[i].second = -i;
	}
	for (int i = 0; i < N; i++) {
		bool best, worst;
		{
			pair<int, int> c = cnt[i];
			if (M-A) {
				c.first += M-A;
				c.second = M;
			}
			int num_lose = 0;
			for (int j = 0; j < N; j++) {
				if (j == i) continue;
				if (cnt[j] > c) {
					num_lose ++;
				}
			}
			best = (num_lose < K);
		}
		{
			vector<int> num_needed;
			for (int j = 0; j < N; j++) {
				if (j == i) continue;
				if (cnt[j].first && cnt[j] > cnt[i]) {
					num_needed.push_back(0);
				} else {
					num_needed.push_back(cnt[i].first - cnt[j].first + 1);
				}
			}
			worst = cnt[i].first;
			sort(num_needed.begin(), num_needed.end());
			for (int i = 0; i < K; i++) {
			}
		}
		if (worst) {
			cout << 1;
		} else if (best) {
			cout << 2;
		} else {
			cout << 3;
		}
		cout << ((i+1 == N) ? '\n' : ' ') << '\n';
	}

	return 0;
}
