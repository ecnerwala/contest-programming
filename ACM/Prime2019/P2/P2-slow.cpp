#include<bits/stdc++.h>
using namespace std;

int main() {
    string S; cin >> S;
    int N = int(S.size());

    vector<string> substrings;
    for (int i = 0; i < N; i++) {
        for (int j = i+1; j <= N; j++) {
            substrings.push_back(S.substr(i, j-i));
        }
    }
    sort(substrings.begin(), substrings.end());
    substrings.resize(unique(substrings.begin(), substrings.end()) - substrings.begin());

    uint64_t ans = 0;
    for (int i = 0; i < int(substrings.size()); i++) {
        for (int j = i+1; j < int(substrings.size()); j++) {
            if (substrings[i].find(substrings[j]) == string::npos && substrings[j].find(substrings[i]) == string::npos) {
                ans ++;
            }
        }
    }
    cout << ans << '\n';
}
