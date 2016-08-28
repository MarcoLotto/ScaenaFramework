package com.example.scaena;

import android.app.NativeActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.WindowManager;


public class CustomNativeActivity extends NativeActivity {

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        // Evitamos que la pantalla entre en reposo
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
    }
	
    // Metodos a ser invocados por ndk en c++
	/*
    public void nombreMetodoJava(){
        Log.e("CustomNativeActivity", "JAVA WORLD - FUNCION SIN PARAMETROS");
    }
	*/
}