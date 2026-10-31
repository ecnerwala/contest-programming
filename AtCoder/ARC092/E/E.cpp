#include<bits/stdc++.h>
using namespace std;

int main() {
  int N; cin >> N;
  vector<int> A(N);
  for (int i = 0; i < N; i++) cin >> A[i];
  if (*max_element(A.begin(),A.end()) <= 0) {
    auto ind = int(max_element(A.begin(), A.end())-A.begin());
    cout << A[ind] << '\n';
    cout << N-1 << '\n';
    for (int i = N-1; i > ind; i--) {
      cout << i+1 << '\n';
    }
    for (int i = 0; i < ind; i++) {
      cout << 0+1 << '\n';
    }
    exit(0);
  }
  long long bestVal = 0;
  vector<int> bestOps;
  for (int z = 0; z < 2; z++) {
    vector<int> curOps;
    long long curVal = 0;
    for (int i = N-1; i >= 0; i--) {
      if ((i&1) == z) {
        if (A[i] < 0) curOps.push_back(i);
        else curVal += A[i];
      } else {
        if (i>0 && A[i-1] < 0) ;
        else curOps.push_back(i);
      }
    }
    if (curVal > bestVal) { bestVal = curVal, bestOps = curOps; }
  }
  assert(bestVal > 0);
  cout << bestVal << '\n';
  cout << bestOps.size() << '\n';
  for (int v : bestOps) cout << v+1 << '\n';
}
