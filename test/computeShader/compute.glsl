#version 430

layout(rgba8) uniform readonly  image2D u_inTex;
layout(rgba8) uniform writeonly image2D u_outTex;

uniform int u_kernSize;

layout (local_size_x = 1, local_size_y = 1) in;

layout(std430, binding = 3) buffer gaussLayout
{
	float gauss[12][12];
};

void main()
{
	vec4 sum = vec4(0,0,0,0);
	ivec2 index = ivec2(gl_GlobalInvocationID.xy);
	float len = 0;
	
	for (int dx=-u_kernSize;dx<=u_kernSize;++dx)
	for (int dy=-u_kernSize;dy<=u_kernSize;++dy)
	{
		float g = gauss[abs(dx)][abs(dy)];
		vec4 c = imageLoad(u_inTex, index + ivec2(dx,dy));
		
		sum += c * g;
		len += g;
	}
	
	imageStore(u_outTex, index, sum / len + imageLoad(u_inTex,index));
}