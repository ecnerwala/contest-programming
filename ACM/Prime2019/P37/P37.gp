\\ We work with generating fuctions in F[[x, y]] / y^2, i.e. formal power series in 2 variables, modulo y^2
\\ In particular, the coefficient of y corresponds to the total number of non-inversions, while the coefficient of 1 corresponds to total number

\\ Then, let C = 2 / (1+sqrt(1-4x)) be the catalan generating function, or the number of 123-free permutations
\\ We have I = (C-1) / (1-4x) * x is the generating function of total number of non-inversions in 123-free permutations
\\
\\ Then, we have V = C + Iy = C + (C-1) / (1-4x) * x * y is the gen func for M = 0

\\ Let u = sqrt(1-4x)
x = (1-u^2) / 4
\\u = sqrt(1-4*x)

C = 2 / (1+u)
V = C + (C-1) / (1-4*x) * x * y
V11 = (1-x) * V - 1
V1 = (1+y) * V11 * V11 / x
V12 = V11 - x * (V-1) * (1+y)
V2 = 2 * V12 * V12 / x^2 * (1+y) + V1 * (1-x) * V11 / x * (1+y)
V13 = V12 - x * (V - V11 + V12 - x - 1) * (1 + 2 * y)
V3 = 2 * V13 * V13 / x^3 * (1+y) + 4 * V12^2 / x^2 * (1+y) * (1-x) * V11 / x * (1+y) + V1 * (1-x) * V11 / x * (1+y) * (1-x) * V11 / x * (1+y)

to_ans(v) = deriv(deriv(v % y, 'u) / deriv(x, 'u), 'u) / deriv(x, 'u) / 2 * x^2 - (v % y^2 - v%y) / y
\\to_ans(v) = deriv(deriv(v, 'x), 'x) / 2 * x^2 * y - v

u2 = (1-4*z)
A0 = to_ans(V)
\\ A0 = (u^5 - 5*u^4 + 8*u^3 - 4*u^2 - u + 1) / (1-u^2) / u^3 / 4
A0 = (u2^2*u - 5*u2^2 + 8*u2^1*u - 4*u2^1 - u + 1) / (1-u2) / u^3 / 4
\\ A0 = ((2*z^2 - 5*z + 1)*u + (-10*z^2 + 7*z - 1))/(2*z*u^3)
A1 = to_ans(V1)
\\ A1 = (-u^10 + u^9 + 3*u^8 + 32*u^7 - 182*u^6 + 378*u^5 - 406*u^4 + 232*u^3 - 57*u^2 - 3*u + 3) / (1-u^2)^3 / u^3 / 4
A1 = (-u2^5 + u2^4*u + 3*u2^4 + 32*u2^3*u - 182*u2^3 + 378*u2^2*u - 406*u2^2 + 232*u2^1*u - 57*u2^1 - 3*u + 3) / (1-u2)^3 / u^3 / 4
\\ A1 = ((2*z^4 - 18*z^3 + 60*z^2 - 34*z + 5)*u + (8*z^5 - 4*z^4 + 90*z^3 - 118*z^2 + 44*z - 5))/(2*z^3*u^3)
A2 = to_ans(V2)
\\ A2 = (3*u^15 - 7*u^14 + 19*u^13 - 155*u^12 + 335*u^11 - 43*u^10 + 1423*u^9 - 12159*u^8 + 35529*u^7 - 56901*u^6 + 56281*u^5 - 35041*u^4 + 12965*u^3 - 2249*u^2 - 59*u + 59) / (1-u^2)^5 / u^3 / 16
A2 = (3*u2^7*u - 7*u2^7 + 19*u2^6*u - 155*u2^6 + 335*u2^5*u - 43*u2^5 + 1423*u2^4*u - 12159*u2^4 + 35529*u2^3*u - 56901*u2^3 + 56281*u2^2*u - 35041*u2^2 + 12965*u2^1*u - 2249*u2^1 - 59*u + 59) / (1-u2)^5 / u^3 / 16
\\ ((-6*z^7 + 20*z^6 - 64*z^5 + 109*z^4 - 352*z^3 + 342*z^2 - 117*z + 13)*u + (14*z^7 - 102*z^6 + 140*z^5 - 467*z^4 + 854*z^3 - 550*z^2 + 143*z - 13))/(2*z^5*u^3)
A3 = to_ans(V3)
\\ A3 = (-u^20 + 3*u^19 - 6*u^18 + 55*u^17 - 187*u^16 + 60*u^15 + 1448*u^14 - 6916*u^13 + 17702*u^12 - 13654*u^11 - 71764*u^10 + 304690*u^9 - 624550*u^8 + 824236*u^7 - 749496*u^6 + 474892*u^5 - 204341*u^4 + 55147*u^3 - 7318*u^2 - 113*u + 113) / (1-u^2)^7 / u^3 / 8
A3 = (-u2^10 + 3*u2^9*u - 6*u2^9 + 55*u2^8*u - 187*u2^8 + 60*u2^7*u + 1448*u2^7 - 6916*u2^6*u + 17702*u2^6 - 13654*u2^5*u - 71764*u2^5 + 304690*u2^4*u - 624550*u2^4 + 824236*u2^3*u - 749496*u2^3 + 474892*u2^2*u - 204341*u2^2 + 55147*u2^1*u - 7318*u2^1 - 113*u + 113) / (1-u2)^7 / u^3 / 8
\\ A3 = ((-12*z^9 + 82*z^8 - 152*z^7 - 294*z^6 + 788*z^5 + 543*z^4 - 1732*z^3 + 1108*z^2 - 280*z + 25)*u + (-16*z^10 + 64*z^9 - 286*z^8 + 96*z^7 + 1368*z^6 - 834*z^5 - 2661*z^4 + 3488*z^3 - 1618*z^2 + 330*z - 25))/(2*z^7*u^3)

\\ 1/u^2 = 1/(1-4x) = sum 4^k x^k
\\ 1/u^3 = 1/(1-4x)^{3/2} = sum (k+0.5 choose k) 4^k x^k = sum 4^k * (2k+1)! / 2^k / (2^k * k!) / k! x^k = sum (2k+1)! / k! / k! x^k
