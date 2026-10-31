#include<bits/stdc++.h>
using namespace std;

using ll = long long;

int main() {
  ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
  int N; ll L; cin >> N >> L;
  deque<pair<int, double>> mp;
  mp.push_back(pair<int, double>(L, 0));
  double sum = 0;
  
  for (int i = 0; i < N; i++) {
    double t; int v; cin >> t >> v;
    int drain = v;
    while (drain > 0) {
      int amt = min(mp.back().first, drain);
      sum -= amt * mp.back().second;
      drain -= amt;
      mp.back().first -= amt;
      if (mp.back().first == 0) mp.pop_back();
    }
    sum += t * v;
    while (!mp.empty() && mp.front().second >= t) {
      t = t * v + mp.front().first * mp.front().second;
      v += mp.front().first;
      t /= v;
      mp.pop_front();
    }
    mp.push_front(pair<int, double>(v, t));
    cout << fixed << setprecision(10) << (sum/L) << '\n';
  }
}
