/*
 * Copyright (C) 2026 The AxionOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

package com.oplus.datasync;

import android.os.Bundle;
import android.util.Log;

/**
 * Compatibility shim for Oplus apps that publish cloud/RUS data through the
 * stock DataSync framework.
 */
public class OplusDataSyncManager {
    private static final String TAG = "OplusDataSyncManager";

    private static final OplusDataSyncManager INSTANCE = new OplusDataSyncManager();

    private OplusDataSyncManager() {
    }

    public static OplusDataSyncManager getInstance() {
        return INSTANCE;
    }

    public boolean updateAppData(String module, Bundle data) {
        Log.d(TAG, "updateAppData: module=" + module);
        return true;
    }
}
