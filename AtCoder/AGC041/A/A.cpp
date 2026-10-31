#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	long long N, A, B; cin >> N >> A >> B;
	assert(A != B);
	if (A > B) swap(A,B);
	assert(A < B);
	if (A % 2 == B % 2) {
		cout << (B-A) / 2 << '\n';
	} else {
		cout << min((N-B) + 1 + (N-A), (A-1) + 1 + (B-1)) / 2<< '\n';
	}

	return 0;
}
