import threading
import time
import random

n = 5
count = 0

mutex = threading.Semaphore(1)
barrier = threading.Semaphore(0)

def worker(worker_id):
    global count
    
    time.sleep(random.uniform(0.5, 2.0))
    print(f"Worker {worker_id} has complete the randveouz step!")
    
    mutex.acquire()
    count += 1
    
    if count == n:
        print(f"\nThread {worker_id} is the last thread!\n")
        barrier.release()
    mutex.release()
    
    
    barrier.acquire()
    barrier.release()
    
    mutex.acquire()
    count -= 1
    
    if count == 0:
        print(f"\nThread {worker_id} is the last thread past the barrier!\n")
        barrier.acquire()
    mutex.release()
    
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