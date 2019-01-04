#include<bits/stdc++.h>
using namespace std;

const int MAXN = 5e5;
int N;
int A[MAXN];

bool isGood(int D) {
	assert(D > 0);
	int L = 0;
	vector<pair<int, int>> cur;
	for (int i = 0; i < N; i++) {
		int a = A[i];
		if (a > L) {
			cur.emplace_back(a-L, 0);
			L = a;
		} else {
			// first we pop cur all the way down
			while (L > a) {
				int d = min(cur.back().first, L-a);
				cur.back().first -= d;
				if (cur.back().first == 0) cur.pop_back();
				L -= d;
			}

			assert(a == L);
			while (!cur.empty() && cur.back().second == D-1) {
				L -= cur.back().first;
				cur.pop_back();
			}
			if (cur.empty()) {
				return false;
			}
			assert(cur.back().second < D-1);
			if (cur.back().first > 1) {
				cur.back().first --;
				cur.emplace_back(1, cur.back().second);
			}
			assert(cur.back().first == 1);
			assert(cur.back().second < D-1);
			cur.back().second++;
			if (a > L) {
				cur.emplace_back(a-L, 0);
				L = a;
			}
		}
	}

	return true;
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N;
	for (int i = 0; i < N; i++) cin >> A[i];
	int mi = 0, ma = N+1;
	while (ma - mi > 1) {
		int md = (mi + ma) / 2;
		if (isGood(md)) {
			ma = md;
		} else {
			mi = md;
		}
	}
	cout << ma << '\n';

	return 0;
}
