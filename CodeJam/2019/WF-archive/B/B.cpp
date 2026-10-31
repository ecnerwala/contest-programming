#include<bits/stdc++.h>
using namespace std;

int P, S, K, N;
void go() {
	cin >> P >> S >> K >> N;
	vector<vector<int>> perms;
	const int CYC = int(perms.size());
	perms.emplace_back(N);
	for (int i = 0; i < N; i++) {
		perms.back()[i] = (i+1)%N;
	}
	const int SWAP = int(perms.size());
	perms.emplace_back(N);
	for (int i = 0; i < N; i++) {
		perms.back()[i] = (i < 2 ? (i ^ 1) : i);
	}

	int ROT1[8];
	for (int d = 1; d < 7; d++) {
		ROT1[d] = int(perms.size());
		perms.emplace_back(N);
		for (int i = 0; i < N; i++) {
			// all but 0
			if (i == 0) perms.back()[i] = 0;
			else perms.back()[i] = (i-1 + d) % (N-1) + 1;
		}
	}
	int ROT7[8];
	for (int d = 1; d < 7; d++) {
		ROT7[d] = int(perms.size());
		perms.emplace_back(N);
		for (int i = 0; i < N; i++) {
			// all but 0
			if (i == 0) perms.back()[i] = 0;
			else perms.back()[i] = (i-1 + d*7) % (N-1) + 1;
		}
	}

	cout << perms.size() << '\n';
	for (const auto& v : perms) {
		for (int i = 0; i < N; i++) {
			cout << v[i]+1 << " \n"[i+1==N];
		}
	}

	for (int k = 0; k < K; k++) {
		vector<pair<int, int>> vals(N);
		for (int i = 0; i < N; i++) {
			cin >> vals[i].first;
			vals[i].second = i;
		}
		sort(vals.begin(), vals.end());
		vector<int> A(N);
		for (int i = 0; i < N; i++) {
			A[vals[i].second] = i;
		}

		vector<int> ops;
		auto do_op = [&](int ind) {
			ops.push_back(ind);
			vector<int> B(N);
			for (int i = 0; i < N; i++) {
				B[i] = A[perms[ind][i]];
			}
			A = B;
		};

		for (int i = 0; i < N; i++) {
			int j = 0;
			while (A[j] != i) j++;
			if (j != 0) {
				int d = j-1;
				if (d / 7) {
					do_op(ROT7[d / 7]);
				}
				if (d % 7) {
					do_op(ROT1[d % 7]);
				}
				do_op(SWAP);
				d = ((N-1) - d) % (N-1);
				if (d / 7) {
					do_op(ROT7[d / 7]);
				}
				if (d % 7) {
					do_op(ROT1[d % 7]);
				}
			}
			do_op(CYC);
		}

		//for (int i = 0; i < N; i++) cerr << A[i] << ' ';
		//cerr << '\n';
		cout << ops.size();
		for (int op : ops) {
			cout << ' ' << op+1;
		}
		cout << '\n';
	}
}

int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;

	for (int case_num = 1; case_num <= T; case_num ++) {
		cout << "Case #" << case_num << ":\n";
		go();
	}

	return 0;
}
