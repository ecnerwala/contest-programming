#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		string A, B; cin >> A >> B;
		vector<int> F(B.size()+1);
		F[0] = -1;
		for (int i = 0; i < int(B.size()); i++) {
			int cur = F[i];
			while (cur != -1 && B[cur] != B[i]) cur = F[cur];
			F[i+1] = cur+1;
		}
		vector<int> resp; resp.reserve(A.size());
		int cur = 0;
		for (int i = 0; i < int(A.size()); i++) {
			while (cur != -1 && B[cur] != A[i]) cur = F[cur];
			cur++;
			if (cur == int(B.size())) {
				resp.push_back((i+1) - int(B.size()));
			}
		}
		for (int z = 0; z < int(resp.size()); z++) {
			cout << resp[z]+1 << " \n"[z+1==int(resp.size())];
		}
	}

	return 0;
}
