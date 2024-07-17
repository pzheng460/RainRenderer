#include "Common.h"

std::string toString(TextureFactoryType textureFactoryType) {
    switch (textureFactoryType) {
        CASE_STR(TextureFactoryType::TEXTURE_2D_LOADED);
        CASE_STR(TextureFactoryType::TEXTURE_2D_HDR_LOADED);
        CASE_STR(TextureFactoryType::TEXTURE_CUBE_LOADED);
        CASE_STR(TextureFactoryType::TEXTURE_CUBE_MAP_SKYBOX);
        CASE_STR(TextureFactoryType::TEXTURE_COMMON_COLOR_ATTACHMENT);
        CASE_STR(TextureFactoryType::TEXTURE_COMMON_DEPTH_ATTACHMENT);
        CASE_STR(TextureFactoryType::TEXTURE_MSAA_COLOR_ATTACHMENT);
        CASE_STR(TextureFactoryType::TEXTURE_GEOMETRY_POSITION_COLOR_ATTACHMENT);
        CASE_STR(TextureFactoryType::TEXTURE_GEOMETRY_NORMAL_COLOR_ATTACHMENT);
        CASE_STR(TextureFactoryType::TEXTURE_GEOMETRY_ALBEDO_SPEC_COLOR_ATTACHMENT);
        CASE_STR(TextureFactoryType::TEXTURE_SSAO_COLOR_ATTACHMENT);
        CASE_STR(TextureFactoryType::TEXTURE_SSAO_NOISE);
        CASE_STR(TextureFactoryType::TEXTURE_IRRADIANCE);
        CASE_STR(TextureFactoryType::TEXTURE_PREFILTER);
        CASE_STR(TextureFactoryType::TEXTURE_BRDF_LUT);
        default:
            return "UNKNOWN";
    }
}

std::string toString(FrameBufferFactoryType frameBufferFactoryType) {
    switch (frameBufferFactoryType) {
        CASE_STR(FrameBufferFactoryType::FRAME_BUFFER_SHADOW_MAP);
        CASE_STR(FrameBufferFactoryType::FRAME_BUFFER_MSAA);
        CASE_STR(FrameBufferFactoryType::FRAME_BUFFER_INTERMEDIATE);
        CASE_STR(FrameBufferFactoryType::FRAME_BUFFER_BLOOM);
        CASE_STR(FrameBufferFactoryType::FRAME_BUFFER_GEOMETRY);
        CASE_STR(FrameBufferFactoryType::FRAME_BUFFER_SSAO);
        CASE_STR(FrameBufferFactoryType::FRAME_BUFFER_SSAO_BLUR);
        CASE_STR(FrameBufferFactoryType::FRAME_BUFFER_SKYBOX_CAPTURE);
        default:
            return "UNKNOWN";
    }
}

std::string toString(ShaderFactoryType shaderFactoryType) {
    switch (shaderFactoryType) {
        CASE_STR(ShaderFactoryType::SHADER_LIGHT);

        CASE_STR(ShaderFactoryType::SHADER_SKYBOX);
        CASE_STR(ShaderFactoryType::SHADER_SKYBOX_HDR);
        CASE_STR(ShaderFactoryType::SHADER_SPHERE_MAP_TO_CUBE_MAP);
        CASE_STR(ShaderFactoryType::SHADER_IRRADIANCE_MAP);
        CASE_STR(ShaderFactoryType::SHADER_PREFILTER_MAP);
        CASE_STR(ShaderFactoryType::SHADER_BRDF_LUT);

        CASE_STR(ShaderFactoryType::SHADER_SHADOW_MAP);
        CASE_STR(ShaderFactoryType::SHADER_SHADOW_MAP_DEBUG);

        CASE_STR(ShaderFactoryType::SHADER_BASIC);
        CASE_STR(ShaderFactoryType::SHADER_PHONG);
        CASE_STR(ShaderFactoryType::SHADER_BLINN_PHONG);
        CASE_STR(ShaderFactoryType::SHADER_DEPTH_TESTING);
        CASE_STR(ShaderFactoryType::SHADER_ENVIRONMENT_MAPPING);
        CASE_STR(ShaderFactoryType::SHADER_PBR);
        CASE_STR(ShaderFactoryType::SHADER_NORMAL_VISUALIZATION);

        CASE_STR(ShaderFactoryType::SHADER_BLOOM);
        CASE_STR(ShaderFactoryType::SHADER_POST_PROCESSING);

        CASE_STR(ShaderFactoryType::SHADER_GEOMETRY);
        CASE_STR(ShaderFactoryType::SHADER_DEFERRED_LIGHTING);
        CASE_STR(ShaderFactoryType::SHADER_SSAO);
        CASE_STR(ShaderFactoryType::SHADER_SSAO_BLUR);

        default:
            return "UNKNOWN";
    }
}