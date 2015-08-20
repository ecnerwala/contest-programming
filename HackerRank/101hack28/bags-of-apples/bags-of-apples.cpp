#include<bits/stdc++.h>
using namespace std;

vector<int> v[3];
int main() {
	ios_base::sync_with_stdio(0);
	int N;
	cin >> N;
	int tot = 0;
	for(int i = 0; i < N; i++) {
		int val; cin >> val;
		tot += val;
		v[val%3].push_back(val);
	}
	sort(v[1].begin(), v[1].end());
	sort(v[2].begin(), v[2].end());
	cerr << tot << '\n';

	if(tot % 3 == 0) {
		cout << tot << '\n';
	} else {
		int res = 0;
		if(v[tot % 3].size() >= 1) {
			res = max(res, tot - v[tot % 3][0]);
		}
		if(v[3 - tot % 3].size() >= 2) {
			res = max(res, tot - v[3 - tot % 3][0] - v[3 - tot % 3][1]);
		}
		assert(res % 3 == 0);
		cout << res << '\n';
	}
	return 0;
}
