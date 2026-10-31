#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<int> A(N);
	for (auto& a : A) { cin >> a; a--; } 

	std::vector<int> freq(N);
	for (auto a : A) freq[a]++;

	std::string S(N, '0');
	std::vector<bool> alive(N);
	for (int i = 0; i < N; i++) {
	}

	return 0;
}
