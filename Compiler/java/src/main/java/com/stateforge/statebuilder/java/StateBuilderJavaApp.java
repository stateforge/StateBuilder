package com.stateforge.statebuilder.java;

import com.jdotsoft.jarloader.JarClassLoader;

public class StateBuilderJavaApp {
	public static void main(String[] args) {
		System.exit(run(args));
	}
	
	public static int run(String[] args) {
		JarClassLoader jcl = new JarClassLoader();
		try {
			return jcl.invokeRun("com.stateforge.statebuilder.java.StateBuilderJava", args);
		} catch (Throwable e) {
			e.printStackTrace();
			return -999;
		}
	}
}
