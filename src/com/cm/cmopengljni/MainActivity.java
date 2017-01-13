package com.cm.cmopengljni;

import javax.microedition.khronos.opengles.GL10;

import android.app.Activity;
import android.opengl.GLSurfaceView;
import android.os.Bundle;

public class MainActivity extends Activity {

	private GLSurfaceView view;
	private long cOpenGLHandle;
	private CmOpenGL openGLHandle;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		view = new GLSurfaceView(MainActivity.this);
		view.setEGLContextClientVersion(2);
		view.setRenderer(new GLSurfaceView.Renderer() {
			
			@Override
			public void onSurfaceCreated(GL10 gl,
					javax.microedition.khronos.egl.EGLConfig config) {
				CmOpenGL.onSurfaceCreate();
			}
			
			@Override
			public void onSurfaceChanged(GL10 gl, int width, int height) {
				CmOpenGL.onSurfaceChanged(cOpenGLHandle, width, height);
			}
			
			@Override
			public void onDrawFrame(GL10 gl) {
				CmOpenGL.onDrawFrame(cOpenGLHandle);
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
