#ifndef UTILS_H
#define UTILS_H

#pragma once

#include "Texture.h"

Texture* loadTexture2D(const std::string& path, bool flip = false);

Texture* loadTexture2DHDR(const std::string& path, bool flip = true);

Texture* loadTextureCube(const std::vector<std::string>& paths, bool flip = false);

#endif // UTILS_H
