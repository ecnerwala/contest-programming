#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int H, W; cin >> H >> W;
	int N; cin >> N;
	vector<int> res;
	for (int i = 0; i < N; i++) {
		int a; cin >> a;
		while (a--) {
			res.push_back(i+1);
		}
	}
	for (int i = 0; i < H; i++) {
		vector<int> v(res.begin() + i * W, res.begin() + (i+1) * W);
		if (i & 1) reverse(v.begin(), v.end());
		for (int j = 0; j < W; j++) {
			cout << v[j] << " \n"[j+1==W];
		}
	}

	return 0;
}
