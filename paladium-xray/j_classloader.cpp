#include "j_classloader.hpp"
#include <stdexcept>

j_classloader::j_classloader(JNIEnv* env, jobject classloader)
{
	this->env = env;
	this->classloader = classloader;

	jclass c_urlclassloader = env->FindClass("java/net/URLClassLoader");

	if (!env->IsInstanceOf(classloader, c_urlclassloader))
	{
		throw std::invalid_argument("classloader must be an (instance of) URLClassLoader");
	}

	this->m_findClass = env->GetMethodID(c_urlclassloader, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");
	this->m_addURL = env->GetMethodID(c_urlclassloader, "addURL", "(Ljava/net/URL;)V");

	env->DeleteLocalRef(c_urlclassloader);
}

j_classloader::~j_classloader()
{
	env->DeleteLocalRef(this->classloader);
}

jclass j_classloader::find_class(const char* class_name)
{
	jclass clazz = nullptr;
	jstring j_class_name = env->NewStringUTF(class_name);

	clazz = reinterpret_cast<jclass>(env->CallObjectMethod(this->classloader, m_findClass, j_class_name));
	env->DeleteLocalRef(j_class_name);

	return clazz;
}

void j_classloader::add_url(jobject url)
{
	env->CallVoidMethod(this->classloader, m_addURL, url);
}