D(a, b, k) = { res = 0; forprime(p=a,a+b-1, res += lift(Mod(1/(k - 1), p))); res };
ans = D(10^9,10^5,10^5);
print(ans);
