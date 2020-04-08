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
out vec3 color;

// Values that stay constant for the whole mesh.
uniform sampler2D depthTexture;
uniform mat4 MV;
uniform vec3 LightPosition_worldspace;


void main(){

	// Light emission properties
	// You probably want to put them as uniforms
	vec3 LightColor = vec3(1,1,1);
	float LightPower = 50.0f;
	
	color = vec3(0, 1, 0);
	return;

//	float depth = gl_FragCoord.z;
	

	// Material properties
//    vec3 MaterialDiffuseColor = 
//		mod(floor(100.0f * UV.x) + floor(100.0f * UV.y), 2.0) == 0.0 ?
//			// Yellow
//			vec3(1, 1, 0) : 
//			// Blue
//			vec3(0, 0, 1);

    vec3 MaterialDiffuseColor = vec3(0, 0, 1);

	vec3 MaterialAmbientColor = vec3(0.1,0.1,0.1) * MaterialDiffuseColor;
	vec3 MaterialSpecularColor = vec3(0.3,0.3,0.3);

	// Distance to the light
	float distance = length( LightPosition_worldspace - Position_worldspace );

	// Normal of the computed fragment, in camera space
	vec3 n = normalize( Normal_cameraspace );
	// Direction of the light (from the fragment to the light)
	vec3 l = normalize( LightDirection_cameraspace );
	// Cosine of the angle between the normal and the light direction, 
	// clamped above 0
	//  - light is at the vertical of the triangle -> 1
	//  - light is perpendicular to the triangle -> 0
	//  - light is behind the triangle -> 0
	float cosTheta = clamp( dot( n,l ), 0,1 );
	
	// Eye vector (towards the camera)
	vec3 E = normalize(EyeDirection_cameraspace);
	// Direction in which the triangle reflects the light
	vec3 R = reflect(-l,n);
	// Cosine of the angle between the Eye vector and the Reflect vector,
	// clamped to 0
	//  - Looking into the reflection -> 1
	//  - Looking elsewhere -> < 1
	float cosAlpha = clamp( dot( E,R ), 0,1 );
		
//	float zdepth = texture(depthTexture, UV).r;
	
	color = vec3(0, 1, 0);
	return;

//	if (zdepth >= gl_FragCoord.z) return;	
//
//	color = vec3(1, 0, 0);
//		// Ambient : simulates indirect lighting
//		MaterialAmbientColor +
//		// Diffuse : "color" of the object
//		MaterialDiffuseColor * LightColor * LightPower * cosTheta / (distance*distance) +
//		// Specular : reflective highlight, like a mirror
//		MaterialSpecularColor * LightColor * LightPower * pow(cosAlpha,5) / (distance*distance);

}