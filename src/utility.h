#pragma once

namespace atom
{
    consteval size_t get_string_length(const char* str)
    {
        size_t length = 0;
        while (str[length] != '\0')
        {
            length++;
        }
        return length;
    }

    consteval uint32_t fnv1a(const char* key, uint32_t seed = 0)
    {
        constexpr uint32_t fnv_prime = 16777619;
        uint32_t hash = seed;

        for (uint32_t i = 0; i < get_string_length(key); i++)
        {
            hash ^= static_cast<uint8_t>(key[i]);
            hash *= fnv_prime;
        }

        return hash;
    }

    uint32_t fnv1a(const std::string& key, uint32_t seed = 0)
    {
        constexpr uint32_t fnv_prime = 16777619;
        uint32_t hash = seed;

        for (uint32_t i = 0; i < key.size(); i++)
        {
            hash ^= static_cast<uint8_t>(key[i]);
            hash *= fnv_prime;
        }

        return hash;
    }

    consteval uint32_t operator""_h(const char* key, size_t length)
    {
        return fnv1a(key);
    }

	static_assert("atomic_engine"_h == 3024981448, "Hash of \"atomic_engine\" is not 3024981448");
}
