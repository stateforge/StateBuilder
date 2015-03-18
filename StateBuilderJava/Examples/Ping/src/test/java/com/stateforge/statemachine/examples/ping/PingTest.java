package com.stateforge.statemachine.examples.ping;

import java.util.concurrent.CountDownLatch;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import com.stateforge.statemachine.examples.ping.Ping;

/**
 * Unit test for Ping
 */
public class PingTest 
{
    private EchoServer server;
    private Ping ping;
    CountDownLatch countDownLatch = new CountDownLatch(1);
   
    @Before
    public void setUp() throws Exception {
        this.server = new EchoServer();
        this.server.start();
        this.ping = new Ping(new IPingListener() {
            public void end() {
                countDownLatch.countDown();
            }
        });
    }

    @After
    public void tearDown() throws Exception {
        //this.server.stop();
    } 

    @Test
    public void testStart()
    {
        this.ping.start();
        try {
            this.countDownLatch.await();
        } catch (InterruptedException e) {
            e.printStackTrace();
            junit.framework.Assert.assertTrue(false);
        }
    }
}
