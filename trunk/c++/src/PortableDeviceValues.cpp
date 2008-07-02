/*
 * Copyright 2007 Pieter De Rycke
 * 
 * This file is part of JMTP.
 * 
 * JTMP is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as 
 * published by the Free Software Foundation, either version 3 of 
 * the License, or any later version.
 * 
 * JMTP is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU LesserGeneral Public 
 * License along with JMTP. If not, see <http://www.gnu.org/licenses/>.
 */

#include <PortableDeviceApi.h>

#include "jmtp.h"
#include "jmtp_PortableDeviceValuesImplWin32.h"

inline IPortableDeviceValues* GetPortableDeviceValues(JNIEnv* env, jobject obj)
{
	return (IPortableDeviceValues*)GetComReferencePointer(env, obj, "pDeviceValues");
}

JNIEXPORT void JNICALL Java_jmtp_PortableDeviceValuesImplWin32_clear
	(JNIEnv* env, jobject obj)
{
	//variabelen
	HRESULT hr;
	IPortableDeviceValues* pValues;


	//methode implementatie
	pValues = GetPortableDeviceValues(env, obj);
	hr = pValues->Clear();

	if(FAILED(hr))
	{
		ThrowCOMException(env, L"Failed to clear the collection", hr);
	}
}

JNIEXPORT jlong JNICALL Java_jmtp_PortableDeviceValuesImplWin32_count
	(JNIEnv* env, jobject obj)
{
	//variabelen
	HRESULT hr;
	IPortableDeviceValues* pValues;
	DWORD dwCount;


	//methode implementatie
	pValues = GetPortableDeviceValues(env, obj);
	hr = pValues->GetCount(&dwCount);

	if(SUCCEEDED(hr))
	{
		return dwCount;
	}
	else
	{
		ThrowCOMException(env, L"Failed to count the collection", hr);
		return -1;
	}
}

JNIEXPORT void JNICALL Java_jmtp_PortableDeviceValuesImplWin32_setStringValue
	(JNIEnv* env, jobject obj, jobject key, jstring value)
{
	HRESULT hr;
	IPortableDeviceValues* pValues;
	LPWSTR wszValue;

	pValues = GetPortableDeviceValues(env, obj);
	wszValue = (WCHAR*)env->GetStringChars(value, NULL);
	
	hr = pValues->SetStringValue(ConvertJavaToPropertyKey(env, key), wszValue);

	env->ReleaseStringChars(value, (jchar*)wszValue);
}

JNIEXPORT jstring JNICALL Java_jmtp_PortableDeviceValuesImplWin32_getStringValue
	(JNIEnv* env, jobject obj, jobject key)
{
	HRESULT hr;
	IPortableDeviceValues* pValues;
	LPWSTR wszValue;
	jstring jsValue;

	pValues = GetPortableDeviceValues(env, obj);

	hr = pValues->GetStringValue(ConvertJavaToPropertyKey(env, key), &wszValue);

	if(SUCCEEDED(hr))
	{
		jsValue = env->NewString((jchar*)wszValue, wcslen(wszValue));
		CoTaskMemFree(wszValue);
		return jsValue;
	}
	else
	{
		ThrowCOMException(env, L"Failed to get the string value", hr);
		return NULL;
	}
}

JNIEXPORT void JNICALL Java_jmtp_PortableDeviceValuesImplWin32_setGuidValue
	(JNIEnv* env, jobject obj, jobject key, jobject guid)
{
	HRESULT hr;
	IPortableDeviceValues* pValues;

	pValues = GetPortableDeviceValues(env, obj);

	if(guid == NULL || key == NULL)
	{
		env->ThrowNew(env->FindClass("java/lang/NullPointerException"), "Parameters can't be \"null\"");
		return;
	}

	hr = pValues->SetGuidValue(ConvertJavaToPropertyKey(env, key), ConvertJavaToGuid(env, guid));
	if(FAILED(hr))
	{
		ThrowCOMException(env, L"Failed to set the Guid value", hr);
		return;
	}
}

JNIEXPORT jobject JNICALL Java_jmtp_PortableDeviceValuesImplWin32_getGuidValue
	(JNIEnv* env, jobject obj, jobject jobjKey)
{
	HRESULT hr;
	IPortableDeviceValues* pValues;
	GUID guid;

	pValues = GetPortableDeviceValues(env, obj);

	if(jobjKey == NULL)
	{
		env->ThrowNew(env->FindClass("java/lang/NullPointerException"), "PropertyKey can't be null");
		return NULL;
	}

	hr = pValues->GetGuidValue(ConvertJavaToPropertyKey(env, jobjKey), &guid);
	if(FAILED(hr))
	{
		ThrowCOMException(env, L"Failed to retrieve the Guid value", hr);
		return NULL;
	}
	else
		return ConvertGuidToJava(env, guid);
}

