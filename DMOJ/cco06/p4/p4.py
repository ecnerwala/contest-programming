# i think this algorithm runs in n^2 because i'm checking all starting locations
# probably not though, someone else can tell me
times = []
for _ in range(int(raw_input())):
 # note that each degree takes 12.00 seconds
 times.append(int(float(raw_input().split()[1])*1200+0.5))
times = sorted(times)
charge = int(raw_input())*100
shift = times[0]
for i in range(len(times)): times[i] -= shift
# so there are 144 minutes or 2 full rotations
# each rotation is 4320.00 seconds
# check if three rotations are necessary
def blocktime(times,charge):
 times = list(times)
 out = []
 for i in range(2):
  times.append(times[i]+432000)
 # now check
 for i in range(len(times)-2):
  if times[i+1]-times[i] >= charge: out.append(0)
  elif times[i+2]-times[i] >= charge: out.append(1)
  else: return []   # since only two rotations are allowed
 return out
# very important method, should be fast
def rotatetime(times,blocks,charge):
 # guaranteed len(times)>=2
 # first element of times is always used
 lastsecond = -2    # on second rotation, this is the index of the last element
 # check getting the last index during first rotation
 pos = len(times)-1
 while pos > 0:
  if blocks[pos-1] == 0: pos -= 1
  else: break
 # check possible
 if pos % 2 == 1:
  if 0 not in set(blocks[1:pos:2]):
   pos += 1
 # done
 if pos < len(times):
  lastsecond = pos-1
 # check getting the second last index
 if lastsecond == -2:
  pos = len(times)-2
  if pos % 2 == 1:
   if 0 not in set(blocks[1:pos:2]):
    pos += 2
  if pos < len(times):
   lastsecond = pos+1
 # start returning things
 if lastsecond == -2: return 864001
 if lastsecond == -1: return times[-1]+charge
 return times[lastsecond]+432000+charge
def besttime(times,charge):
 if len(times) == 1: return charge
 out = 864001
 blocks = blocktime(times,charge)
 if blocks == []: return out
 for _ in range(len(times)):
  temp = rotatetime(times,blocks,charge)
  if temp < out:
   out = temp
  # shift for next starting position
  blocks.append(blocks.pop(0))
  times.append(times.pop(0)+432000)
  shift = times[0]
  for i in range(len(times)): times[i] -= shift
 return out
result = besttime(times,charge)
if result > 864000: print "not possible"
else: print (result+99)//100
