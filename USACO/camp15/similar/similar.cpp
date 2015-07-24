#include<bits/stdc++.h>
using namespace std;

int main() {
	multiset<string> st;
	int N; cin >> N;
	for(int i = 0; i < N; i++) {
		string op; cin >> op;
		if(op == "BUY") {
			string id; cin >> id;
			st.insert(id);
		} else if(op == "SELL") {
			string l, r; cin >> l >> r;
			st.erase(st.lower_bound(l), st.upper_bound(r));
		} else assert(false);
	}

	for(string s : st) {
		cout << s << '\n';
	}
}
