#include <bits/stdc++.h>
int main() {
int T; std::cin >> T;
while (T--) {
int N; std::cin >> N;
int ans = int(2e9);
int A; std::cin >> A;
for (int i = 1; i < N; i++) {
int B; std::cin >> B;
if (B < A) ans = 0;
else ans = std::min(ans, (B - A + 2) / 2);
A = B;
}
std::cout << ans << '\n';
}
}
