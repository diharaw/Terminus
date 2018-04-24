#pragma once

#include <core/include/terminus_macros.hpp>
#include <core/include/terminus_types.hpp>
#include <stl/include/string_buffer.hpp>
#include <stl/include/sparse_array.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

using ResourceID = uint32_t;

#define INVALID_RESOURCE_ID UINT32_MAX

template <typename T>
class Handle;

template <typename T>
class ResourceCache
{
public:
	ResourceCache()
	{

	}

	~ResourceCache()
	{

	}

	Handle<T> load(const StringBuffer64& name)
	{
		ResourceID id = load_internal(name);

		if (id == INVALID_RESOURCE_ID)
			return Handle<T>();

		return Handle<T>(id, this);
	}

	bool unload(const ResourceID& id)
	{
		if (id != INVALID_RESOURCE_ID)
		{

		}
		else
			return false;
	}

	T* lookup(const ResourceID& id)
	{
		if (id != INVALID_RESOURCE_ID)
		{

		}
		else
			return nullptr;
	}

	void increment_ref(const ResourceID& id)
	{
		if (id != INVALID_RESOURCE_ID)
		{

		}
	}

	void decrement_ref(const ResourceID& id)
	{
		if (id != INVALID_RESOURCE_ID)
		{

		}
	}

protected:
	virtual ResourceID load_internal(const StringBuffer64& name) = 0;

protected:
	struct ResourceEntry
	{
		T*		 resource;
		uint32_t ref_count;
		bool	 persistent;
	};
};

template <typename T>
class Handle
{
public:
	Handle() : m_id(INVALID_RESOURCE_ID), m_resource_cache(nullptr)
	{

	}

	Handle(ResourceID id, ResourceCache<T>* res_mgr) : m_id(id), m_resource_cache(res_mgr)
	{
		m_resource_cache->increment_ref(m_id);
	}

	Handle(const Handle<T>& other) : m_id(other.m_id), m_resource_cache(other.m_resource_cache)
	{
		m_resource_cache->increment_ref(m_id);
	}

	~Handle()
	{
		unload();
	}

	operator bool() const
	{
		return m_id != INVALID_RESOURCE_ID && get() != nullptr;
	}

	T* operator -> () const
	{
		return get();
	}

	Handle<T>& operator = (const Handle<T>& other)
	{
		m_resource_cache = other.m_resource_cache;
		m_id = other.m_id;
		m_resource_cache->increment_ref(m_id);

		return *this;
	}

	T* get() const
	{
		return m_resource_cache->lookup(m_id);
	}

	ResourceID id() const
	{
		return m_id;
	}

	void unload()
	{
		if (m_resource_cache)
		{
			if (m_resource_cache->unload(m_id))
				m_id = INVALID_RESOURCE_ID;
		}
	}

private:
	ResourceID		  m_id;
	ResourceCache<T>* m_resource_cache;
};


TE_END_TERMINUS_NAMESPACE