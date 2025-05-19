#pragma once

#include "RGS/Maths.h"

#include <string>

namespace RGS {

class Texture
{
public:
    Texture(const std::string& path);
    ~Texture();

    Vec4 Sample(Vec2 texCoords) const;  // 纹理采样

private:
    void Init();

private:
    int m_Width;
    int m_Height;
    int m_Channels;         
    std::string m_Path;
    Vec4* m_Data = nullptr;
};

}