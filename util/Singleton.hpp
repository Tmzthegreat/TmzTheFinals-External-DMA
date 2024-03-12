#pragma once

template<typename T>
class Singleton {
protected:
	Singleton() { }
	~Singleton() { }

	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;

	Singleton(Singleton&&) = delete;
	Singleton& operator=(Singleton&&) = delete;
public:
	static T& i() { // returns instance
		static T inst{};
		return inst;
	}
};