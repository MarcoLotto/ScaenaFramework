package com.example.scaena;

import android.app.NativeActivity;
import android.content.Context;
import android.os.Bundle;
import android.util.Log;
import android.view.WindowManager;
import android.view.inputmethod.InputMethodManager;
import android.os.Vibrator;

public class CustomNativeActivity extends NativeActivity {

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        // Evitamos que la pantalla entre en reposo
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
    }
	
    // Metodos a ser invocados por ndk en c++
	public void showSoftKeyboard(){
        ((InputMethodManager)getSystemService(Context.INPUT_METHOD_SERVICE)).toggleSoftInput(InputMethodManager.SHOW_FORCED, 0);
    }
    public void hideSoftKeyboard(){
        ((InputMethodManager)getSystemService(Context.INPUT_METHOD_SERVICE)).toggleSoftInput(InputMethodManager.HIDE_IMPLICIT_ONLY, 0);
    }
	public void vibrate(int timeInMilliseconds){
		Vibrator vibrator = (Vibrator) getSystemService(Context.VIBRATOR_SERVICE);
		if(vibrator != null){
			vibrator.vibrate(timeInMilliseconds);
		}
	}
	
	/** Aqui pueden definirse todos los metodos que se precisen **/
}