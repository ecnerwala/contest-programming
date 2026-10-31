#include<bits/stdc++.h>
using namespace std;

const int MAXN = 120;
const int MAXM = 120;
const int MAXH = 120;
int N, M, H;
vector<int> A[MAXN];
vector<int> B[MAXN];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N >> M >> H;
	for (int x = 0; x < N; x++) {
		string s; cin >> s;
		for (int y = 0; y < M; y++) {
			if (s[y] == '1') {
				A[x].push_back(y);
			}
		}
	}
	for (int x = 0; x < N; x++) {
		string s; cin >> s;
		for (int z = 0; z < H; z++) {
			if (s[z] == '1') {
				B[x].push_back(z);
			}
		}
	}

	for (int x = 0; x < N; x++) {
		if (A[x].empty() != B[x].empty()) {
			cout << -1 << '\n';
			exit(0);
		}
	}

	int kmax = 0;
	for (int x = 0; x < N; x++) {
		kmax += int(A[x].size()) * int(B[x].size());
	}
	cout << kmax << '\n';
	for (int x = 0; x < N; x++) {
		if (A[x].empty()) continue;
		for (int y : A[x]) {
			for (int z : B[x]) {
				cout << x << ' ' << y << ' ' << z << '\n';
			}
		}
	}

	int kmin = 0;
	for (int x = 0; x < N; x++) {
		kmin += max(int(A[x].size()), int(B[x].size()));
	}
	cout << kmin << '\n';
	for (int x = 0; x < N; x++) {
		if (A[x].empty()) continue;
		reverse(A[x].begin(), A[x].end());
		reverse(B[x].begin(), B[x].end());
		while (A[x].size() < B[x].size()) A[x].push_back(A[x].back());
		while (B[x].size() < A[x].size()) B[x].push_back(B[x].back());
		reverse(A[x].begin(), A[x].end());
		reverse(B[x].begin(), B[x].end());
		for (int i = 0; i < int(A[x].size()); i++) {
			cout << x << ' ' << A[x][i] << ' ' << B[x][i] << '\n';
		}
	}

	return 0;
}
