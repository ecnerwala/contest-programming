#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);





	string N; cin >> N;

	if (N.size() <= 3) cout << N << '\n';
	else {

		cout << N[0] << N[1] << N[2];
		for (int i = 3; i < N.size(); i++) cout << 0;
		cout << '\n';
	}




	return 0;


}
