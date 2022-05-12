//Version number
#version 330 core

//Layout qualfier
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

//Passing in texture coordinates
in VS_OUT {
    vec2 texCoords;
} gs_in[];

//Passes out the texture coord
out vec2 TexCoords; 

//uniform variable used for linked to the main.cpp file.
uniform float time;


vec4 explode(vec4 position, vec3 normal)
{
    // the magnitude of the explosion is scale of the over all effect
    float magnitude = 0.6;
	//Direction on the explosion being applied across the normal
    vec3 direction = normal * ((sin(time) + 1.0) / 2) * magnitude; 
	//returns the position plus the vec4 direction.
    return position + vec4(direction, 0.0);
}

vec3 GetNormal()
{
//each vertex's normal is calculated in the below code.
   vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
   vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
   //returns the cross product between the two vectors calculated
   return normalize(cross(a, b));
}

void main()
{
//Gets the normal of the from the above method.
    vec3 normal = GetNormal();
//sets up the vertex position and keeps the textcoords to the position.
    gl_Position = explode(gl_in[0].gl_Position, normal);
    TexCoords = gs_in[0].texCoords;
    EmitVertex();
    gl_Position = explode(gl_in[1].gl_Position, normal);
    TexCoords = gs_in[1].texCoords;
    EmitVertex();
    gl_Position = explode(gl_in[2].gl_Position, normal);
    TexCoords = gs_in[2].texCoords;
    EmitVertex();
    EndPrimitive();
}  