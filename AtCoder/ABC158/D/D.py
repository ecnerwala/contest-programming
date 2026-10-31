S,f=__import__('collections').deque(input()),0
for _ in range(int(input())):
	T=input()
	if'2'>T:f=1-f
	else:(S.appendleft if(T[2]<'2')^f else S.append)(T[4])
if f:S.reverse()
print(*S,sep='')
