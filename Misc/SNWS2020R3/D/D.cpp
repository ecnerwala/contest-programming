#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N; cin >> N;
	stack<int> st;
	for (int i = 0; i < N; i++) {
		int v; cin >> v;
		while (!st.empty() && st.top() <= v) st.pop();
		st.push(v);
		cout << st.size() << '\n';
	}

	return 0;
}
