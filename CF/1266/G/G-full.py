'''
// There are n! / (i+1)! * i permutations with decreasing seq length i (i+1 changes) for i <= n!
// There are n! / (i+1)! permutations for each 1 <= j <= i of position of moved one

// All nonempty things
+ n*n! * (n*n! + 1) / 2

// Subtract off everything inside one of them
- n! * n * (n+1) / 2

// Subtract off everything that doesn't include the min-incr
- sum_{perm i,j} n * (n-i)

// We're left with all words that include at least last of prv -> min-incr, which all have at least one match

// No matches
+ sum_{i=1..n} n!/(n-i)!

// First match is dist N, doesn't include anything
// aaa....aaa -> middle part can't all be decreasing
+ sum_{i=1..n} n! - n!/(n-i)!

// First match is dist N, includes subst
// aaabxyc   aaac
// Everything decreasing xycb
+ sum_{i=1..n-1} n!/(i+1)! * (n-i-1)

// First match is dist N, includes min-incr -> is unique, already counted

// First match is < N -> either came from min-incr or from subst
// You can distinguish because match..match includes everything smaller or everything smaller minus one of them
// Ending at min-incr gives unique thing

// How many distinct things end at subst?
// Ending at subst:   (b ijk ) lmn c xyz  ... c such that zyxbcnmlkji are sorted leftovers
                   or   b ijk lmn c xyz .... c
// Might as well not include xyz
+ sum_{i=1..n-1} n!/(i+1)! * (i+1)

// Simplified claim:
if it includes min-incr, then it's always unique;
if it includes subst, only count if it came from the end (j = 1) (in which case it always matches)
otherwise, it's just some delta-N match or it's all distinct

n*n! * (n*n! + 1) / 2
- n! * n * (n+1) / 2
- sum_{i=1..n-1} n! / (i+1)! * i * n * (n-i)
+ sum_{i=1..n} n! / (n-i)!
+ sum_{i=1..n} n! - n! / (n-i)!
+ sum_{i=1..n-1} n! / (i+1)! * n

= n*n! * (n*n! + 1) / 2
- n! * n * (n+1) / 2
- sum_{i=1..n-1} n! / (i+1)! * n * (i * (n-i) - 1)
+ n * n!

= n*n! * (n*n! - n + 2) / 2
- sum_{i=1..n-1} n! / (i+1)! * n * (i * (n-i) - 1)
'''

def fact(n):
    res = 1
    for i in range(1,n+1):
        res *= i
    return res

def f(n):
    l = fact(n) * n
    ans = l * (l-n+2) // 2
    for i in range(1,n):
        ans -= fact(n) // fact(i+1) * n * (i * (n-i) - 1)
    return ans

#print(f(int(input())) % 998244353)

def g(n):
    M = 998244353
    p = n
    a = 0
    for i in range(n,1,-1):
        a=(a+p*(i-1)*(n-i+1)-p)%M
        p=p*i%M
    a=(p*(p-n+2)-a-a)%M
    if a&1:a+=M
    return a//2

print(g(int(input())))
