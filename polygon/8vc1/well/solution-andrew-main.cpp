#include<bits/stdc++.h>
using namespace std;

int main() {
	int L, A, B; cin >> L >> A >> B;
	if(L <= A) cout << 1 << '\n';
	else cout << 1 + (L - A + (A - B) - 1) / (A - B) << '\n';
}
