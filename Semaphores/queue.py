import threading
import time
import random

leaders_waiting = 0
followers_waiting = 0

mutex = threading.Semaphore(1)
follower_queue = threading.Semaphore(0)
leader_queue = threading.Semaphore(0)
randezvous = threading.Semaphore(0)

def follower(thread_id):
    global leaders_waiting, followers_waiting
    
    #simulating random arrival time
    time.sleep(random.uniform(0.1, 1.0))
    print(f"Follower {thread_id} arrived at the queue!")
    
    mutex.acquire()
    if leaders_waiting > 0:
        leaders_waiting -= 1
        leader_queue.release()
    else:
        followers_waiting += 1
        mutex.release()
        follower_queue.acquire()
    
    print(f"Follower {thread_id} entered the dance floor!")
    randezvous.release()
    

def leader(thread_id):
    global leaders_waiting, followers_waiting
    
    #simulating random arrival time
    time.sleep(random.uniform(0.1, 1.0))
    print(f"Leader {thread_id} arrived at the queue!")
    
    mutex.acquire()
    if followers_waiting > 0:
        followers_waiting -= 1
        follower_queue.release()
    else:
        leaders_waiting += 1
        mutex.release()
        leader_queue.acquire()
        
    print(f"Leader {thread_id} entered the dance floor!")
    randezvous.acquire()
    mutex.release()


if __name__ == "__main__":
    total_pairs = 5
    threads = []
    
    for i in range(1, total_pairs + 1):
        t_leader = threading.Thread(target=leader, args=[i])
        t_follower = threading.Thread(target=follower, args=[i])
        
        threads.append(t_leader)
        threads.append(t_follower)
        t_leader.start()
        t_follower.start()
        
    for t in threads:
        t.join()
        
    print("\nAll dancers have successfully paired up and finished dancing!")
        
        