package com.cm.cmopengljni;

import android.util.Log;
/**
 * ÓÃì¶²Ù×÷JniŒÓOpenGL ES 2.0´ú´a
 * @author chenming
 *
 */
public class CmOpenGL {
	
	public static final String TAG = CmOpenGL.class.getSimpleName();
	
	static {
		System.loadLibrary("cmopengl");
	}
	
	public static long CMGL_POINTER = 0;
	
	public static native long onSurfaceCreate();
	
	public static native long onSurfaceChanged(long pointer, int width, int height);
	
	public static native long onDrawFrame(long pointer);

	public static native void release(long pointer);
	
	public static void close(long pointer) {
		Log.i(TAG, "close()");
		release(pointer);
		CMGL_POINTER = 0;
	}
	
	public static void drawFrame() {
		if (CMGL_POINTER != 0) {
			onDrawFrame(CMGL_POINTER);
		} else {
			Log.i(TAG, "drawFrame() CMGL_POINTER=0, you should init CMOpenGL!");
		} 
	}
	
	public static void surfaceChange(int width, int height) {
		Log.i(TAG, String.format("surfaceChange() width = %d height = %d!", width, height));
		if (CMGL_POINTER != 0) {
			onSurfaceChanged(CMGL_POINTER, width, height);
		} else {
			Log.i(TAG, "drawFrame() CMGL_POINTER=0, you should init CMOpenGL!");
		} 
	}
	
	public static void onCreate() {
		Log.i(TAG, "onCreate()");
		CMGL_POINTER = onSurfaceCreate();
		Log.i(TAG, "onCreate() CMGL_POINTER = " + CMGL_POINTER);
	}
}
