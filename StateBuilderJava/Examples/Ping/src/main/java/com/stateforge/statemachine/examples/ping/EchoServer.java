package com.stateforge.statemachine.examples.ping;

import java.net.InetSocketAddress;
import java.util.concurrent.Executors;

import org.jboss.netty.bootstrap.ServerBootstrap;
import org.jboss.netty.channel.ChannelPipeline;
import org.jboss.netty.channel.ChannelPipelineFactory;
import org.jboss.netty.channel.Channels;
import org.jboss.netty.channel.socket.nio.NioServerSocketChannelFactory;

public class EchoServer {

    private ServerBootstrap bootstrap;
    private int port = 2011;
    
    public EchoServer(){
        // Configure the server.
        this.bootstrap = new ServerBootstrap(
                new NioServerSocketChannelFactory(
                        Executors.newCachedThreadPool(),
                        Executors.newCachedThreadPool()));

        // Set up the pipeline factory.
        this.bootstrap.setPipelineFactory(new ChannelPipelineFactory() {
            public ChannelPipeline getPipeline() throws Exception {
                return Channels.pipeline(new EchoServerHandler());
            }
        });  
    }
    
    public void start(){
        // Bind and start to accept incoming connections.
        this.bootstrap.bind(new InetSocketAddress(port));  
    }
    
    public void stop(){
        //TODO it hangs herer and eat 100% CPU
        bootstrap.releaseExternalResources();
    }
}
