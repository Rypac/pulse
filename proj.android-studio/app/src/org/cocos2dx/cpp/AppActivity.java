package org.cocos2dx.cpp;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;

public class AppActivity extends Cocos2dxActivity {

    private Cocos2dxGLSurfaceView glSurfaceView;

    public Cocos2dxGLSurfaceView onCreateView() {
        glSurfaceView = new Cocos2dxGLSurfaceView(this);
        glSurfaceView.setEGLConfigChooser(5, 6, 5, 0, 16, 8);
        this.hideSystemUI();
        return glSurfaceView;
    }

    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);
        if (hasFocus) {
            this.hideSystemUI();
        }
    }

    private void hideSystemUI() {
        glSurfaceView.setSystemUiVisibility(
            Cocos2dxGLSurfaceView.SYSTEM_UI_FLAG_LAYOUT_STABLE |
            Cocos2dxGLSurfaceView.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION |
            Cocos2dxGLSurfaceView.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN |
            Cocos2dxGLSurfaceView.SYSTEM_UI_FLAG_HIDE_NAVIGATION |
            Cocos2dxGLSurfaceView.SYSTEM_UI_FLAG_FULLSCREEN |
            Cocos2dxGLSurfaceView.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
        );
    }
}
