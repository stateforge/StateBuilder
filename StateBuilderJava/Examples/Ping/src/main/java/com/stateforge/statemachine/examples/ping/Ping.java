package com.stateforge.statemachine.examples.ping;

import java.util.logging.Level;
import java.util.logging.Logger;

import com.stateforge.statemachine.context.IContextEnd;
import com.stateforge.statemachine.listener.ObserverConsole;

/**
 * Ping
 *
 */
public class Ping
{
    private EchoClient echoClient;
    private final PingContext context;
    private int rx; // Number of packet received
    private int tx; // Number of packet already sent
    private int count = 5; //Total number of packet to send
    private long timeout = 1000;// Timeout in msec between 2 packets
    
    private final IPingListener listener;
    private static final Logger logger = Logger.getLogger(Ping.class.getName());
    
    public Ping(final IPingListener listener) {
        this.listener = listener;
        this.context = new PingContext(this);
        this.context.setObserver(ObserverConsole.getInstance());
        this.context.setEndHandler(new IContextEnd() {
            public void end(Throwable throwable) {
                printStatistics();
                listener.end(); 
            }
        });
        
        this.echoClient = new EchoClient(new IEchoClientListener() {
            public void messageReceived() {
               rx++;
               context.evPingReply();
            }

            public void error(String message) {
                context.evError();
            }
        });
    }
    
    public int getRx() {
        return rx;
    }

    public void setRx(int rx) {
        this.rx = rx;
    }
    
    public int getTx() {
        return tx;
    }

    public void setTx(int tx) {
        this.tx = tx;
    }

    public int getCount() {
        return count;
    }

    public void setCount(int count) {
        this.count = count;
    }

    public long getTimeout() {
        return timeout;
    }

    public void setTimeout(long timeout) {
        this.timeout = timeout;
    }

    public void send(){
        this.tx++;
        this.echoClient.send();
    }
    
    public void start(){
        this.context.evStart();
    } 
    
    public void printStatistics(){
        logger.log(Level.INFO, this.tx + " packet(s) sent out of " + this.count + ", lost " + (this.tx - this.rx));
    }
}
