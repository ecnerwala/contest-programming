#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;

	for(int case_num = 1; case_num <= T; case_num++) {
		string s; cin >> s;
		deque<char> q;
		for(char c : s) {
			if(q.empty() || c >= q.front()) q.push_front(c);
			else q.push_back(c);
		}
		cout << "Case #" << case_num << ": ";
		cout << string(q.begin(), q.end()) << '\n';
	}

	return 0;
}
