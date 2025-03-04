#pragma once
#define NOMINMAX

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <Windows.h>
#include <memory>
#include <format>
#include <ostream>
#include <sstream>

namespace atom
{
    constexpr size_t get_string_length(const char* str)
    {
        size_t length = 0;
        while (str[length] != '\0')
        {
            length++;
        }
        return length;
    }

    inline constexpr uint32_t fnv1a(const char* key, uint32_t seed = 0)
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

    inline uint32_t fnv1a(const std::string& key, uint32_t seed = 0)
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

    template<bool Debug = true, bool Line = false, bool Disabled = false, std::ostream&... ConstOstreams>
    class c_logger
    {
    public:
        template<typename... Args>
        c_logger(Args... ostreams)
            : m_ostreams{ ostreams... }
        {
        }

        template<typename... Args>
        c_logger(char const* file, Args... ostreams) : m_ostreams{ ostreams... }
        {
            if (file != nullptr)
            {
                m_file_stream = std::make_unique<std::ofstream>(file);
            }
        }

        ~c_logger() = default;

        template<uint32_t... Tags>
        inline void enable()
        {
            (m_tags.push_back(Tags), ...);
        }

        inline void disable(uint32_t tag)
        {
            m_tags.erase(std::remove(m_tags.begin(), m_tags.end(), tag), m_tags.end());
        }

        template<uint32_t... Tags>
        inline void disable()
        {
            (disable(Tags), ...);
        }

        template<typename... Args>
        inline void operator()(std::format_string<Args...> fmt, Args&&... args) const
        {
            if constexpr (Disabled)
            {
                return;
            }

            std::string message = std::format(fmt, std::forward<Args>(args)...);
            if constexpr (Line)
            {
                message += '\n';
            }

            if constexpr (Debug)
            {
                OutputDebugStringA(message.c_str());
            }

            std::ostringstream batch_output;
            batch_output << message;

            ((ConstOstreams << batch_output.str()), ...);
            for (auto& os : m_ostreams)
            {
                *os << batch_output.str();
            }

            if (m_file_stream != nullptr)
            {
                *m_file_stream << batch_output.str();
            }
        }

        template<typename... Args>
        inline void operator()(std::vector<uint32_t> const&& tags, std::format_string<Args...> fmt, Args&&... args) const
        {
            if constexpr (Disabled)
            {
                return;
            }

            bool tag_found = false;
            for (auto tag : m_tags)
            {
                if (std::find(tags.begin(), tags.end(), tag) == tags.end())
                {
                    continue;
                }

                tag_found = true;
                break;
            }

            if (!tag_found)
            {
                return;
            }

            operator()(fmt, std::forward<Args>(args)...);
        }

    private:
        std::vector<std::ostream*> m_ostreams;
        std::unique_ptr<std::ofstream> m_file_stream;
        std::vector<uint32_t> m_tags;
    };

    // Disable debug logging in release builds
#if defined NDEBUG
    extern c_logger<true, false, true> debug;
    extern c_logger<true, true, true> debugln;
#else
    extern c_logger<true, false> debug;
    extern c_logger<true, true> debugln;
#endif

    extern c_logger<true, false, false, std::cout> log;
    extern c_logger<true, true, false, std::cout> logln;
    extern c_logger<true, false, false, std::cerr> error;
    extern c_logger<true, true, false, std::cerr> errorln;

	template <typename T>
	struct t_vector2
	{
		T x;
		T y;
	};

	using s_vector2i32 = t_vector2<int32_t>;
	using s_vector2u8 = t_vector2<uint8_t>;
}
