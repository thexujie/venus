#pragma once

VENUS_BEG

template<typename ObjT>
class CORE_API object_ptr
{
public:
	object_ptr() :ptr(nullptr) {}
	explicit object_ptr(ObjT * _ptr) :ptr(_ptr) {}
	object_ptr(const object_ptr & another) : ptr(another.ptr) { if(ptr) ptr->AddRef(); }
	~object_ptr() { if(ptr) { ptr->Release(); ptr = nullptr; } }

	ObjT * operator->() const { return ptr; }
	operator ObjT * () const { return ptr; }


	object_ptr & operator = (ObjT * _ptr)
	{
		SafeRelease(ptr);
		ptr = _ptr;
		SafeAddRef(ptr);
		return *this;
	}

	object_ptr & operator = (const object_ptr & another)
	{
		if(ptr)
			ptr->Release();
		ptr = another.ptr;
		if(ptr)
			ptr->AddRef();
		return *this;
	}

	void attach(ObjT * _ptr)
	{
		if(ptr)
			ptr->Release();
		ptr = _ptr;
	}
	void detach()
	{
		if(ptr)
			ptr = nullptr;
	}

	bool operator == (const object_ptr & another) const { return ptr == another.ptr; }
	bool operator != (const object_ptr & another) const { return ptr != another.ptr; }

	ObjT * ptr;
};

template<typename ObjT>
object_ptr<ObjT> object_ref(ObjT * ptr)
{
	if(ptr)
		ptr->AddRef();
	return object_ptr<ObjT>(ptr);
}

VENUS_END
