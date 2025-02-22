#include "texture.h"

namespace atom
{
    c_texture::c_texture(const std::string& file_path)
    {
        if (!m_texture.loadFromFile(file_path))
        {
            throw std::runtime_error("Failed to load texture: " + file_path);
        }
    }
} // namespace atom
