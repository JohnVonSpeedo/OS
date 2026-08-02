import threading
import time
import random

class Barrier:
    def __init__(self, n):
        self.n = n
        self.count = 0
        self.mutex = threading.Semaphore(1)
        self.turnstile1 = threading.Semaphore(0)
        self.turnstile2 = threading.Semaphore(0)
        
    def phase1(self):
        self.mutex.acquire()
        self.count += 1
        if self.count == self.n:
            self.turnstile1.release(self.n)
        self.mutex.release()
        
        self.turnstile1.acquire()
        
    def phase2(self):
        self.mutex.acquire()
        self.count -= 1
        if self.count == 0:
            self.turnstile2.release(self.n)
        self.mutex.release()
        
        self.turnstile2.acquire()
        
    def wait(self):
        self.phase1()
        self.phase2()
        
def worker(worker_id, sync_barrier, loops=3):
    for i in range(1, loops + 1):
        # 1. Independent work phase (threads run at different speeds)
        time.sleep(random.uniform(0.1, 0.8))
        print(f"Worker {worker_id} arrived at the barrier (Loop {i}).")
        
        # 2. Synchronization phase
        sync_barrier.wait()
        
        # 3. Critical point - no thread reaches here until everyone has arrived
        if worker_id == 0: 
            print(f"\n--- Everyone passed Loop {i}! Moving to next phase. ---\n")
            time.sleep(0.1)
        
if __name__ == "__main__":
    total_threads = 5
    barrier = Barrier(total_threads)
    
    threads = []
    
    for i in range(total_threads):
        t = threading.Thread(target=worker, args=[i, barrier])
        threads.append(t)
        t.start()
        
    for thread in threads:
        thread.join()
        
    print("All workers have finished all loops safely!")