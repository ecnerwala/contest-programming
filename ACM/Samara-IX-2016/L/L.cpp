#include<bits/stdc++.h>
using namespace std;

const int MAXN = 2.1e5;
int N;
bool canWin(vector<int> A, vector<int> B) {
	sort(A.begin(), A.end());
	sort(B.begin(), B.end());
	int nA = 0;
	while (!A.empty() && !B.empty()) {
		if (A.back() > B.back()) {
			nA ++;
			A.pop_back(), B.pop_back();
		} else {
			B.pop_back();
		}
	}
	return nA > (N - nA);
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N;
	vector<int> A(N);
	vector<int> B(N);
	for (int i = 0; i < N; i++) cin >> A[i];
	for (int i = 0; i < N; i++) cin >> B[i];
	bool first = canWin(A, B);
	bool second = canWin(B, A);
	if (first && second) {
		cout << "Both" << '\n';
	} else if (first) {
		cout << "First" << '\n';
	} else if (second) {
		cout << "Second" << '\n';
	} else {
		cout << "None" << '\n';
	}

	return 0;
}
