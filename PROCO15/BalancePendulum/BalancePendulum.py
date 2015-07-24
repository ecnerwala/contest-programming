import requests
import time

s = requests.Session()

def submit(val) :
    url = "http://proco2015-r2-890298229.us-west-1.elb.amazonaws.com/tasks/pendulum/submit"
    files = {"actions.txt": val}
    s.post(url, files=files)

def start_session() :
    global s
    data = {"username": "ecnerwal", "password" : "PfCazJAD"}
    url = "http://proco2015-r2-890298229.us-west-1.elb.amazonaws.com/login"
    s.post(url, data=data)

def gen_string(d, t1, t2, t3, t4) :
    res = ""
    for ang in range(-90, 91):
        for vel in range(-30, 30):
            right = False
            if(vel == 0 and ang == 0) :
                right = (d == 1)
            elif(abs(vel) == 0) :
                right = (ang > 0)
            elif(abs(vel) == 1) :
                if(abs(ang) >= t1) :
                    right = (ang > 0)
                else:
                    right = (vel > 0)
            elif(abs(vel) == 2) :
                if(abs(ang) >= t2) :
                    right = (ang > 0)
                else:
                    right = (vel > 0)
            elif(abs(vel) == 3) :
                if(abs(ang) >= t3) :
                    right = (ang > 0)
                else:
                    right = (vel > 0)
            elif(abs(vel) == 4) :
                if(abs(ang) >= t4) :
                    right = (ang > 0)
                else:
                    right = (vel > 0)
            else:
                right = vel > 0
            res = "{}{} {} {}\n".format(res, ang + 90, vel, 'R' if right else 'L')
    return res

def main() :
    start_session()
    for t4 in range(11, 13):
        for t3 in range(6, 13):
            for t2 in range(6, 13):
                for t1 in range(6, 13):
                    for d in range(2) :
                        if(t1 <= t2 and t2 <= t3 and t3 <= t4): 
                            submit(gen_string(d, t1, t2, t3, t4))
                            print("{} {} {} {} {}".format(d, t1, t2, t3, t4))

main()
