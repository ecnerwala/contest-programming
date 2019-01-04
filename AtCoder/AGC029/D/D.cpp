#include<bits/stdc++.h>
using namespace std;

const int MAXH = 5e5;
const int MAXW = 5e5;
const int MAXN = 5e5;
int H, W;
int N;
vector<int> obstacle[MAXW];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> H >> W >> N;
	for (int i = 0; i < N; i++) {
		int x, y; cin >> x >> y;
		obstacle[y].push_back(x);
	}
	for (int j = 1; j <= W; j++) {
		sort(obstacle[j].begin(), obstacle[j].end());
		obstacle[j].push_back(H+1);
	}
	int firstBad = H+1;
	int firstReach = 0;
	for (int j = 1; j <= W; j++) {
		firstReach++;
		int ind = 0;
		while (ind < int(obstacle[j].size()) && obstacle[j][size_t(ind)] <= firstReach) {
			firstReach = max(firstReach, obstacle[j][size_t(ind)]+1);
			ind++;
		}
		if (ind == int(obstacle[j].size())) {
			break;
		}
		firstBad = min(firstBad, obstacle[j][size_t(ind)]);
	}
	
	cout << firstBad-1 << '\n';

	return 0;
}
