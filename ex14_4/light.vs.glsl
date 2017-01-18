in vec4 vPosition; 
in vec4 vNormal;

out vec4 oNormal;
out vec4 oPosition;

uniform mat4 MVMatrix; // Model View 
uniform mat4 NormalMatrix;
uniform mat4 PMatrix; // Projection 



void main()
{
	gl_Position = PMatrix * MVMatrix * vPosition;

	//Diffuse component:
	vec4 N = NormalMatrix * vNormal;
	N.w = 0.0;
	oNormal = normalize(N);
	oPosition = MVMatrix * vPosition;


}