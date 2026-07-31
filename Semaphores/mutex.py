import threading
import time
import random

count = 0
mutex = threading.Semaphore(1)

def thread_a():
    global count
    time.sleep(random.random())
    
    mutex.acquire()
    count += 1
    print(count)
    mutex.release()
    
def thread_b():
    global count
    time.sleep(random.random())
    
    mutex.acquire()
    mutex.release()
    count += 1
    print(count)
    mutex.release()

if __name__ == "__main__":
    t1 = threading.Thread(target=thread_a)
    t2 = threading.Thread(target=thread_b)
    
    t1.start()
    t2.start()
    
    t1.join()
    t2.join()
    
    print("Mutex complete!")