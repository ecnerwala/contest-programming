# Incorrect program. Only checks what happens when Marius always jumps into
# the left pipe.

answers = []

N, K = [int(x) for x in input().split()]
for k in range(K):
  print("? {} {}".format(k, "L" * N), flush=True)
  answers.append(int(input()))
print("!", flush=True)
Q = int(input())
for q in range(Q):
  k, s = input().split()
  if all([c == 'L' for c in s]):
    print(answers[int(k)], flush=True)
  else:
    # Oops, we have to guess.
    print(42, flush=True)
