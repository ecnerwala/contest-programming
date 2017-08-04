#include<bits/stdc++.h>
using namespace std;

int N;
string G[60];
vector<int> rows[60];
vector<int> cols[60];
bool go() {
	for (int i = 0; i < N; i++) {
		if (rows[i].size() > 2) return false;
		if (cols[i].size() > 2) return false;
	}
}

int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;

	for(int case_num = 1; case_num <= T; case_num ++) {
		
		cout << "Case #" << case_num << ": ";
	}

	return 0;
}
