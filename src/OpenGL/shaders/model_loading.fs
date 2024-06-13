#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

// uniform sampler2D texture_diffuse1;

struct Material {
    sampler2D texture_diffuse1; // 漫反射贴图
    sampler2D texture_specular1; // 镜面光贴图
    sampler2D texture_normal1; // 法线贴图
    sampler2D texture_height1; // 高度贴图
    float shininess; // 反光度
};

uniform Material material;

void main()
{    
    FragColor = texture(material.texture_diffuse1, TexCoords);
}