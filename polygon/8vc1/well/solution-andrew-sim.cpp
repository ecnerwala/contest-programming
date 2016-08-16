#include<bits/stdc++.h>
using namespace std;

int main() {
	int L, A, B; cin >> L >> A >> B;
	int pos = 0;
	for(int t = 1; true; t++) {
		pos += A;
		if(pos >= L) {
			cout << t << '\n';
			return 0;
		}
		pos -= B;
	}
}
