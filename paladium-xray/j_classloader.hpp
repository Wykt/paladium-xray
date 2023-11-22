#pragma once

#include <jni.h>

class j_classloader {
public:
	j_classloader(JNIEnv* env, jobject class_loader);
	~j_classloader();

	jclass find_class(const char* class_name);
private:
	JNIEnv* env;
	jobject classloader;
	jmethodID m_findClass;
};