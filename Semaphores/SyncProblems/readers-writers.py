import threading
import time
import random

roomEmpty = threading.Semaphore(1)
mutex = threading.Semaphore(1)
readers = 0
data = "Initial data"

def reader(thread_id):
    global readers
    
    time.sleep(random.uniform(0.1, 0.8))
    
    mutex.acquire()
    readers += 1
    if readers == 1:
        roomEmpty.acquire()
    mutex.release()
    
    print(f"Reader {thread_id} read: '{data}' (Total readers inside: {readers})")
    time.sleep(random.uniform(0.1, 0.2))
    
    mutex.acquire()
    readers -= 1
    if readers == 0:
        roomEmpty.release()
    mutex.release()
    

def writer(thread_id):
    global data
    
    time.sleep(random.uniform(0.1, 0.2))
    
    roomEmpty.acquire()
    data = f"Data writen by writer {thread_id}!"
    print(f"Writer {thread_id} updated the data.")
    roomEmpty.release()

if __name__ == "__main__":
    threads = []
    
    for i in range(1, 6):
        t_reader = threading.Thread(target=reader, args=[i])
        threads.append(t_reader)
        t_reader.start()
        
    for i in range(1, 3):
        t_writer = threading.Thread(target=writer, args=[i])
        threads.append(t_writer)
        t_writer.start()
        
    for t in threads:
        t.join()
        
    print("\nAll threads finished!")