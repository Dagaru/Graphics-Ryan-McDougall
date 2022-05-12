#version 330 core
out vec4 FragColor;
  
uniform vec3 ambientColor;
uniform vec3 lightColor;
uniform vec3 lightPos;  
uniform vec3 viewPos;

in vec3 FragPos; 
in vec3 Normal; 

void main()
{
    //Ambient Code ==================================================
    // creates a dim light applied to the object with a set colour.
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    //Diffuse ==========================================
    //the code below creates a light with a direction.
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);  

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    //Specular ==============================================
    //create a specular reflection from the light direction.
    float specularStrength = 2.5;
    vec3 v_Direction = normalize(viewPos - FragPos);
    vec3 r_Direction = reflect(-lightDir, norm);  

    float s = pow(max(dot(v_Direction, r_Direction), 0.0), 32);
    vec3 specular = specularStrength * s * lightColor;  

    //phong Shader ====================================================================
    //combines the ambient, diffuse and specular lighting to create the phong shader.
    vec3 phong = (ambient + diffuse + specular) * ambientColor;
    gl_FragColor = vec4(phong, 1.0);
}