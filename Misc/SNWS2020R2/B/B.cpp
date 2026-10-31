#include<bits/stdc++.h>
using namespace std;

const int MAXN = 1.1e5;
int N;
int H[MAXN];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N;
	for (int i = 0; i < N; i++) {
		int x, y; cin >> x >> y; x--, y--;
		H[x] = y;
	}
	stack<int> stk;
	stk.push(N);
	int ans = 0;
	for (int i = 0; i < N; i++) {
		int y = H[i];
		while (stk.top() < y) {
			ans ++;
			stk.pop();
		}
		stk.push(y);
	}
	cout << ans << '\n';

	return 0;
}
