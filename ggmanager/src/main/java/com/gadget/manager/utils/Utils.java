package com.gadget.manager.utils;

public class Utils {

    public static final <T> boolean isEmpty (T[] t) {
        if (t == null || t.length == 0) {
            return true;
        }
        return false;
    }
}
