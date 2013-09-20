#version 150
uniform float time;
in vec4 vPosition ;

void main()
{
	//vPosition.x += (1+sin(time));
    gl_Position = vPosition;//vec4(1.0, 2.0, 300.0, 400.0);
	//color_out = vec4(1.0, 0.0, 0.0, 1.0);
}
