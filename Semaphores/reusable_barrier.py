import threading
import time
import random

n = 5
count = 0

mutex = threading.Semaphore(1)
turnstile1 = threading.Semaphore(0)
turnstile2 = threading.Semaphore(1)

def worker(worker_id):
    global count
    
    time.sleep(random.uniform(0.5, 2.0))
    print(f"Worker {worker_id} has complete the randveouz step!")
    
    #mutual exclusion 1
    mutex.acquire()
    count += 1
    
    if count == n:
        print(f"\nThread {worker_id} is the last thread!\n")
        turnstile2.acquire()      #locking the second barrier
        turnstile1.release()      #unlocking the first barrier
    mutex.release()
    
    #first barrier
    turnstile1.acquire()
    turnstile1.release()
    
    #mutual exclusion 2
    mutex.acquire()
    count -= 1
    
    if count == 0:
        print(f"\nThread {worker_id} is the last thread past the barrier!\n")
        turnstile1.acquire()      #locking the first barrier
        turnstile2.release()      #unlocking the second barrier
    mutex.release()
    
    #second barrier
    turnstile2.acquire()
    turnstile2.release()
    
    print(f"Thread {worker_id} passed into the critical point!")
    
if __name__ == "__main__":
    threads = []
    for i in range(1, n + 1):
        t = threading.Thread(target=worker, args=[i])
        threads.append(t)
        t.start()
        
    for t in threads:
        t.join()
        
    print(f"\nAll threads passed the barrier!")