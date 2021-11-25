#include <iostream>

template <typename T>
struct ScopedPointer {
private:
	T *ptr;
public:
	ScopedPointer(const ScopedPointer &) = delete;
	ScopedPointer &operator=(const ScopedPointer &) = delete;

	ScopedPointer(T *ptr)
		: ptr(ptr) {}

	~ScopedPointer() {
		delete ptr;
	}

	T &operator*() {
		return *ptr;
	}
};

template <typename T>
struct has_deref_operator {
	
	typedef char one;
    struct two { char x[2]; };

    template <typename Y> 
	static one test( decltype(&Y::operator*));

    template <typename Y>
	static two test(...);

public:

	enum { 
		value = sizeof(test<T>(0)) == sizeof(char)
	};

};

template <bool val, typename T>
struct enable_if {
	typedef T type;
};

template <typename T>
struct enable_if<false, T> {
	
};


// TODO: implement this, without function overloading, use templates and SFINAE
template <typename T, bool val = has_deref_operator<T>::value>
typename enable_if<!val, void>::type deepCopy(T &dest,T &source) {
	dest = source;
}

template <typename T>
void deepCopy(T* dest, T* source) {
	*dest = *source;
}

template <typename T, bool val = has_deref_operator<T>::value>
typename enable_if<val, void>::type deepCopy(T &dest, T &source) {
	*dest = *source;
}

int main() {

	std::cout << has_deref_operator<int>::value << '\n';
	std::cout << has_deref_operator<ScopedPointer<int>>::value << '\n';
	std::cout << has_deref_operator<double*>::value << '\n';

	{
		ScopedPointer<int> x(new int{100}), y(new int{2});
		std::cout << "x = " << *x << " y = " << *y << std::endl;
		//deepCopy<int>(x, y);
		deepCopy(x, y);
		std::cout << "x = " << *x << " y = " << *y << std::endl;
	}

	{
		float x = 3.14f, y = 17.3f;
		std::cout << "x = " << x << " y = " << y << std::endl;
		deepCopy(x, y);
		std::cout << "x = " << x << " y = " << y << std::endl;
	}

	{
		double vals[2] = {100.1, 200.2};
		//double *x = &vals[0];
		//double *y = &vals[1];
		double * const x = &vals[0];
		double * const y = &vals[1]; 
		std::cout << "x = " << *x << " y = " << *y << std::endl;
		deepCopy(x, y);
		std::cout << "x = " << *x << " y = " << *y << std::endl;

		std::cout << "v[0]=" << vals[0] << "  v[1]=" << vals[1] << std::endl;
	}

	/* Expected output:
	 * x = 100 y = 2
	 * x = 2 y = 2
	 * x = 3.14 y = 17.3
	 * x = 17.3 y = 17.3
	 * x = 100.1 y = 200.2
	 * x = 200.2 y = 200.2
	 */

	return 0;
}