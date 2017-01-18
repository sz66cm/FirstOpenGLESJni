package com.cm.cmopengljni;

import javax.microedition.khronos.opengles.GL10;

import android.app.Activity;
import android.content.pm.ActivityInfo;
import android.opengl.GLSurfaceView;
import android.opengl.Matrix;
import android.os.Bundle;
import android.util.Log;

public class MainActivity extends Activity {

	private GLSurfaceView view;
	private final String TAG = "cmopengles_JavaMatrix";
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		view = new GLSurfaceView(MainActivity.this);
		view.setEGLContextClientVersion(2);
		view.setRenderer(new GLSurfaceView.Renderer() {
			
			@Override
			public void onSurfaceCreated(GL10 gl,
					javax.microedition.khronos.egl.EGLConfig config) {
				CmOpenGL.onCreate();
			}
			
			@Override
			public void onSurfaceChanged(GL10 gl, int width, int height) {
				CmOpenGL.surfaceChange(width, height);
			}
			
			@Override
			public void onDrawFrame(GL10 gl) {
				CmOpenGL.drawFrame();
			}
		});
		setContentView(view);
	}
	
	@Override
	protected void onResume() {
		super.onResume();
		view.onResume();
	}
	@Override
	protected void onPause() {
		super.onPause();
		view.onPause();
	}
	
}
