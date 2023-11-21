#include <Windows.h>
#include "utils.hpp"

typedef long(__stdcall* _JNI_GetCreatedJavaVMs)(JavaVM**, long, long*);

JavaVM* utils::get_jvm_instance()
{
    JavaVM* jvm;

    HMODULE jvm_handle = GetModuleHandleA("jvm.dll");
    _JNI_GetCreatedJavaVMs JNI_GetCreatedJavaVMs = reinterpret_cast<_JNI_GetCreatedJavaVMs>(GetProcAddress(reinterpret_cast<HMODULE>(jvm_handle), "JNI_GetCreatedJavaVMs"));
    JNI_GetCreatedJavaVMs(&jvm, 1, nullptr);

    return jvm;
}

j_classloader* utils::get_classloader_by_thread_name(JNIEnv* env, jvmtiEnv* jvmti, const char* thread_name)
{
    jint threadsCount;
    jthread* threads;

    jvmti->GetAllThreads(&threadsCount, &threads);

    for (int i = 0; i < threadsCount; ++i)
    {
        jthread thread = threads[i];
        jvmtiThreadInfo threadInfo;
        jvmti->GetThreadInfo(thread, &threadInfo);

        if (strcmp(thread_name, threadInfo.name) == 0) {
            return new j_classloader(env, threadInfo.context_class_loader);
        }
    }

    return NULL;
}
