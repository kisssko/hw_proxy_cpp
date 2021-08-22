
#ifndef __HW_PROXY_HPP__
#define __HW_PROXY_HPP__

enum hw_proxy_access
{
	NONE	= 0x00,
	RD		= 0x01,
	WR		= 0x02,
	RDWR	= 0x03
};

 // Data [T]ype, [U]nit Type, [A]ccess, [R]eader, [W]riter
template <typename T, typename U, hw_proxy_access A,
	T(*R)(uintptr_t), void(*W)(uintptr_t,T)>
class HW_PROXY
{
	private:
	typedef union
	{
		void		*p;
		uintptr_t	 u;
	} __uniptr_t;

	U __dummy__;

	T __getit__()
	{
		__uniptr_t t = {.p = (void*)this};
		T        v = 0;
		if(A & RD)
			v = R(t.u);
		else
			puts("Read access denied!");
		return v;
	}

	T __setit__(T v)
	{
		__uniptr_t t = {.p = (void*)this};
		if(A & WR)
			W(t.u, v);
		else
			puts("Write access denied!");
		return v;
	}

	public:
	T operator=  (T   v) {return __setit__(v);}
	  operator T (     ) {return __getit__();}
	T operator() (     ) {return __getit__();}
	T operator++ (     ) {return __setit__(__getit__()  + 1);}
	T operator+= (T   v) {return __setit__(__getit__()  + v);}
	T operator-= (T   v) {return __setit__(__getit__()  - v);}
	T operator*= (T   v) {return __setit__(__getit__()  * v);}
	T operator/= (T   v) {return __setit__(__getit__()  / v);}
	T operator%= (T   v) {return __setit__(__getit__()  % v);}
	T operator&= (T   v) {return __setit__(__getit__()  & v);}
	T operator|= (T   v) {return __setit__(__getit__()  | v);}
	T operator^= (T   v) {return __setit__(__getit__()  ^ v);}
	T operator<<=(T   v) {return __setit__(__getit__() << v);}
	T operator>>=(T   v) {return __setit__(__getit__() >> v);}
	T operator++ (int v)
	{
		T t = __getit__();
		__setit__(t + 1);
		return t;
	}
};

#endif
