
package com.stateforge.statemachine.examples.ping;

import java.util.concurrent.ScheduledFuture;
import java.util.concurrent.TimeUnit;
import com.stateforge.statemachine.context.AbstractContext;

public class PingContext
    extends AbstractContext<PingPingState, PingContext>
{
    private Ping _ping;
    private ScheduledFuture myTimerScheduledFuture;

    /**
     * Context constructor
     */
    public PingContext(Ping ping) {
        super();
        _ping = ping;
        setName("PingContext");
        setInitialState(PingIdleState.getInstance());
    }

    public Ping getPing() {
        return _ping;
    }

    /**
     * Enter the initial state
     */
    public void enterInitialState() {
        com.stateforge.statemachine.algorithm.StateOperation.walkTreeEntry(this, PingPingState.getInstance(), PingIdleState.getInstance());
    }

    /**
     * Leave the current state
     */
    public void leaveCurrentState() {
        com.stateforge.statemachine.algorithm.StateOperation.walkTreeExit(this, this.getStateCurrent(), PingPingState.getInstance());
    }

    /**
     * Asynchronous event evStart
     */
    public void evStart() {
        final PingContext me = this;
        getExecutorService().execute(new Runnable() {
            public void run() {
                try {
                    getStateCurrent().evStart(me);
                } catch (Exception exception) {
                    onEnd(exception);
                }
            }
        });
    }

    /**
     * Asynchronous event evCancel
     */
    public void evCancel() {
        final PingContext me = this;
        getExecutorService().execute(new Runnable() {
            public void run() {
                try {
                    getStateCurrent().evCancel(me);
                } catch (Exception exception) {
                    onEnd(exception);
                }
            }
        });
    }

    /**
     * Asynchronous event evError
     */
    public void evError() {
        final PingContext me = this;
        getExecutorService().execute(new Runnable() {
            public void run() {
                try {
                    getStateCurrent().evError(me);
                } catch (Exception exception) {
                    onEnd(exception);
                }
            }
        });
    }

    /**
     * Asynchronous event evPingReply
     */
    public void evPingReply() {
        final PingContext me = this;
        getExecutorService().execute(new Runnable() {
            public void run() {
                try {
                    getStateCurrent().evPingReply(me);
                } catch (Exception exception) {
                    onEnd(exception);
                }
            }
        });
    }

    /**
     * Start the timer Timer
     */
    public void timerStartTimer(long duration) {
        getObserver().onTimerStart(this.getName(), "Timer", duration);
        final PingContext me = this;
        myTimerScheduledFuture = getExecutorService().schedule(new Runnable() {
            public void run() {
                try {
                    getStateCurrent().evTimer(me);
                } catch (Exception exception) {
                    onEnd(exception);
                }
            }
        }, duration, TimeUnit.MILLISECONDS);
    }

    /**
     * Stop the timer Timer
     */
    public void timerStopTimer() {
        getObserver().onTimerStop(this.getName(), "Timer");
        if (myTimerScheduledFuture!= null) {
            myTimerScheduledFuture.cancel(false);
        }
    }
}
