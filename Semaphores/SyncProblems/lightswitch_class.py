import threading
import time
import random

class Lightswitch:
    def __init__(self):
        self.counter = 0
        self.mutex = threading.Semaphore(1)
        
    def lock(self, semaphore):
        self.mutex.acquire()
        self.counter +=1
        if self.counter == 1:
            semaphore.acquire()
        self.mutex.release()

    def unlock(self, semaphore):
        self.mutex.acquire()
        self.counter -= 1
        if self.counter == 0:
            semaphore.release()
        self.mutex.release()
        
data = "Initial Data"
roomEmpty = threading.Semaphore(1)
read_switch = Lightswitch()

def reader(thread_id):
    time.sleep(random.uniform(0.1, 0.8))
    
    read_switch.lock(roomEmpty)
    
    print(f"Reader {thread_id} read: '{data}' (Total readers inside: {read_switch.counter})")
    time.sleep(random.uniform(0.1, 0.2))
    
    read_switch.unlock(roomEmpty)
    

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
    