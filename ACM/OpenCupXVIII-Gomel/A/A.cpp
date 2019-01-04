#include<bits/stdc++.h>
using namespace std;
// arith mean = sum(d) / num(d)
// harmonic mean = 1 / (sum(d) / n / num(d))
// harmonic mean * arith_mean = n

int main() {
	ios_base::sync_with_stdio(0); cin.tie(0), cout.tie(0);
	long long A, H;
	cin >> A >> H;
	cout << A * H << '\n';

	return 0;
}
