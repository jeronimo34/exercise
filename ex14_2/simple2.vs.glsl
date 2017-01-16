in vec4 vPosition;
in vec4 vColor;

out vec4 Color;

void main()
{
	Color = vColor;
	gl_Position = vPosition;
}