JNIEXPORT void JNICALL Java_jmtp_PortableDeviceValuesImplWin32_setUnsignedIntegerValue
	(JNIEnv* env, jobject obj, jobject key, jlong value)
{
	//variabelen
	HRESULT hr;
	IPortableDeviceValues* pValues;

	
	//methode implementatie
	if(key != NULL)
	{
		if(key >= 0)
		{
			pValues = GetPortableDeviceValues(env, obj);
			hr = pValues->SetUnsignedIntegerValue(ConvertJavaToPropertyKey(env, key), static_cast<ULONG>(value));
			if(FAILED(hr))
			{
				ThrowCOMException(env, L"Failed to set the integer value", hr);
			}
		}
		else
		{
			env->ThrowNew(env->FindClass("java/lang/IllegalArgumentException"), "The integer must be possitive");
		}
	}
	else
	{
		env->ThrowNew(env->FindClass("java/lang/NullPointerException"), "PropertyKey can't be null");
	}
}

JNIEXPORT jlong JNICALL Java_jmtp_PortableDeviceValuesImplWin32_getUnsignedIntegerValue
	(JNIEnv* env, jobject obj, jobject key)
{
	//variabelen
	HRESULT hr;
	IPortableDeviceValues* pValues;
	ULONG value;


	//methode implementatie
	if(key != NULL)
	{
		pValues = GetPortableDeviceValues(env, obj);
		hr = pValues->GetUnsignedIntegerValue(ConvertJavaToPropertyKey(env, key), &value);
		if(SUCCEEDED(hr))
			return value;
		else
		{
			ThrowCOMException(env, L"Failed to retrieve the integer value", hr);
			return -1;
		}
	}
	else
	{
		env->ThrowNew(env->FindClass("java/lang/NullPointerException"), "PropertyKey can't be null");
		return -1;
	}
}

JNIEXPORT void JNICALL Java_jmtp_PortableDeviceValuesImplWin32_setPortableDeviceValuesCollectionValue
	(JNIEnv* env, jobject obj, jobject jobjKey, jobject jobjValue)
{
	//variabelen
	HRESULT hr;
	IPortableDeviceValues* pValues;
	IPortableDevicePropVariantCollection* pPropVariantCollection;
	jobject jobjReference;


	//methode implementatie
	if(jobjKey != NULL && jobjValue != NULL)
	{
		pValues = GetPortableDeviceValues(env, obj);
		jobjReference = RetrieveCOMReferenceFromCOMReferenceable(env, jobjValue);
		pPropVariantCollection = 
			(IPortableDevicePropVariantCollection*)ConvertComReferenceToPointer(env, jobjReference);
		hr = pValues->SetIPortableDevicePropVariantCollectionValue(
			ConvertJavaToPropertyKey(env, jobjKey), pPropVariantCollection);
		if(FAILED(hr))
		{
			ThrowCOMException(env, L"Failed to set the propvariant collection", hr);
		}
	}
	else
	{
		env->ThrowNew(env->FindClass("java/lang/NullPointerException"), "parameters can't be null");
	}
}

JNIEXPORT jobject JNICALL Java_jmtp_PortableDeviceValuesImplWin32_getPortableDeviceValuesCollectionValue
	(JNIEnv* env, jobject obj, jobject jobjKey)
{
	//variabelen
	HRESULT hr;
	IPortableDeviceValues* pValues;
	IPortableDevicePropVariantCollection* pPropVariantCollection;
	jclass cls;
	jmethodID mid;
	jobject jobjReference;


	//methode implementatie
	if(jobjKey != NULL)
	{
		pValues = GetPortableDeviceValues(env, obj);
		hr = pValues->GetIPortableDevicePropVariantCollectionValue(
			ConvertJavaToPropertyKey(env, jobjKey), &pPropVariantCollection);
		if(SUCCEEDED(hr))
		{
			cls = env->FindClass("be/derycke/pieter/com/COMReference");
			mid = env->GetMethodID(cls, "<init>", "(J)V");
			jobjReference = env->NewObject(cls, mid, pPropVariantCollection);
			
			cls = env->FindClass("be/derycke/pieter/wpd/PortableDevicePropVariantCollectionImplWin32");
			mid = env->GetMethodID(cls, "<init>", "(Lbe/derycke/pieter/com/COMReference;)V");
			return env->NewObject(cls, mid, jobjReference);
		}
		else
		{
			ThrowCOMException(env, L"Failed to retrieve the propvariant collection", hr);
		}
	}
	else
	{
		env->ThrowNew(env->FindClass("java/lang/NullPointerException"), "key can't be null");
	}

	return NULL;
}

