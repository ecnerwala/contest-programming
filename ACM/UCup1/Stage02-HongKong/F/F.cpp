#include <bits/stdc++.h>
using namespace std;

vector<int> add(vector<int> a, vector<int> b){
	a.resize(max(a.size(), b.size()), 0);
	for(int i = 0; i < b.size(); i++){
		a[i] += b[i];
		while(a[i] >= 10){
			a[i] -= 10;
			if(i + 1 == a.size()) a.push_back(0);
			a[i+1] += 1;
		}
	}
	return a;
}

vector<int> smaller(vector<int> a, vector<int> b){
	if(a.size() < b.size()) return a;
	if(a.size() > b.size()) return b;
	for(int i = (int)a.size() - 1; i >= 0; i--){
		if(a[i] < b[i]) return a;
		if(a[i] > b[i]) return b;
	}
	return a;
}

void solve(){
	int N, K;
	cin >> N >> K;
	string _S;
	cin >> _S;
	vector<int> S;
	for(char c : _S) S.push_back(c - '0');
	reverse(S.begin(), S.end());

	int summands = K + 1;
	int max_digits = (N + summands - 1) / summands + 1;
	vector<int> minv(summands + 1);
	vector<int> maxv(summands + 1);
	for(int i = 0; i <= summands; i++){
		minv[i] = N - max_digits * (summands - i);
		maxv[i] = max_digits * i;
		minv[i] = min(max(minv[i], 0), N);
		maxv[i] = min(max(maxv[i], 0), N);
	}
	minv[0] = maxv[0];
	maxv[summands] = minv[summands];
	map<int, vector<int>> dp;
	dp[0] = {};
	for(int i = 1; i <= summands; i++){
		map<int, vector<int> > ndp;
		for(int v = minv[i]; v <= maxv[i]; v++){
			vector<int> best;
			for(auto [loc, num] : dp){
				if(v <= loc) continue;
				if(v - loc > max_digits) continue;
				vector<int> new_num = add(num, vector<int>(S.begin() + loc, S.begin() + v));
				if(best.empty()){
					best = new_num;
				} else {
					best = smaller(best, new_num);
				}
			}
			if(!best.empty()) ndp[v] = best;
		}
		dp = ndp;
	}
	vector<int> ans = dp[maxv[summands]];
	reverse(ans.begin(), ans.end());
	for(int x : ans) cout << x;
	cout << '\n';
}
int main(){
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int T;
	cin >> T;
	while(T--) solve();
}