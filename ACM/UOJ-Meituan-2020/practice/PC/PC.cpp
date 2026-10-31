#include<bits/stdc++.h>

int SEED;

int get_rand() {
	const int MUL = int(1e8)+5;
	const int ADD = 20150609;
	const int MOD = 998244353;
	SEED = int((int64_t(SEED) * MUL + ADD) % MOD);
	return SEED / 100;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	cout << "Please enter your team's random seed:" << endl;
	cin >> SEED;

	int A = SEED;
	for (int z = 0; z < 1000; z++) {
		get_rand();
	}
	int B = get_rand();
	ofstream out("large.ans");
	out << A << ' ' << B << endl;
	return 0;
}
