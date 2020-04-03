#version 330

// Input Variables (received from Vertex Shader)
in vec4 color;

// Output Variable (sent down through the Pipeline)
out vec4 outColor;

// Debug Text - Do Not Change!
uniform int Text = 0;

void main(void) 
{
	outColor = color;



	// Debug Text - Do Not Change! - Leave it as the Last Line of Code here... 
	if (Text == 1) outColor = vec4(1, 0.8, 0, 1);
}
