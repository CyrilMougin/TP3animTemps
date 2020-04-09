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
//uniform sampler2DShadow
uniform sampler2D DepthTexture;
uniform mat4 MV;
uniform vec3 LightPosition_worldspace;

float near = 0.01; 
float far  = 100; 

float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main(){
	
	// https://stackoverflow.com/questions/48091045/accessing-a-gl-depth-component-texture
	float zdepth = LinearizeDepth(texelFetch(DepthTexture, ivec2(gl_FragCoord.xy), 0).r);
	float z = LinearizeDepth(gl_FragCoord.z);
	if (zdepth > z - .05) discard;
	else color = vec4(1, 0, 0, 0);			
}
  
