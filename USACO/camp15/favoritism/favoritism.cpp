#include<bits/stdc++.h>

using namespace std;

unordered_map<string, vector<string>> mp;

int main() {
	int Q; cin >> Q;
	for(int i = 0; i < Q; i++) {
		string op; cin >> op;
		if(op == "BUY") {
			string id, type; cin >> id >> type;
			mp[type].push_back(id);
		} else if(op == "SELL") {
			string type; cin >> type;
			mp[type].pop_back();
		} else if(op == "QUERY") {
			int i; string type; cin >> i >> type;
			cout << mp[type][i - 1] << '\n';
		}
	}
}
