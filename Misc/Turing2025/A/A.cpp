#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<int> A(N);
	for (auto& a : A) cin >> a;
	std::vector<int> B(N);
	for (auto& b : B) cin >> b;

	std::vector<int> P(N);
	P[0] = -1;
	for (int i = 1; i < N; i++) {
		cin >> P[i]; P[i]--;
	}


	return 0;
}
