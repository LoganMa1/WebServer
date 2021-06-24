import requests
from concurrent.futures import ThreadPoolExecutor
import time
import random

file = ["index.html","index1.html","index2.html","index3.html","index4.html","index5.html"]


def print_status(URL,n):
    ts = time.time()
    for i in range(n):
        response  = requests.get(URL+"/"+file[random.randint(0,5)])
        #response  = requests.get(URL)
        if i%10==0:
            print("the {} time's status_code is {}".format(i,response.status_code))
    te = time.time()
    print("spend {} s in this function\n".format(te-ts))


def printtext(URL,n):
    ts = time.time()
    for i in range(n):
        response  = requests.get(URL)
        print(response.text())
    te = time.time()
    print("spend {} s in this function\n".format(te-ts))
if __name__ == '__main__':
    URL = "http://127.0.0.1:7878"
    
    with ThreadPoolExecutor(max_workers = 3) as executor:
        futures = executor.submit(print_status,URL,100)
