#include <bits/stdc++.h>
using namespace std;

int main() {

	int N; cin >> N;
	std::vector<std::string> S(N);
	std::vector<int> A(N);

	for (int i = 0; i < N; i++) {
		cin >> S[i] >> A[i];
	}


	int x  =int(std::min_element(A.begin(), A.end()) - A.begin());
	for (int i = 0; i < N; i++) {

		cout << S[(i+x)%N] << '\n';
	}
}
