#version 400

uniform vec3 lightDirection;
varying vec3 normal;

void main()
{
	float intensity;
	vec4 color;
	intensity = dot(lightDir,normalize(normal));

	if (intensity > 1.0)
		color = vec4(1.0,0.5,0.5,0.31);
	else if (intensity > 0.64)
		color = vec4(0.6,0.3,0.3,1.0);
	else if (intensity > 0.35)
		color = vec4(0.4,0.2,0.2,1.0);
	else
		color = vec4(0.2,0.1,0.1,1.0);

	gl_FragColor = color;
}