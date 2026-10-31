#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int N; cin >> N;
	// you can attain 0..L-N
	int L; cin >> L; L -= N;

	vector<int> A; A.reserve(N+2);
	A.push_back(0);
	for (int i = 0; i < N; i++) {
		int a; cin >> a; a--;
		A.push_back(a-i);
	}
	A.push_back(L);

	vector<int> B; B.reserve(N+2);
	B.push_back(0);
	for (int i = 0; i < N; i++) {
		int a; cin >> a; a--;
		B.push_back(a-i);
	}
	B.push_back(L);

	N += 2;

	map<int, int> minA;
	map<int, int> maxA;
	map<int, int> minB;
	map<int, int> maxB;

	for (int i = 0; i < N; i++) {
		maxA[A[i]] = i;
		maxB[B[i]] = i;
	}
	for (int i = N-1; i >= 0; i--) {
		minA[A[i]] = i;
		minB[B[i]] = i;
	}

	int64_t ans = 0;
	for (auto it : minB) {
		int v = it.first;
		if (minA.count(v)) {
			minB[v] = min(minB[v], minA[v]);
			maxB[v] = max(maxB[v], maxA[v]);
			ans += (maxB[v] - minB[v]) - (maxA[v] - minA[v]);
		} else {
			cout << -1 << '\n';
			exit(0);
		}
	}

	cout << ans << '\n';

	return 0;
}
