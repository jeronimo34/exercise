in vec4 oNormal;
in vec4 oPosition;
out vec4 FragColor;

uniform vec4 lightPosition; 
uniform vec4 materialColor;

void main(){
	vec4 V = oPosition;
	vec4 L = normalize(lightPosition - V);
	vec4 N = oNormal;

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
	FragColor = diffuse + specular;
}
