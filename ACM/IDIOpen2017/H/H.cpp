#include <bits/stdc++.h>

#define I_LOVE_KEKLUX main

int I_LOVE_KEKLUX() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int B, BR, BS, A, AS; cin >> B >> BR >> BS >> A >> AS;

	cout << A + ((BR - B) * BS + AS) / AS << '\n';

	return 0;
}
