import threading
import time
import random

name_buffer = []
mutex = threading.Semaphore(1)
items = threading.Semaphore(0)

# Using extra randezvous semaphore so
# terminal doesn't get scrambled.
randezvous = threading.Semaphore(0)

def user_input_producer():
    while True:
        name = input("Enter a name (or type 'exit to quit): \n")
        
        mutex.acquire()
        name_buffer.append(name)
        mutex.release()
        
        items.release()
        
        if name.lower() == 'exit':
            break
        
        randezvous.acquire()
        
def greeting_consumer():
    while True:
        items.acquire()
        
        mutex.acquire()
        name_to_process = name_buffer.pop(0)
        mutex.release()
        
        if name_to_process.lower() == 'exit':
            print("Consumer thread shutting down...")
            break
        
        print(f"\nHello, {name_to_process}!\n")
        randezvous.release()

if __name__ == "__main__":
    t_producer = threading.Thread(target=user_input_producer)
    t_consumer = threading.Thread(target=greeting_consumer)
    
    t_producer.start()
    t_consumer.start()
    
    t_producer.join()
    t_consumer.join()
    
    print("Program successfully executed!")