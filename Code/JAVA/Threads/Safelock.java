package getReady;

import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;
import java.util.Random;

public class Safelock {
	
    static class Friend {
    	
        private final String name;
        private final Lock lock = new ReentrantLock();

        public Friend(String name) {
            this.name = name;
        }

        public String getName() {
            return this.name;
        }

        public boolean impendingBow(Friend bower) { //if someone is waiting on a bow
            Boolean myLock = false;
            Boolean yourLock = false;
            try {
                myLock = lock.tryLock();
                yourLock = bower.lock.tryLock();
            } finally {
                if (! (myLock && yourLock)) { //if we are stuck
                    if (myLock) { // if i have the lock (i am bowing to him) then unlock mine
                        lock.unlock();
                    }
                    if (yourLock) { // if my friend can lock (already bowing to me) then unlock his
                        bower.lock.unlock();
                    }
                }
            }
            return myLock && yourLock;//return true if we both could lock or false else
        }
            
        public void bow(Friend bower) {//bow
            if (impendingBow(bower)) {//if we can both bow, we lock and he bows to me
                try {
                    System.out.format("%s: %s has"
                        + " bowed to me!%n", 
                        this.name, bower.getName());
                    bower.bowBack(this); // i bow back to him
                } finally {
                    lock.unlock(); //unlock mine (i bowed back)
                    bower.lock.unlock(); //unlock his (bowed to me) 
                }
            } else { // if false, then i am bowing to him (tried to get my lock but lock was taken)
                System.out.format("%s: %s started"
                    + " to bow to me, but saw that"
                    + " I was already bowing to"
                    + " him.%n",
                    this.name, bower.getName());
            }
        }

        public void bowBack(Friend bower) {//i will bow back to him
            System.out.format("%s: %s has" +
                " bowed back to me!%n",
                this.name, bower.getName());
        }
    }

    static class BowLoop implements Runnable {
        private Friend bower;
        private Friend bowee;

        public BowLoop(Friend bower, Friend bowee) {
            this.bower = bower;
            this.bowee = bowee;
        }
    
        public void run() {
            Random random = new Random();
            for (;;) {
                try {
                    Thread.sleep(random.nextInt(10));
                } catch (InterruptedException e) {}
                bowee.bow(bower);
            }
        }
    }
            

    public static void main(String[] args) {
        final Friend alphonse =
            new Friend("A");
        final Friend gaston =
            new Friend("B");
        new Thread(new BowLoop(alphonse, gaston)).start();
        new Thread(new BowLoop(gaston, alphonse)).start();
    }
}
