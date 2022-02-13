#pragma once
#include <memory>
#include <iostream>

struct Counters {
	size_t uses = 1;
	size_t weaks = 1;
};

template <typename T>
class WeakPtr;

template <typename T>
class SharedPtr {
public:
	SharedPtr() = default;

	SharedPtr(T* ptr)
	{
		if (ptr == nullptr) {
			return;
		}

		_ptr = ptr;
		_counters = new Counters;
	}

	~SharedPtr()
	{
		if (_ptr == nullptr) {
			return;
		}

		if (--_counters->uses == 0) {
			delete _ptr;

			if (--_counters->weaks == 0) {
				delete _counters;
			}
		}
	}

	SharedPtr(const SharedPtr<T>& right)
	{
		if (this == &right) {
			return;
		}
	
		++right._counters->uses;

		_ptr		= right._ptr;
		_counters	= right._counters;
	}

	SharedPtr<T>& operator=(const SharedPtr<T>& right)
	{
		if (this == &right) {
			return *this;
		}

		--_counters->uses;
		SharedPtr<T> temp(*this);

		++right._counters->uses;

		_ptr = right._ptr;
		_counters = right._counters;

		return *this;
	}
	
	SharedPtr(SharedPtr<T>&& right)
	{
		if (this == &right) {
			return;
		}

		_ptr = right._ptr;
		_counters = right._counters;

		right._ptr = nullptr;
		right._counters = nullptr;
	}

	SharedPtr<T>& operator=(SharedPtr<T>&& right)
	{
		if (this == &right) {
			return *this;
		}

		--_counters->uses;
		SharedPtr<T> temp(*this);
		
		_counters = right._counters;
		_ptr = right._ptr;

		right._ptr = nullptr;
		right._counters = nullptr;

		return *this;
	}

	SharedPtr(const WeakPtr<T>& weak) 
	{
		if (weak.expired()) {
			return;
		}

		++weak._counters->uses;

		_ptr = const_cast<T*>(weak._ptr);
		_counters = weak._counters;
	}

	T& operator*()
	{
		return *_ptr;
	}

	const T& operator*() const
	{
		return *_ptr;
	}

	T* operator->()
	{
		return _ptr;
	}

	const T* operator->() const
	{
		return _ptr;
	}

private:
	T* _ptr = nullptr;
	Counters* _counters = nullptr;

	friend class WeakPtr<T>;
};

struct MyType {
	~MyType()
	{
		std::cout << "~MyType()\n";
	}
};

template <typename T>
class WeakPtr {
public:
	WeakPtr(const SharedPtr<T>& shared)
		: _ptr(shared._ptr)
		, _counters(shared._counters)
	{
		++_counters->weaks;
	}

	WeakPtr(const WeakPtr<T>& right)
	{
		if (this == &right) {
			return;
		}

		++right._counters->weaks;
		_counters = right._counters;
	}

	WeakPtr<T>& operator=(const WeakPtr<T>& right)
	{
		if (this == &right) {
			return *this;
		}

		--_counters->weaks;
		WeakPtr<T> temp(*this);

		++right._counters->weaks;
		_counters = right._counters;

		return *this;
	}

	WeakPtr(WeakPtr<T>&& right)
	{
		if (this == &right) {
			return;
		}

		_counters = right._counters;
		right._counters = nullptr;
	}

	WeakPtr<T>& operator=(WeakPtr<T>&& right)
	{
		if (this == &right) {
			return *this;
		}

		--_counters->weaks;
		WeakPtr<T> temp(*this);

		_counters = right._counters;
		right._counters = nullptr;

		return *this;
	}

	bool expired() const
	{
		return _counters->uses == 0;
	}

	~WeakPtr()
	{
		if (_counters == nullptr) {
			return;
		}

		if (--_counters->weaks == 0) {
			delete _counters;
		}
	}

	SharedPtr<T> lock() const
	{
		return expired() ? SharedPtr<T>() : SharedPtr<T>(*this);
	}

private:

	const T* const _ptr = nullptr;
	Counters* _counters = nullptr;

	friend class SharedPtr<T>;
};

void testSharedPtr()
{
	SharedPtr<MyType> pInt(new MyType);
	WeakPtr<MyType> wp(pInt);

	auto sharedFromWeak = wp.lock();
	{
		auto copy = pInt;
		auto copy2 = copy;

		SharedPtr<MyType> p2(new MyType);
		p2 = copy2;

		SharedPtr<MyType> p3(new MyType);
		p3 = std::move(pInt);

	}
	wp.expired() ? std::cout << "Expired\n" : std::cout << "Alive\n";
}

 