#version 330 core

// Nom 1 : Olivier Perrault
// CIP 1 : pero2102
// Mat 1 : 16212377
// Nom 2 : Cyril Mougin
// CIP 2 : mouc2401
// Mat 2 : 19108350	

// Interpolated values from the vertex shaders
in vec2 UV;
in vec3 Position_worldspace;
in vec3 Normal_cameraspace;
in vec3 EyeDirection_cameraspace;
in vec3 LightDirection_cameraspace;

// Ouput data
out vec4 color;

// Values that stay constant for the whole mesh.
uniform sampler2D DepthTexture;
uniform sampler2D ColorTexture;
uniform mat4 MV;
uniform vec3 LightPosition_worldspace;


void main()
{
	color = vec4(1, 0, 0, 1);
	vec4 col = texture(ColorTexture, UV);	
	color = vec4(col.r/2, col.g/2, col.b/2, 1);
}