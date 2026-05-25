#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

out vec4 finalColor;

uniform sampler2D texture0; 
uniform vec2 resolution;
uniform vec2 playerPos;
uniform float radius;
uniform float softness;

void main()
{
    vec4 texColor = texture(texture0, fragTexCoord);
    vec2 pixelPos = fragTexCoord * resolution;
    float dist = distance(pixelPos, playerPos);

    float visibility = max(1.0 - smoothstep(radius - softness, radius, dist),0.1);

    finalColor = vec4(texColor.rgb * visibility, texColor.a);

}