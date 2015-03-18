package com.stateforge.statebuilder.examples.Unitest01;

import java.util.concurrent.CountDownLatch;

import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;

import com.stateforge.statemachine.context.IContextEnd;
import com.stateforge.statemachine.examples.Unitest01.Action;
import com.stateforge.statemachine.examples.Unitest01.Unitest01Context;
import com.stateforge.statemachine.listener.ObserverConsole;

/**
 * Unit test for Unitest01.
 * Objective to test what's happen when an unchecheck exception is thrown inside an action
 */
public class Unitest01Test   
{
    private Unitest01Context unitest01Context;
    private Action action = new Action();
    CountDownLatch countDownLatch = new CountDownLatch(1);
   
    @Before
    public void setUp() throws Exception {
    	this.unitest01Context = new Unitest01Context(action);
    	this.unitest01Context.setObserver(ObserverConsole.getInstance());
    	this.unitest01Context.setEndHandler(new IContextEnd() {
			@Override
			public void end(Throwable throwable) {
				Assert.assertNotNull(throwable);
				countDownLatch.countDown();
			}
		});
    }

    @Test
    public void testStart()
    {
        try {
        	this.unitest01Context.start();
            this.countDownLatch.await();
        } catch (InterruptedException e) {
            e.printStackTrace();
            junit.framework.Assert.assertTrue(false);
        }
    }
}
