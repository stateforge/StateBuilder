
package com.stateforge.statemachine.examples.ping;

import java.net.InetSocketAddress;
import java.util.concurrent.Executors;
import java.util.logging.Level;
import java.util.logging.Logger;

import org.jboss.netty.bootstrap.ClientBootstrap;
import org.jboss.netty.buffer.ChannelBuffer;
import org.jboss.netty.buffer.ChannelBuffers;
import org.jboss.netty.channel.ChannelHandlerContext;
import org.jboss.netty.channel.ChannelPipeline;
import org.jboss.netty.channel.ChannelPipelineFactory;
import org.jboss.netty.channel.ChannelStateEvent;
import org.jboss.netty.channel.Channels;
import org.jboss.netty.channel.ExceptionEvent;
import org.jboss.netty.channel.MessageEvent;
import org.jboss.netty.channel.SimpleChannelUpstreamHandler;
import org.jboss.netty.channel.socket.nio.NioClientSocketChannelFactory;

public class EchoClient extends SimpleChannelUpstreamHandler {

    private ClientBootstrap bootstrap;
    private String target = "127.0.0.1";
    private int port = 2011;
    private static final Logger logger = Logger.getLogger(
            EchoClient.class.getName());

    private final ChannelBuffer channelBuffer;
    private IEchoClientListener listener;
    
    public EchoClient(IEchoClientListener listener){
        this.listener = listener;
        // Configure the client.
        this.bootstrap = new ClientBootstrap(
                new NioClientSocketChannelFactory(
                        Executors.newCachedThreadPool(),
                        Executors.newCachedThreadPool()));

        // Set up the pipeline factory.
        final EchoClient me = this;
        this.bootstrap.setPipelineFactory(new ChannelPipelineFactory() {
            public ChannelPipeline getPipeline() throws Exception {
                return Channels.pipeline(me);
            }
        }); 
        
        int firstMessageSize = 256;
        channelBuffer = ChannelBuffers.buffer(firstMessageSize);
        for (int i = 0; i < channelBuffer.capacity(); i ++) {
            channelBuffer.writeByte((byte) i);
        }
    }
    
    public void stop(){
        bootstrap.releaseExternalResources();
    }

    public void send(){
        logger.log(Level.INFO, "send");
        // Start the connection attempt.
        this.bootstrap.connect(new InetSocketAddress(target, port)); 
    }

    @Override
    public void channelConnected(
            ChannelHandlerContext ctx, ChannelStateEvent e) {
        logger.log(Level.INFO, "channelConnected");
        e.getChannel().write(this.channelBuffer);
    }

    @Override
    public void messageReceived(
            ChannelHandlerContext ctx, MessageEvent e) {
        logger.log(Level.INFO, "messageReceived");
        listener.messageReceived();
    }

    @Override
    public void exceptionCaught(
            ChannelHandlerContext ctx, ExceptionEvent e) {
        logger.log(Level.WARNING, "Unexpected exception from downstream.", e.getCause());
        e.getChannel().close();
        listener.error(e.getCause().getMessage());
    }
}
