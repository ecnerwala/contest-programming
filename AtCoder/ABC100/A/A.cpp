#include<bits/stdc++.h>
using namespace std;

bool good(int a, int b) {
	return a + b <= 16 && a <= 8 && b <= 8;
}

int main() {
	int A, B; cin >> A >> B;
	cout << (good(A, B) ? "Yay!" : ":(") << '\n';
	return 0;
}
