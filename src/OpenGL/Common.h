#ifndef COMMON_H
#define COMMON_H

#pragma once

#define CASE_STR(x) case x : return #x; break;

#include <glad/glad.h>
#include <iostream>

// settings 窗口设置
constexpr unsigned int SCR_WIDTH = 1280;
constexpr unsigned int SCR_HEIGHT = 720;

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum class Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
constexpr float YAW         = -90.0f;
constexpr float PITCH       =  0.0f;
constexpr float ROLL        =  0.0f;
constexpr float SPEED       =  2.5f;
constexpr float SENSITIVITY =  0.1f;
constexpr float ZOOM        =  45.0f;

constexpr int MAX_BONE_INFLUENCE = 4;

constexpr const char* DEFAULT_FILE_PATH = "resources/objects/YYB Symphony Miku by HB-Squiddy/yyb Symphony Miku by HB-Squiddy.pmx";

enum class RenderingPath {
    FORWARD_RENDERING,
    DEFERRED_RENDERING
};

enum class RenderMode {
    BASIC,
    PHONG,
    BLINN_PHONG,
    DEPTH,
    ENVIRONMENT_MAPPING,
    PBR
};

enum class SkyboxLoadMode {
    CUBE_MAP,
    SPHERE_MAP
};

enum class GeometryType {
    MODEL_MESH,
    SPHERE,
    CUBE,
    QUAD,
    PLANE
};

enum class TextureType {
    TEXTURE_2D,
    TEXTURE_2D_MULTISAMPLE,
    TEXTURE_CUBE_MAP,
};

enum class TextureFactoryType {
    TEXTURE_2D_LOADED,
    TEXTURE_2D_HDR_LOADED,
    TEXTURE_CUBE_LOADED,
    TEXTURE_CUBE_MAP_SKYBOX,
    TEXTURE_COMMON_COLOR_ATTACHMENT,
    TEXTURE_COMMON_DEPTH_ATTACHMENT,
    TEXTURE_MSAA_COLOR_ATTACHMENT,
    TEXTURE_GEOMETRY_POSITION_COLOR_ATTACHMENT,
    TEXTURE_GEOMETRY_NORMAL_COLOR_ATTACHMENT,
    TEXTURE_GEOMETRY_ALBEDO_SPEC_COLOR_ATTACHMENT,
    TEXTURE_SSAO_COLOR_ATTACHMENT,
    TEXTURE_SSAO_NOISE,
    TEXTURE_IRRADIANCE,
    TEXTURE_PREFILTER,
    TEXTURE_BRDF_LUT,
};

std::string toString(TextureFactoryType textureFactoryType);

constexpr const char* DIFFUSE_NAME = "material.texture_diffuse1";
constexpr const char* SPECULAR_NAME = "material.texture_specular1";
constexpr const char* NORMAL_NAME = "material.texture_normal1";
constexpr const char* HEIGHT_NAME = "material.texture_height1";

constexpr const char* PBR_ALBEDO_NAME = "material.albedoMap";
constexpr const char* PBR_NORMAL_NAME = "material.normalMap";
constexpr const char* PBR_METALLIC_NAME = "material.metallicMap";
constexpr const char* PBR_ROUGHNESS_NAME = "material.roughnessMap";
constexpr const char* PBR_AO_NAME = "material.aoMap";
constexpr const char* PBR_EMISSION_NAME = "material.emissionMap";
constexpr const char* PBR_IRRADIANCE_NAME = "irradianceMap";
constexpr const char* PBR_PREFILTER_NAME = "prefilterMap";
constexpr const char* PBR_BRDF_LUT_NAME = "brdfLUT";

enum class RenderBufferObjectType {
    RENDER_BUFFER_OBJECT,
    RENDER_BUFFER_OBJECT_MULTI_SAMPLE
};

enum class RenderBufferObjectFactoryType {
    RENDER_BUFFER_OBJECT_DEPTH,
    RENDER_BUFFER_OBJECT_DEPTH_MULTI_SAMPLE,
    RENDER_BUFFER_OBJECT_DEPTH_SKYBOX,
};

enum class FrameBufferFactoryType {
    FRAME_BUFFER_DEFAULT,
    FRAME_BUFFER_SHADOW_MAP,
    FRAME_BUFFER_MSAA,
    FRAME_BUFFER_INTERMEDIATE,
    FRAME_BUFFER_BLOOM,
    FRAME_BUFFER_GEOMETRY,
    FRAME_BUFFER_SSAO,
    FRAME_BUFFER_SSAO_BLUR,
    FRAME_BUFFER_SKYBOX_CAPTURE,
};

std::string toString(FrameBufferFactoryType frameBufferFactoryType);

enum class ShaderFactoryType {
    SHADER_LIGHT,

    SHADER_SKYBOX,
    SHADER_SKYBOX_HDR,
    SHADER_SPHERE_MAP_TO_CUBE_MAP,
    SHADER_IRRADIANCE_MAP,
    SHADER_PREFILTER_MAP,
    SHADER_BRDF_LUT,

    SHADER_SHADOW_MAP,
    SHADER_SHADOW_MAP_DEBUG,

    SHADER_BASIC,
    SHADER_PHONG,
    SHADER_BLINN_PHONG,
    SHADER_DEPTH_TESTING,
    SHADER_ENVIRONMENT_MAPPING,
    SHADER_PBR,
    SHADER_NORMAL_VISUALIZATION,

    SHADER_BLOOM,
    SHADER_POST_PROCESSING,

    SHADER_GEOMETRY,
    SHADER_DEFERRED_LIGHTING,
    SHADER_SSAO,
    SHADER_SSAO_BLUR,
};

std::string toString(ShaderFactoryType shaderFactoryType);

#endif // COMMON_H
