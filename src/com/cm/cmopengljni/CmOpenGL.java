package com.cm.cmopengljni;

public class CmOpenGL {
	static {
		System.loadLibrary("cmopengl");
	}
	
	public static native long onSurfaceCreate();
	
	public static native long onSurfaceChanged(long pointer, int width, int height);
	
	public static native long onDrawFrame(long pointer);
}
