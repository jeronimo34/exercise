in vec4 vPosition; 
in vec4 vNormal;
out vec4 Color;

uniform mat4 MVMatrix; // Model View 
uniform mat4 NormalMatrix;
uniform mat4 PMatrix; // Projection 
uniform vec4 lightPosition; 
uniform vec4 materialColor;

uniform vec4 lightColor;

void main()
{
	gl_Position = PMatrix * MVMatrix * vPosition;

	//Diffuse component:
	vec4 N = NormalMatrix * vNormal;
	N.w = 0.0;
	N = normalize(N);

	vec4 V = MVMatrix * vPosition;
	vec4 L = normalize(lightPosition - V);

	float diff = max(0.0, dot(N, L));
	vec4 diffuse = materialColor * diff;

	//Specular Component:
	//Reflected light vector:
	vec4 R = reflect(-L, N);
	vec4 View = normalize(-V);

	float spec = max(dot(View, R), 0.0);
	float shiny = 16.0;
	spec = pow(spec, shiny);
	vec4 specular = vec4(1) * spec;

	//Ignore ambient component
	Color = diffuse + specular;
}