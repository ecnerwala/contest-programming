#include <bits/stdc++.h>
using namespace std;

vector<pair<int,int>> solve(vector<vector<int> > A){
	int N = (int)A.size();
	auto pr = [&](){
		for(int i = 0; i < N; i++){
			for(int j = 0; j < N; j++){
				cerr << A[i][j];
			}
			cerr << '\n';
		}
		cerr << '\n';
	};
	vector<pair<int,int> > ops;
	auto do_row = [&](int i){
		ops.push_back({0, i});
		rotate(A[i].begin(), A[i].begin() + 1, A[i].end());
		// pr();
	};
	auto do_col = [&](int i){
		ops.push_back({1, i});
		int v = 1 ^ A[N-1][i];
		for(int j = N-1; j > 0; j--){
			A[j][i] = A[j-1][i];
		}
		A[0][i] = v;
		// pr();
	};
	int cnt = 0;
	while(true){
		bool did = false;
		for(int r = 0; r < N; r++){
			if(A[r][0] == 1) continue;
			if(A[r] == vector<int>(N, 0)) continue;
			did = true;
			while(A[r][0] == 0) do_row(r);
		}
		cnt++;
		do_col(0);
		if(did) cnt = 0;
		if(cnt >= 2*N) break;
	}
	for(int i = 0; i < N; i++){
		for(int j = 1; j < N; j++){
			assert(A[i][j] == 0);
		}
	}
	for(int i = 0; i < N; i++){
		if(A[N-1][0] == 1){
			do_row(N-1);
			do_col(N-1);
		}
		do_col(0);
	}
	for(int i = 0; i < N; i++){
		do_col(0);
	}
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
			assert(A[i][j] == 0);
		}
	}
	return ops;
}

int main(){
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int N;
	cin >> N;
	vector<vector<int> > A(N, vector<int>(N));
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
			char c;
			cin >> c;
			A[i][j] = c - '0';
		}
	}
	vector<vector<int> > B(N, vector<int>(N));
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
			char c;
			cin >> c;
			B[i][j] = c - '0';
		}
	}
	auto rA = solve(A);
	vector<vector<int> > fB(N, vector<int>(N));
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
			fB[N-1-i][N-1-j] = B[i][j];
		}
	}
	auto rB = solve(fB);
	reverse(rB.begin(), rB.end());
	for(auto& [type, row] : rB){
		row = N-1-row;
	}
	vector<pair<int,int> > ops = rA;
	ops.insert(ops.end(), rB.begin(), rB.end());
	cout << ops.size() << '\n';
	for(auto [type, row] : ops){
		cout << (vector<string>{"row", "column"}[type]) << ' ' << (row+1) << '\n';
	}
}