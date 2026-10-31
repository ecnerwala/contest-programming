#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int D1, V1, D2, V2, P; cin >> D1 >> V1 >> D2 >> V2 >> P;
	D1--, D2--;
	if (D1 > D2) swap(D1, D2), swap(V1, V2);
	assert(D1 <= D2);
	if (P <= (D2 - D1) * V1) {
		cout << D1 + (P + V1 - 1) / V1 << '\n';
	} else {
		P -= (D2 - D1) * V1;
		cout << D2 + (P + V1 + V2 - 1) / (V1 + V2) << '\n';
	}

	return 0;
}
