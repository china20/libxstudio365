#include "secure_number.h"
#include <assert.h>
#include "mathext.h"

int FloatToInt(float val)
{
	return *((int*)&val);
}

float IntToFloat(int val)
{
	return *((float*)&val);
}


template<>
secure_number<int>::operator int() const
{
	const int actualValue = m_randomValue ^ m_confuseValue;
#ifdef _DEBUG
	assert(actualValue == m_actualValue);
#endif
	return actualValue;
}

template<>
void secure_number<int>::Init()
{
	m_randomValue = mathext::rand() + 341; // 341 = (101010101)2
	m_confuseValue = m_randomValue;
#ifdef _DEBUG
	m_actualValue  = 0;
#endif
}

template<>
void secure_number<int>::Set(int val)
{
	m_confuseValue = m_randomValue ^ val;
#ifdef _DEBUG
	m_actualValue  = val;
#endif
}

template<>
secure_number<float>::operator float() const
{
	const float actualValue = IntToFloat(FloatToInt(m_confuseValue) ^ FloatToInt(m_randomValue));
#ifdef _DEBUG
	assert(actualValue == m_actualValue);
#endif
	return actualValue;
}

template<>
void secure_number<float>::Init()
{
	m_randomValue  = IntToFloat(rand() + 341); // 341 = (101010101)2
	m_confuseValue = m_randomValue;
#ifdef _DEBUG
	m_actualValue  = 0.0f;
#endif
}

template<>
void secure_number<float>::Set(float val)
{
	m_confuseValue = IntToFloat(FloatToInt(m_randomValue) ^ FloatToInt(val));
#ifdef _DEBUG
	m_actualValue  = val;
#endif
}

