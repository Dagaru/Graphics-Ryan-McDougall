#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
out vec3 Normal;

void main()
{
    //Normal ============================================
    //sets the normal to the same as the models normal.
    Normal = mat3(transpose(inverse(model))) * aNormal;  

    //Shader Position ======================================================================================================
    //sets the shaders postion to the MVP which is the projection of the camera,the camera position and the model position.
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    FragPos = vec3(model * vec4(aPos, 1.0));
} 