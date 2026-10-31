#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, K; cin >> N >> K;
	std::vector<int> A(N);
	int L = 0;
	for (auto& a : A) { cin >> a; L += a; }

	if (*std::max_element(A.begin(), A.end()) > (L + K - 1) / K) {
		cout << -1 << '\n';
		exit(0);
	}

	return 0;
}

// If nothing used so far:
// You need <= L % K are == floor(L / K) + 1, and the rest <= floor(L / K)
// Proof: greedy algorithm, always take biggest remaining A_i which is valid
//
// If 
