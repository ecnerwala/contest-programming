#include<bits/stdc++.h>
using namespace std;

using ll = long long;

string A = "Good writing is good ";
string B = " writing is good ";
string C = " is good writing.";
string O = "Good writing is good writing is good writing.";
ll lens[40];

char query(int n, ll k) {
	if (k >= lens[n]) return '.';

	if (n == 0) {
		return O[k];
	}

	if (k < int(A.size())) return A[k];
	else k -= int(A.size());

	if (k < lens[n-1]) return query(n-1, k);
	else k -= lens[n-1];

	if (k < int(B.size())) return B[k];
	else k -= int(B.size());

	if (k < lens[n-1]) return query(n-1, k);
	else k -= lens[n-1];

	return C[k];
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);

	lens[0] = int(O.size());
	for (int i = 1; i <= 30; i++) {
		lens[i] = int(A.size()) + lens[i-1] + int(B.size()) + lens[i-1] + int(C.size());
	}
	int Q; cin >> Q;
	while (Q--) {
		int n; ll k; cin >> n >> k; k--;
		cout << query(n, k) << '\n';
	}

	return 0;
}
