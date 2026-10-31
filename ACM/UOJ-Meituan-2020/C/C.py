from pwn import *

timeout = 0.001
p = process('./tower_linux64')
#print(p.recvline())
#p.sendline('99275793') # For contest
p.sendline('99565380') # Practice seed

def update_state():
    global p
    global cur_keys
    state = p.recvuntil(b'Please enter your choice (move: WASD, restart: R):', drop=True).decode('ascii').strip()
    print('\n'.join('> ' + l for l in state.split('\n')))
    for line in state.split('\n'):
        if line.startswith('You keep'):
            tokens = line.split()
            cur_keys = [int(tokens[2]), int(tokens[8]), int(tokens[5])]
            print("keys:", cur_keys)

update_state()
def make_move(c):
    print("Moving", c)
    global p
    p.sendline(c)
    update_state()

def up(): make_move('W')
def down(): make_move('S')
def left(): make_move('A')
def right(): make_move('D')

def try_solve():
    global p

    up(); right()
    if cur_keys[0]:
        up()
    elif cur_keys[1]:
        pass
    elif cur_keys[2]:
        down()
    else: assert False
    right(); right(); right()
    down(); down()

    if cur_keys[0]:

    return False

n_wins = 0
while n_wins < 100:
    success = try_solve()
    if success:
        print("Success!")
        n_wins += 1
    else:
        print("Restarting")
        make_move('R')
p.shutdown()
