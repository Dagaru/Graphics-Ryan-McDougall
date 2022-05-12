#version 400

layout( location = 0 ) out vec4 fragmentColour;
varying vec2 TexCoords;
uniform sampler2D watertexture;

void main()
{
    //assigns the texture to the fragment colour and uses the texcoords to apply the texture with proper scale and direction.
    fragmentColour = texture2D(watertexture, TexCoords);
}