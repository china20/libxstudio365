#ifndef _SECURE_NUMBER_H_
#define _SECURE_NUMBER_H_


template<typename T>
class secure_number
{
public:
	secure_number()
	{
		Init();
	}

	secure_number(const T val)
	{
		Init();
		Set(val);
	}

	secure_number(const secure_number& other)
	{
		if (this != &other)
		{
			Init();
			Set(other);
		}
	}

	T operator = (const T val)
	{
		Set(val);
		return val;
	}

	T operator = (const secure_number& other)
	{
		T val = (T)other;
		Set(val);
		return val;
	}

	operator T() const;

	T value() const
	{
		return (T)*this;
	}


	// overload math operators
	T operator + (const T val)
	{
		return (T)*this + val;
	}

	T operator += (const T val)
	{
		T valNew = (T)*this + val;
		Set(valNew);
		return valNew;
	}

	T operator - (const T val)
	{
		return (T)*this - val;
	}

	T operator -= (const T val)
	{
		T valNew = (T)*this - val;
		Set(valNew);
		return valNew;
	}

	T operator * (const T val)
	{
		return (T)*this * val;
	}

	T operator *= (const T val)
	{
		T valNew = (T)*this * val;
		Set(valNew);
		return valNew;
	}

	T operator / (const T val)
	{
		return (T)*this / val;
	}

	T operator /= (const T val)
	{
		T valNew = (T)*this / val;
		Set(valNew);
		return valNew;
	}

	// compare operators
	bool operator == (const T val) const
	{
		return (T)*this == val;
	}

	bool operator != (const T val) const
	{
		return (T)*this != val;
	}

	bool operator < (const T val) const
	{
		return (T)*this < val;
	}

	bool operator <= (const T val) const
	{
		return (T)*this <= val;
	}

	bool operator > (const T val) const
	{
		return (T)*this > val;
	}

	bool operator >= (const T val) const
	{
		return (T)*this >= val;
	}

private:
	void Init();
	void Set(T val);

private:
	T m_randomValue;
	T m_confuseValue;

	// for debug only
#ifdef _DEBUG
	T m_actualValue;
#endif
};

template<>
secure_number<int>::operator int() const;

template<>
secure_number<float>::operator float() const;


typedef secure_number<int>    SecureInt;
typedef secure_number<float>  SecureFloat;


#endif