JNIEXPORT jboolean JNICALL Java_jmtp_PortableDeviceValuesImplWin32_getBoolValue
	(JNIEnv* env, jobject obj, jobject jobjKey)
{
	//variabelen
	HRESULT hr;
	IPortableDeviceValues* pValues;
	BOOL bValue;

	//methode implementatie
	if(jobjKey != NULL)
	{
		pValues = GetPortableDeviceValues(env, obj);
		hr = pValues->GetBoolValue(ConvertJavaToPropertyKey(env, jobjKey), &bValue);
		if(SUCCEEDED(hr))
		{
			return bValue;
		}
		else
		{
			ThrowCOMException(env, L"Failed to retrieve the boolean", hr);
		}
	}
	else
	{
		env->ThrowNew(env->FindClass("java/lang/NullPointerException"), "key can't be null");
	}

	return NULL;
}

JNIEXPORT void JNICALL Java_jmtp_PortableDeviceValuesImplWin32_setFloateValue
	(JNIEnv* env, jobject obj, jobject jobjKey, jfloat jfValue)
{
	//variabelen
	HRESULT hr;
	IPortableDeviceValues* pValues;


	//methode implementatie
	if(jobjKey != NULL)
	{
		pValues = GetPortableDeviceValues(env, obj);
		hr = pValues->SetFloatValue(ConvertJavaToPropertyKey(env, jobjKey), jfValue);
	}
	else
	{
		env->ThrowNew(env->FindClass("java/lang/NullPointerException"), "key can't be null");
	}
}

JNIEXPORT jfloat JNICALL Java_jmtp_PortableDeviceValuesImplWin32_getFloatValue
	(JNIEnv* env, jobject obj, jobject jobjKey)
{
	//variabelen
	HRESULT hr;
	IPortableDeviceValues* pValues;
	FLOAT value;

	//methode implementatie
	if(jobjKey != NULL)
	{
		pValues = GetPortableDeviceValues(env, obj);
		hr = pValues->GetFloatValue(ConvertJavaToPropertyKey(env, jobjKey), &value);
		if(SUCCEEDED(hr))
		{
			return value;
		}
		else
		{
			ThrowCOMException(env, L"Failed to retrieve the float", hr);
		}
	}
	else
	{
		env->ThrowNew(env->FindClass("java/lang/NullPointerException"), "key can't be null");
	}

	return NULL;
}

JNIEXPORT jthrowable JNICALL Java_jmtp_PortableDeviceValuesImplWin32_getErrorValue
	(JNIEnv* env, jobject obj, jobject jobjKey)
{
	//variabelen
	HRESULT hr;
	IPortableDeviceValues* pValues;
	HRESULT error;
	jclass cls;
	jmethodID mid;
	jstring jsMessage;


	//methode implementatie
	if(jobjKey != NULL)
	{
		pValues = GetPortableDeviceValues(env, obj);
		hr = pValues->GetErrorValue(ConvertJavaToPropertyKey(env, jobjKey), &error);

		if(SUCCEEDED(hr))
		{
			cls = env->FindClass("be/derycke/pieter/com/COMException");
			mid = env->GetMethodID(cls, "<init>", "(Ljava/lang/String;I)V");
			jsMessage = env->NewString((jchar*)L"The request is not supported.", 29);
			return (jthrowable)env->NewObject(cls, mid, jsMessage, (jint)error);
		}
		else
		{
			ThrowCOMException(env, L"Failed to retrieve the error", hr);
			return NULL;
		}
	}
	else
	{
		env->ThrowNew(env->FindClass("java/lang/NullPointerException"), "key can't be null");
		return NULL;
	}
}

JNIEXPORT void JNICALL Java_jmtp_PortableDeviceValuesImplWin32_setUnsignedLargeIntegerValue
	(JNIEnv* env, jobject obj, jobject jobjKey, jobject jobjValue)
{
	//variabelen
	HRESULT hr;
	IPortableDeviceValues* pValues;
	ULONGLONG value;


	//methode implementatie
	if(jobjKey != NULL)
	{
		if(jobjValue != NULL)
		{
			pValues = GetPortableDeviceValues(env, obj);
			value = ConvertJavaToUnsignedLongLong(env, jobjValue);
			hr = pValues->SetUnsignedLargeIntegerValue(ConvertJavaToPropertyKey(env, jobjKey), value);

			if(FAILED(hr))
			{
				ThrowCOMException(env, L"Failed to set the unsigned large integer value.", hr);
			}
		}
		else
		{
			env->ThrowNew(env->FindClass("java/lang/NullPointerException"), "value can't be null");
		}
	}
	else
	{
		env->ThrowNew(env->FindClass("java/lang/NullPointerException"), "key can't be null");
		return;
	}
}