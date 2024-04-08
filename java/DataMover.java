import java.util.concurrent.TimeUnit;

class ThreadForArray extends Thread {
    private Integer[] data;
    private int stepCount;
    private int indWaitingTime;
    private int allWaitingTime;
    private int threadIdx;
    private Object[] objArr;

    public ThreadForArray(Integer[] data, Object[] objArr, int stepCount, int indWaitingTime, int allWaitingTime, int threadIdx) {
        this.data=data;
        this.stepCount=stepCount;
        this.indWaitingTime=indWaitingTime;
        this.allWaitingTime=allWaitingTime;
        this.threadIdx=threadIdx;
        this.objArr=objArr;
    }
    @Override
    public void run() {
        for (int j = 0; j < stepCount; j++) {
            try {
                Thread.sleep(indWaitingTime);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            int nextThreadIdx = (threadIdx + 1) % data.length;

            int curLock = threadIdx;
            int nextLock = nextThreadIdx;
            if(curLock>nextLock) {
                int temp = curLock;
                curLock = nextLock;
                nextLock = temp;
            }

            synchronized(objArr[curLock]) {
                synchronized(objArr[nextLock]) {
                    int readNum=0;
                    data[threadIdx]-=threadIdx;
                    readNum= data[threadIdx];
                    synchronized(System.out) {
                        System.out.printf("#%d: data %d == %d\n", threadIdx, threadIdx, data[threadIdx]);
                    }
                    try {
                        TimeUnit.MILLISECONDS.sleep(allWaitingTime);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                    data[nextThreadIdx]=data[nextThreadIdx]+readNum;
                    synchronized(System.out) {
                        System.out.printf("#%d: data %d -> %d\n", threadIdx, nextThreadIdx, data[nextThreadIdx]);
                    }
                }
            }
        }
    }
}

public class DataMover {
    public static Integer[] data;
    public static Thread[] movers;
    
    private static void printArr() {
        String ptstr="[";
        for(int i=0; i<data.length-1; i++) {
            ptstr=ptstr+data[i]+", ";
        }
        ptstr=ptstr+data[data.length-1]+"]";
        System.out.println(ptstr);
    }

    public static void main(String[] args) {
        String[] params;
        if(args.length<1) {
            params = new String[]{"123", "111", "256", "404"};
        } else {
            params=args;
        }
        int STEP_COUNT = 10;
     
        data = new Integer[params.length-1];
        movers = new ThreadForArray[params.length-1];
        int waitingTime=Integer.parseInt(params[0]);
        int[] indwaitingTimes = new int[params.length-1];
        Object[] objArr = new Object[params.length-1];
        for(int i=0; i<params.length-1; i++) {
            data[i]=i*1000;
            indwaitingTimes[i]=Integer.parseInt(params[i+1]);
            objArr[i]=new Object();
        } 
        for(int i=0; i<params.length-1; i++) {
            movers[i] = new ThreadForArray(data, objArr, STEP_COUNT, indwaitingTimes[i], waitingTime, i);
        }

        for(Thread mover : movers) {
            mover.start();
        }

        try {
            for(Thread mover : movers) {
                mover.join();
            }
        } catch (InterruptedException e) {
            System.out.println(e);
        }
        
        printArr();
    }
}
