import threading
import time
import random

multiplex = None

def nightclub_guest(guest_id):
    print(f"Guest {guest_id} is waiting in line...")
    
    multiplex.acquire()
    
    print(f"Guest {guest_id} ENTERED the club!")
    time.sleep(random.uniform(1, 3))
    print(f"Guest {guest_id} LEFT the club!")
    
    multiplex.release()
    
if __name__ == "__main__":
    try:
        user_input = input("Enter the maximum number of thread allowed in the club: ")
        max_threads = int(user_input)
    except ValueError:
        print("Invalid input. Defaulting to 3 threads.")
        max_threads = 3
        
    multiplex = threading.Semaphore(max_threads)
        
    print(f"\n--- Club opening with a capacity of {max_threads} ---\n")
    
    threads = []
    
    for i in range(1, 31):
        t = threading.Thread(target=nightclub_guest, args=[i])
        threads.append(t)
        t.start()
        
    for t in threads:
        t.join()
    