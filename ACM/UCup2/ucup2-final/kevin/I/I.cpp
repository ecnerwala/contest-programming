#include <bits/stdc++.h>
using namespace std;

using ll = int64_t;

void solve(){
	ll N, M;
	cin >> N >> M;
	string S;
	cin >> S;

	ll scoreA = 0;
	ll scoreB = 0;
	{
		ll cA = 0, cB = 0;
		for(char c : S) if(c == 'A') cA++; else cB++;
		ll x = max(cA, cB);
		ll full = max(ll(0), M / x - 1);
		scoreA += full * cA;
		scoreB += full * cB;
	}
	if(max(scoreA, scoreB) >= M && abs(scoreA - scoreB) >= 2){
		cout << "Yes" << '\n';
		cout << (scoreA + scoreB) << '\n';
		return;
	}
	for(int i = 0; i < 10 * N; i++){
		if(S[i % N] == 'A'){
			scoreA++;
		} else {
			scoreB++;
		}
		if(max(scoreA, scoreB) >= M && abs(scoreA - scoreB) >= 2){
			cout << "Yes" << '\n';
			cout << (scoreA + scoreB) << '\n';
			return;
		}
	}
	cout << "No" << '\n';
}

int main(){
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int T;
	cin >> T;
	while(T--) solve();
}