#include "Common.h"

std::string toString(TextureFactoryType textureFactoryType) {
    switch (textureFactoryType) {
        CASE_STR(TextureFactoryType::TEXTURE_2D_LOADED);
        CASE_STR(TextureFactoryType::TEXTURE_CUBE_LOADED);
        CASE_STR(TextureFactoryType::TEXTURE_COMMON_COLOR_ATTACHMENT);
        CASE_STR(TextureFactoryType::TEXTURE_COMMON_DEPTH_ATTACHMENT);
        CASE_STR(TextureFactoryType::TEXTURE_MSAA_COLOR_ATTACHMENT);
        CASE_STR(TextureFactoryType::TEXTURE_GEOMETRY_POSITION_COLOR_ATTACHMENT);
        CASE_STR(TextureFactoryType::TEXTURE_GEOMETRY_NORMAL_COLOR_ATTACHMENT);
        CASE_STR(TextureFactoryType::TEXTURE_GEOMETRY_ALBEDO_SPEC_COLOR_ATTACHMENT);
        CASE_STR(TextureFactoryType::TEXTURE_SSAO_COLOR_ATTACHMENT);
        CASE_STR(TextureFactoryType::TEXTURE_SSAO_NOISE);
        default:
            return "UNKNOWN";
    }
}

std::string toString(FrameBufferFactoryType frameBufferFactoryType) {
    switch (frameBufferFactoryType) {
        CASE_STR(FrameBufferFactoryType::FRAME_BUFFER_SHADOW_MAP);
        CASE_STR(FrameBufferFactoryType::FRAME_BUFFER_MSAA);
        CASE_STR(FrameBufferFactoryType::FRAME_BUFFER_INTERMEDIATE);
        CASE_STR(FrameBufferFactoryType::FRAME_BUFFER_PING_PONG);
        CASE_STR(FrameBufferFactoryType::FRAME_BUFFER_GEOMETRY);
        CASE_STR(FrameBufferFactoryType::FRAME_BUFFER_SSAO);
        CASE_STR(FrameBufferFactoryType::FRAME_BUFFER_SSAO_BLUR);
        default:
            return "UNKNOWN";
    }
}