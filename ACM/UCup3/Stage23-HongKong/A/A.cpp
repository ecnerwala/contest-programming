#include <bits/stdc++.h>
using namespace std;

using bs = bitset<200000>;

int M = 1010;
int main(){
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int N, K;
	cin >> N >> K;
	vector<int> A(N);
	for(int i = 0; i < N; i++){
		cin >> A[i];
		A[i]--;
	}
	vector<vector<int> > positions(M);
	for(int i = 0; i < N; i++){
		positions[A[i]].push_back(i);
	}
	bs cur;
	// start with 0
	for(int r = 0; r <= K; r++){
		for(int x : positions[r]) cur[x] = 1;
	}
	vector<bs> near(M);
	for(int r = 0; r < M; r++){
		near[r] = cur;
		if(r + K + 1 < M){
			for(int x : positions[r + K + 1]) cur[x] = 1;
		}
		if(r - K >= 0){
			for(int x : positions[r - K]) cur[x] = 0;
		}
	}
	int BLOCK = 200;
	vector<int> ans(2*N-1, N);
	bs f;
	for(int b = 0; b < N; b += BLOCK){
		int R = min(N, b + BLOCK);
		bs combined;
		for(int j = b; j < R; j++){
			combined |= (near[A[j]] & f) << j;
			f[j] = 1;
		}
		for(int x = 0; x < 2*N-1; x++){
			if(combined[x] && ans[x] == N){
				for(int j = b; j < R; j++){
					if(x >= j && near[A[j]][x-j]){
						ans[x] = j;
						break;
					}
				}
			}
		}
	}
	for(int i = 0; i < 2*N-1; i++){
		ans[i] = min(ans[i], N);
		ans[i] = min(ans[i], i - (-1));
	}
	for(int i = 0; i < 2*N-1; i++){
		int x = ans[i];
		cout << x << ' ';
	}
	cout << '\n';
}
