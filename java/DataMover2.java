import java.util.ArrayList;
import java.util.List;
import java.util.Random;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicInteger;

class DataMover2Result {
    public int count=0;
    public int data=0;
    public int forwarded=0;
}

public class DataMover2 {
    public static AtomicInteger arrivalCount= new AtomicInteger(0);
    public static AtomicInteger totalSent = new AtomicInteger(0);
    public static AtomicInteger totalArrived = new AtomicInteger(0);
    public static ExecutorService pool;
    public static List<BlockingQueue<Integer>> queues;
    public static List<Future<DataMover2Result>> moverResults;
    public static List<Integer> discards;
    private static Random  rand = new Random();

    public static Integer got(Integer threadIdx) throws InterruptedException {
        long waitingTime= rand.nextInt(701)+300;
        return queues.get(threadIdx).poll(waitingTime, TimeUnit.MILLISECONDS);
    }

    public static void sends(Integer threadIdx, Integer x) throws InterruptedException {
        queues.get(threadIdx).put(x);
    }

    public static void forwards(Integer threadIdx, Integer x) throws InterruptedException {
        queues.get(threadIdx).put(x);
    }

    public static void main(String[] args) {
        String[] params;
        if(args.length<1) {
            params = new String[]{"123", "111", "256", "404"};
        } else {
            params=args;
        }
        int threadCount = params.length;
        int POOL_SIZE=100;
        int QUEUE_SIZE=threadCount*100;
        pool = Executors.newFixedThreadPool(POOL_SIZE);
        queues = new  ArrayList<>(threadCount);
        moverResults = new ArrayList<>(threadCount);
        discards = new ArrayList<>(threadCount);
        for (int i=0; i< threadCount; i++) {
            queues.add(new LinkedBlockingQueue<Integer>(QUEUE_SIZE));
        }
        
        for(int i=0; i<threadCount; i++) {
            final int threadIdx = i;
            Future<DataMover2Result> r = pool.submit(() -> {
                DataMover2Result result = new DataMover2Result();
                try {
                    while(arrivalCount.get()<threadCount*5) {
                        Integer nextInd = threadIdx+1;
                        if(nextInd==threadCount) {
                            nextInd=0;
                        }

                        String toPrint = "/" + threadCount*5 + " | #" + threadIdx;
                        
                        Integer x = rand.nextInt(10001);
                        if(arrivalCount.get()>=threadCount*5) break;
                        sends(nextInd, x);
                        synchronized(System.out) {
                            System.out.print("total " +  arrivalCount.get() + toPrint + " sends " + x + '\n');
                        }
                        synchronized (totalSent) {
                           totalSent.getAndAdd(x); 
                        }

                        if(arrivalCount.get()>=threadCount*5) break;
                        Integer getInt = got(threadIdx);
                        if(getInt!=null) {
                            if(getInt % threadCount==threadIdx) {
                                synchronized (arrivalCount) {
                                    arrivalCount.incrementAndGet();
                                }
                                result.count++;
                                result.data+=getInt;
                            synchronized(System.out) {
                                System.out.print("total " +  arrivalCount.get() + toPrint + " got " + getInt + '\n');
                            }
                            } else {
                                forwards(nextInd, getInt-1);
                                synchronized(System.out) {
                                    System.out.print("total " +  arrivalCount.get() + toPrint + " forwards " + (getInt-1) + " [" + nextInd + "]\n");
                                }
                                result.forwarded++;
                            }
                            Thread.sleep(Integer.parseInt(params[threadIdx]));
                        } else {
                            synchronized(System.out) {
                              System.out.print("total " + arrivalCount.get() + toPrint + " got nothing...\n");  
                            } 
                        }
                    }
                } catch(Exception e) {
                    System.err.println(e);
                }
                return result;  
            });
            moverResults.add(r);
        }

        pool.shutdown();

        try {
            if (!pool.awaitTermination(30, TimeUnit.SECONDS)) {
                pool.shutdownNow();
            }
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        
        for (Future<DataMover2Result> d2r : moverResults) {
            try {
                synchronized(totalArrived) {
                    totalArrived.getAndAdd((d2r.get().data)+(d2r.get().forwarded));
                }
            } catch (InterruptedException e) {
                e.printStackTrace();
            } catch (ExecutionException e) {
                e.printStackTrace();
            }  
        }

        int discardsSum=0;
        for (int j=0; j<queues.size(); j++) {
            int sum=0;
            int queSize=queues.get(j).size();
            for(int i=0; i<queSize; i++) {
                try {
                    sum+=queues.get(j).take();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
            discards.add(sum);
            discardsSum+=sum;
        }

        Integer arrivedPlusDiscardsSum =totalArrived.get()+discardsSum;
        System.out.println("discarded " + discards + " = " + discardsSum );
       
        if(totalSent.get()==arrivedPlusDiscardsSum) {
            System.out.println("sent " + totalSent.get() + " ==== got " + arrivedPlusDiscardsSum + " = " + totalArrived.get() + " + discarded " +  discardsSum  );
        } else {
            System.out.println("WRONG sent " + totalSent.get() + " !== got " + arrivedPlusDiscardsSum + " = " + totalArrived.get() + " + discarded " +  discardsSum  );
        }
    }
}