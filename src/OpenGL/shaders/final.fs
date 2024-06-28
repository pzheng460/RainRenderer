#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D hdrBuffer;
uniform bool HDRActive;
uniform float exposure;

uniform sampler2D bloomBlur;
uniform bool bloomActive;

void main()
{             
    const float gamma = 2.2;
    vec3 hdrColor = texture(hdrBuffer, TexCoords).rgb;
    vec3 bloomColor = texture(bloomBlur, TexCoords).rgb;
    if (bloomActive)
        hdrColor += bloomColor; // additive blending
    if (HDRActive)
    {
        // reinhard
        // vec3 result = hdrColor / (hdrColor + vec3(1.0));
        // exposure
        vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
        // also gamma correct while we're at it       
//         result = pow(result, vec3(1.0 / gamma));
        FragColor = vec4(result, 1.0);
    }
    else
    {
        vec3 result = hdrColor;
//         vec3 result = pow(hdrColor, vec3(1.0 / gamma));
        FragColor = vec4(hdrColor, 1.0);
    }
}