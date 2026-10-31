#include <bits/stdc++.h>
using namespace std;

void solve(){
	int N;
	cin >> N;
	vector<string> S(2);
	cin >> S[0] >> S[1];
	vector<vector<int>> pos(3, vector<int>(N, -2));
	bool bad = false;
	auto chk = [&](int i, int cur, int val){
		if(pos[i][cur] == -2){
			pos[i][cur] = val;
		} else {
			bad = true;
		}
	};
	for(int i = 0; i < N; i++){
		if(S[0][2*i] != '?') chk(S[0][2*i]-'0', i, 0);
		if(S[1][2*i] != '?') chk(S[1][2*i]-'0', i, 1);
		if(S[0][2*i+1] != '?') chk(S[0][2*i+1]-'0', i, 2);
		if(S[1][2*i+1] != '?') chk(S[1][2*i+1]-'0', i, 3);
	}
	auto score = [&](vector<vector<int>> grid) -> vector<int> {
		vector<int> res(2*N, 0);
		vector<int> x = grid[0];
		x.push_back(0);
		vector<int> stk;
		for(int c : x){
			if(c){
				stk.push_back(c);
			} else {
				reverse(stk.begin(), stk.end());
				for(int i = 0; i < stk.size(); i++){
					res[i] += stk[i];
				}
				stk.clear();
			}
		}
		for(int i = 0; i < 2*N-1; i++){
			res[i+1] += res[i] / 10;
			res[i] %= 10;
		}
		reverse(res.begin(), res.end());
		return res;
	};

	vector<vector<int> > best_grid;
	for(int st : {1, 2}){
		if(bad) continue;
		int l = st;
		int r = 3-st;

		int left_start = N;
		for(int i = 0; i < N; i++){
			if((pos[l][i] / 2) == 1){
				left_start = i;
				break;
			}
		}
		int right_end = 0;
		for(int i = N-1; i >= 0; i--){
			if((pos[r][i] / 2) == 0){
				right_end = i+1;
				break;
			}
		}
		if(left_start < right_end) continue;
		// right_end -> left_start can be middle
		int x = right_end;
		int y = left_start;
		bool ok = true;
		for(int i = 0; i < x; i++) if(pos[l][i] / 2 == 1 || pos[r][i] / 2 == 1) ok = false;
		for(int i = x; i < y; i++) if(pos[l][i] / 2 == 1 || pos[r][i] / 2 == 0) ok = false;
		for(int i = y; i < N; i++) if(pos[l][i] / 2 == 0 || pos[r][i] / 2 == 0) ok = false;
		if(!ok) continue;
		vector<vector<int> > vals(2, vector<int>(2*N, 0));
		for(int i = 0; i < x; i++){
			int top = 2;
			if(pos[1][i] == 0 || pos[2][i] == 1) top = 1;
			vals[0][2*i] = top;
			vals[1][2*i] = 3-top;
		}
		for(int i = x; i < y; i++){
			int loc = 0;
			if(pos[l][i] == 1) loc = 1;
			vals[loc][2*i] = l;
		}
		for(int i = x; i < y; i++){
			int loc = 0;
			if(pos[r][i] == 3) loc = 1;
			vals[loc][2*i+1] = r;
		}
		for(int i = y; i < N; i++){
			int top = 2;
			if(pos[1][i] == 2 || pos[2][i] == 3) top = 1;
			vals[0][2*i+1] = top;
			vals[1][2*i+1] = 3-top;
		}
		if(best_grid.empty() || score(vals) >= score(best_grid)){
			best_grid = vals;
		}
	}
	if(best_grid.empty()){
		cout << "impossible" << '\n';
		return;
	}
	for(int i = 0; i < 2; i++){
		for(int j = 0; j < 2*N; j++){
			if(S[i][j] != '?' && S[i][j] != ('0' + best_grid[i][j])) assert(false);
		}
	}
	vector<int> ff = score(best_grid);
	reverse(ff.begin(), ff.end());
	while(!ff.empty() && ff.back() == 0) ff.pop_back();
	if(ff.empty()) ff.push_back(0);
	reverse(ff.begin(), ff.end());
	for(int x : ff) cout << x;
	cout << '\n';
	for(auto l : best_grid){
		for(auto c : l){
			cout << c;
		}
		cout << '\n';
	}
}

int main(){
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int T;
	cin >> T;
	while(T--) solve();
}