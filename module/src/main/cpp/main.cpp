#include <jni.h>
#include <sys/types.h>
#include <riru.h>
#include <malloc.h>
#include <cstring>
#include "utils.h"
#include <dlfcn.h>
#include <unistd.h>
#include "pthread.h"

void get_pkg_name(char* pkgname, int size){

    const char * log_path = "/data/local/tmp/xyzpkgname";
    FILE* log =  fopen(log_path, "r");
    if(log == NULL) {
        LOGW("%s not exist, Pls create it !",log_path);
        return;
    }
    fgets(pkgname, size, log);
    fclose(log);
    str_strip(pkgname);
}

static void loadGadGetLib() {
    while (1) {
#if defined (__x86_64__) || defined (__aarch64__)
        const char* libggpath = "/system/lib64/libxyzgg.so";
#else
        const char* libggpath = "/system/lib/libxyzgg.so";
#endif
        if (access(libggpath,F_OK) == 0) {
            LOGV("%s exist!", libggpath);
        } else {
            LOGW("%s not exist!", libggpath);
        }
        void* handle = dlopen(libggpath,RTLD_LOCAL);
        if(handle == NULL) {
            LOGE("dlopen libxyzgg.so error. %s ", dlerror());
            sleep(10);
            continue;
        }
        LOGV("dlopen libxyzgg.so success.");
        return;
    }
}

static void forkAndSpecializePre(
        JNIEnv *env, jclass clazz, jint *_uid, jint *gid, jintArray *gids, jint *runtimeFlags,
        jobjectArray *rlimits, jint *mountExternal, jstring *seInfo, jstring *niceName,
        jintArray *fdsToClose, jintArray *fdsToIgnore, jboolean *is_child_zygote,
        jstring *instructionSet, jstring *appDataDir, jboolean *isTopApp, jobjectArray *pkgDataInfoList,
        jobjectArray *whitelistedDataInfoList, jboolean *bindMountAppDataDirs, jboolean *bindMountAppStorageDirs) {

    const char* nice = env->GetStringUTFChars(*niceName, NULL);
//    LOGV("specializeAppProcessPre appDataDir : %s, niceName : %s", datadir, nice);
    memset(g_pkgname, 0, sizeof(g_pkgname));
    strncpy(g_pkgname, nice, strlen(nice));

}

static void forkAndSpecializePost(JNIEnv *env, jclass clazz, jint res) {
    if (res == 0) {
        // in app process
        char pkgname[128];
        get_pkg_name(pkgname, 128);
        if (strcmp(pkgname, g_pkgname) == 0) {
            LOGV("forkAndSpecializePost package : %s", g_pkgname);
            int32_t try_count = 10;
            pthread_t thread_id;
            while (pthread_create(&thread_id, NULL, reinterpret_cast<void *(*)(void *)>(loadGadGetLib), NULL) && try_count > 0) {
                LOGW("loadGadGetLib thread error, sleep 1 second, and try again.");
                sleep(1);
                --try_count;
            }
        }
    } else {
        // in zygote process, res is child pid
        // don't print log here, see https://github.com/RikkaApps/Riru/blob/77adfd6a4a6a81bfd20569c910bc4854f2f84f5e/riru-core/jni/main/jni_native_method.cpp#L55-L66
    }
}

static void specializeAppProcessPre(
        JNIEnv *env, jclass clazz, jint *_uid, jint *gid, jintArray *gids, jint *runtimeFlags,
        jobjectArray *rlimits, jint *mountExternal, jstring *seInfo, jstring *niceName,
        jboolean *startChildZygote, jstring *instructionSet, jstring *appDataDir,
        jboolean *isTopApp, jobjectArray *pkgDataInfoList, jobjectArray *whitelistedDataInfoList,
        jboolean *bindMountAppDataDirs, jboolean *bindMountAppStorageDirs) {
    // added from Android 10, but disabled at least in Google Pixel devices
}

