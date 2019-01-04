#include<bits/stdc++.h>
using namespace std;

int H, W;
char A[200][200];
bool take[200];
int main() {
	ios_base::sync_with_stdio(0);
	cin >> H >> W;
	for (int i = 0; i < H; i++) {
		cin >> A[i];
		for (int j = 0; j < W; j++) {
			if (A[i][j] == '#') take[j] = true;
		}
	}
	for (int i = 0; i < H; i++) {
		bool good = false;
		for (int j = 0; j < W; j++) {
			if (A[i][j] == '#') good = true;
		}
		if (!good) continue;
		for (int j = 0; j < W; j++) {
			if (take[j]) cout << A[i][j];
		}
		cout << '\n';
	}

	return 0;
}
