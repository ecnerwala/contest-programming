#include <bits/stdc++.h>

int main() {
  using namespace std;
  ios_base::sync_with_stdio(false); cin.tie(nullptr);
  int N; cin >> N;
  std::vector<int> A(N);
  for (auto& a : A) { cin >> a; a--; }
  int Q; cin >> Q;
  std::vector<int> state(N, 3);
  std::vector<std::pair<int,int>> ops; ops.reserve(N+Q);
  ops.resize(Q);
  for (auto& [i, v] : ops) {
    cin >> i; i--;
    v = state[i];
    char t; cin >> t;
    state[i] = t=='C'?0:t=='O'?1:2;
  }
  for (int i=0; i<N; i++) {
    ops.push_back({i,state[i]});
    state[i] = 0;
  }
  std::reverse(ops.begin(), ops.end());
  
  std::array<int,3> cnt{N,0,0};
  std::vector<int> par(N, -1);
  auto get_par = [&](int a) {
    while (par[a] >= 0) {
      if (par[par[a]] >= 0) par[a] = par[par[a]];
      a=par[a];
    }
    return a;
  };
  std::vector<std::array<int,3>> ans; ans.reserve(N+Q);
  for (auto [i, v] : ops) {
    ans.push_back(cnt);
    assert(state[i] != 3);
    assert(par[i] < 0);
    int sz = -par[i];
    cnt[state[i]] -= sz;
    state[i] = v;
    if (v != 3) {
      cnt[state[i]] += sz;
    } else {
      int j = get_par(A[i]);
      if (j == i) continue;
      par[j] += par[i];
      par[i] = j;
      if (state[j] != 3) cnt[state[j]] += sz;
    }
  }
  std::reverse(ans.begin(), ans.end());
  ans.resize(Q);
  for (auto [a,b,c] : ans) {
    cout << a << ' ' << b << ' ' << c << '\n';
  }
}
