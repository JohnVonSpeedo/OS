import threading
import time
import random

a1Done = threading.Semaphore(0)
b1Done = threading.Semaphore(0)

def thread_a():
    time.sleep(random.random())
    print("Thread A: statement a1")
    a1Done.release()
    
    b1Done.acquire()
    print("Thread A: statement a2")
    
def thread_b():
    time.sleep(random.random())
    print("Thread B: statement b1")
    b1Done.release()
    
    a1Done.acquire()
    print("Thread B: statement b2")
    
if __name__ == "__main__":
    t1 = threading.Thread(target=thread_a)
    t2 = threading.Thread(target=thread_b)

    t1.start()
    t2.start()
    
    t1.join()
    t2.join()
    
    print("Randevouz complete.")    
