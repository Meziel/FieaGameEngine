#include <DefaultHash.h>

namespace FieaGameEngine
{
	template <typename TKey>
	inline size_t DefaultHash<TKey>::operator()(const TKey & key)
	{
		const size_t PRIME = 29;
		size_t hashValue = 0;
		const uint8_t* keyAsBytes = reinterpret_cast<const uint8_t*>(&key);
		for (size_t i = 0; i < sizeof(TKey); ++i)
		{
			hashValue += static_cast<size_t>(keyAsBytes[i]) * PRIME;
		}
		return hashValue;
	}

	inline size_t DefaultHash<const char* const>::operator()(const char* key)
	{
		const size_t PRIME = 29;
		size_t hashValue = 0;
		for (size_t i = 0; i < strlen(key); ++i)
		{
			hashValue += static_cast<const size_t>(key[i]) * PRIME;
		}
		return hashValue;
	}

	inline size_t DefaultHash<char* const>::operator()(const char* key)
	{
		const size_t PRIME = 29;
		size_t hashValue = 0;
		for (size_t i = 0; i < strlen(key); ++i)
		{
			hashValue += static_cast<const size_t>(key[i]) * PRIME;
		}
		return hashValue;
	}

	inline size_t DefaultHash<const char*>::operator()(const char* key)
	{
		const size_t PRIME = 29;
		size_t hashValue = 0;
		for (size_t i = 0; i < strlen(key); ++i)
		{
			hashValue += static_cast<const size_t>(key[i]) * PRIME;
		}
		return hashValue;
	}

	inline size_t DefaultHash<char*>::operator()(const char* key)
	{
		const size_t PRIME = 29;
		size_t hashValue = 0;
		for (size_t i = 0; i < strlen(key); ++i)
		{
			hashValue += static_cast<const size_t>(key[i]) * PRIME;
		}
		return hashValue;
	}
	
	inline size_t DefaultHash<const std::string>::operator()(const std::string& key) {
		const size_t PRIME = 29;
		size_t hashValue = 0;
		for (size_t i = 0; i < key.length(); ++i) {
			hashValue += static_cast<size_t>(key[i]) * PRIME;
		}
		return hashValue;
	}

	inline size_t DefaultHash<std::string>::operator()(const std::string& key) {
		const size_t PRIME = 29;
		size_t hashValue = 0;
		for (size_t i = 0; i < key.length(); ++i) {
			hashValue += static_cast<size_t>(key[i]) * PRIME;
		}
		return hashValue;
	}
}