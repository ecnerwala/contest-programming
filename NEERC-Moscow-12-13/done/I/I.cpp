#include<bits/stdc++.h>
using namespace std;

inline long long ceil(long long a, long long b) { return (a + b - 1) / b; }

int main() {
	long long a, b, c, d;
	cin >> a >> b >> c >> d;
	long long res = (a / c) * (b / d);
	if(a % c && b % d) {
		res += min(
				ceil(a / c, d / (b % d)) + ceil(b / d + 1, c / (a % c)),
				ceil(a / c + 1, d / (b % d)) + ceil(b / d, c / (a % c))
				);
	} else if(a % c) {
		res += ceil(b / d, c / (a % c));
	} else if(b % d) {
		res += ceil(a / c, d / (b % d));
	}
	cout << res << '\n';

	return 0;
}
