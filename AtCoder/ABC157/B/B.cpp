#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int A[3][3];
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			cin >> A[i][j];
		}
	}
	bool filled[3][3];
	memset(filled, 0, sizeof(filled));

	int N; cin >> N;
	while (N--) {
		int v; cin >> v;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				if (A[i][j] == v) filled[i][j] = true;
			}
		}
	}
	if (
			(filled[0][0] && filled[1][1] && filled[2][2]) ||
			(filled[0][0] && filled[0][1] && filled[0][2]) ||
			(filled[1][0] && filled[1][1] && filled[1][2]) ||
			(filled[2][0] && filled[2][1] && filled[2][2]) ||
			(filled[0][0] && filled[1][0] && filled[2][0]) ||
			(filled[0][1] && filled[1][1] && filled[2][1]) ||
			(filled[0][2] && filled[1][2] && filled[2][2]) ||
			(filled[0][2] && filled[1][1] && filled[2][0])
	   ) {
		cout << "Yes" << '\n';
	} else {
		cout << "No" << '\n';
	}

	return 0;
}
