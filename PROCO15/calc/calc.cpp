#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0);
	int n; cin >> n;
	vector<int> st;
	st.push_back(0);
	for(int i = 0; i < n; i++) {
		char c; cin >> c;
		if(c == '+') {
			st.push_back(st.back() + 1);
		} else if(c == '-') {
			st.push_back(st.back() - 1);
		} else if(c == 'z') {
			int a; cin >> a;
			assert(a < st.size());
			st.push_back(st[st.size() - 1 - a]);
		} else assert(false);
	}
	cout << st.back() << '\n';
	return 0;
}
