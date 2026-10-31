#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int H, W, A, B; cin >> H >> W >> A >> B;
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			cout << ((i < B) ^ (j < A));
		}
		cout << '\n';
	}

	return 0;
}
