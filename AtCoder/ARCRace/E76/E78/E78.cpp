#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int R, C; cin >> R >> C;
	int N; cin >> N;
	map<int, int> pts;
	auto toSeg = [&](int x, int y) -> int {
		if (x == 0) return y;
		else if (y == C) return C + x;
		else if (x == R) return C + R + (C - y);
		else if (y == 0) return C + R + C + (R - x);
		else return -1;
	};
	for (int i = 1; i <= N; i++) {
		int x1, y1, x2, y2; cin >> x1 >> y1 >> x2 >> y2;
		int v1 = toSeg(x1, y1);
		int v2 = toSeg(x2, y2);
		if (v1 == -1 || v2 == -1) continue;
		if (v1 > v2) swap(v1, v2);
		pts[v1] = +i;
		pts[v2] = -i;
	}
	stack<int> st;
	for (auto it : pts) {
		if (it.second > 0) {
			st.push(it.second);
		} else {
			if (st.empty() || st.top() != -it.second) {
				cout << "NO" << '\n';
				exit(0);
			}
			st.pop();
		}
	}
	cout << "YES" << '\n';

	return 0;
}
