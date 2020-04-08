## PROJ IMN501

* See how to pass depthTexture to frag shader
	* https://github.com/opengl-tutorials/ogl/blob/316cccc5f76f47f09c16089d98be284b689e057d/tutorial16_shadowmaps/tutorial16.cpp
	
* What to do
	* https://www.reddit.com/r/godot/comments/9t8zwg/occluded_silhouette_shader_problem/
	* First pass to store the depth value
	* Then compare depthTex.z < gl_FragCoord.z
		* Then color
		* otherwise dont draw 