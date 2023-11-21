#pragma once

#include <jni.h>
#include <jvmti.h>
#include "j_classloader.hpp"

namespace utils {
	JavaVM* get_jvm_instance();
	j_classloader* get_classloader_by_thread_name(JNIEnv* env, jvmtiEnv* jvmti, const char* thread_name);
}