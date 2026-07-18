package com.oplus.app;

import android.content.Context;

public class OplusHansFreezeManager {
    private static final OplusHansFreezeManager INSTANCE = new OplusHansFreezeManager();

    public static OplusHansFreezeManager getInstance() {
        return INSTANCE;
    }

    public int requestFastFreeze(Context context, int timeout, String reason) {
        return 0;
    }
}
