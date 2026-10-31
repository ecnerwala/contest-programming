import math
def exact_sqrt(x):
    v = math.isqrt(x)
    assert v * v == x
    return v

AO2, BO2, CO2, DO2 = map(int, input().split())

mid = AO2 + CO2
prod = (AO2 - CO2) ** 2 + (BO2 - DO2) ** 2

v = exact_sqrt(mid ** 2 - prod)
delta_y2 = 2 * v
sum_y = 2 * exact_sqrt(AO2)
assert delta_y2 % sum_y == 0
delta_y = delta_y2 // sum_y

#assert (sum_y + delta_y) % 2 == 0
y_mu_2 = (sum_y + delta_y) // 2
y_sigma_2 = (sum_y - delta_y) // 2

x_mu_2 = exact_sqrt((mid + v) - y_mu_2 ** 2)
x_sigma_2 = -exact_sqrt((mid - v) - y_sigma_2 ** 2)
assert x_mu_2 + x_sigma_2 == 0

ax = (x_mu_2 + x_sigma_2)//2
ay = (y_mu_2 + y_sigma_2)//2
bx = (x_mu_2 + y_sigma_2)//2
by = (y_mu_2 - x_sigma_2)//2
cx = (x_mu_2 - x_sigma_2)//2
cy = (y_mu_2 - y_sigma_2)//2
dx = (x_mu_2 - y_sigma_2)//2
dy = (y_mu_2 + x_sigma_2)//2
if (bx ** 2 + by ** 2) == DO2 and (dx**2 + dy**2) == BO2:
    bx, by, dx, dy = dx, dy, bx, by
print(ay, bx, by, cx, cy, dx, dy)
