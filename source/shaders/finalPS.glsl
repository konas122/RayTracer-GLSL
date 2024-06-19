#version 330 core

in vec2 screenCoord;

out vec4 FragColor;

uniform int sampleCount;
uniform sampler2D scene;
uniform sampler2D prevScene;


vec3 GammaCorrection(vec3 c) {
	return pow(c, vec3(1.0 / 2.2));
}


vec3 InverseGammaCorrection(vec3 c) {
	return pow(c, vec3(2.2));
}


void main() {
    vec3 color = texture(scene, screenCoord).xyz;

    vec3 prevColor = texture(prevScene, screenCoord).xyz;
    // prevColor = InverseGammaCorrection(prevColor);
    prevColor *= (sampleCount - 1);

    color += prevColor;
    color /= sampleCount;

    color = GammaCorrection(color);
    FragColor.xyz = color;
    FragColor.w = 1.0;
}