static void specializeAppProcessPost(
        JNIEnv *env, jclass clazz) {
    // added from Android 10, but disabled at least in Google Pixel devices
}

static void forkSystemServerPre(
        JNIEnv *env, jclass clazz, uid_t *uid, gid_t *gid, jintArray *gids, jint *runtimeFlags,
        jobjectArray *rlimits, jlong *permittedCapabilities, jlong *effectiveCapabilities) {

}

static void forkSystemServerPost(JNIEnv *env, jclass clazz, jint res) {

    if (res == 0) {
        // in system server process
        /**
        int32_t try_count = 10;
        pthread_t thread_id;
        LOGV("forkSystemServerPost system server process ");
        while (pthread_create(&thread_id, NULL, reinterpret_cast<void *(*)(void *)>(loadGadGetLib), NULL) && try_count > 0) {
            LOGW("loadGadGetLib thread error, sleep 1 second, and try again.");
            sleep(1);
            --try_count;
        }
        */
    } else {
        // in zygote process, res is child pid
        // don't print log here, see https://github.com/RikkaApps/Riru/blob/77adfd6a4a6a81bfd20569c910bc4854f2f84f5e/riru-core/jni/main/jni_native_method.cpp#L55-L66
    }
}

static int shouldSkipUid(int uid) {
    // by default, Riru only call module functions in "normal app processes" (10000 <= uid % 100000 <= 19999)
    // false = don't skip
    return false;
}

static void onModuleLoaded() {
    // called when the shared library of Riru core is loaded
}

extern "C" {

int riru_api_version;
RiruApiV9 *riru_api_v9;

/*
 * Init will be called three times.
 *
 * The first time:
 *   Returns the highest version number supported by both Riru and the module.
 *
 *   arg: (int *) Riru's API version
 *   returns: (int *) the highest possible API version
 *
 * The second time:
 *   Returns the RiruModuleX struct created by the module.
 *   (X is the return of the first call)
 *
 *   arg: (RiruApiVX *) RiruApi strcut, this pointer can be saved for further use
 *   returns: (RiruModuleX *) RiruModule strcut
 *
 * The second time:
 *   Let the module to cleanup (such as RiruModuleX struct created before).
 *
 *   arg: null
 *   returns: (ignored)
 *
 */
void *init(void *arg) {
    static int step = 0;
    step += 1;

    static void *_module;

    switch (step) {
        case 1: {
            auto core_max_api_version = *(int *) arg;
            riru_api_version = core_max_api_version <= RIRU_MODULE_API_VERSION ? core_max_api_version : RIRU_MODULE_API_VERSION;
            return &riru_api_version;
        }
        case 2: {
            switch (riru_api_version) {
                case 9: {
                    riru_api_v9 = (RiruApiV9 *) arg;

                    auto module = (RiruModuleInfoV9 *) malloc(sizeof(RiruModuleInfoV9));
                    memset(module, 0, sizeof(RiruModuleInfoV9));
                    _module = module;

                    module->supportHide = true;

                    module->version = RIRU_MODULE_VERSION;
                    module->versionName = RIRU_MODULE_VERSION_NAME;
                    module->onModuleLoaded = onModuleLoaded;
                    module->shouldSkipUid = shouldSkipUid;
                    module->forkAndSpecializePre = forkAndSpecializePre;
                    module->forkAndSpecializePost = forkAndSpecializePost;
                    module->specializeAppProcessPre = specializeAppProcessPre;
                    module->specializeAppProcessPost = specializeAppProcessPost;
                    module->forkSystemServerPre = forkSystemServerPre;
                    module->forkSystemServerPost = forkSystemServerPost;
                    return module;
                }
                default: {
                    return nullptr;
                }
            }
        }
        case 3: {
            free(_module);
            return nullptr;
        }
        default: {
            return nullptr;
        }
    }
}
}