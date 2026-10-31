#include<bits/stdc++.h>
using namespace std;

const int MAXN = 1.1e5;
int N;
pair<int, int> A[MAXN];
pair<int, int> B[MAXN];

int perm[MAXN];

bool go() {
	sort(A, A + N);
	sort(B, B + N);
	for (int i = 0; i < N; i++) {
		if (A[i].first > B[i].first) return false;
	}
	for (int i = 0; i+1 < N; i++) {
		if (A[i+1].first <= B[i].first) return true;
	}

	for (int i = 0; i < N; i++) {
		perm[A[i].second] = B[i].second;
	}

	for (int i = 0, cur = 0; i < N; i++) {
		if (perm[cur] == -1) return true;
		int nxt = perm[cur];
		perm[cur] = -1;
		cur = nxt;
	}
	return false;
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N;
	for (int i = 0; i < N; i++) {
		cin >> A[i].first; A[i].second = i;
	}
	for (int i = 0; i < N; i++) {
		cin >> B[i].first; B[i].second = i;
	}
	cout << (go() ? "Yes" : "No") << '\n';

	return 0;
}
