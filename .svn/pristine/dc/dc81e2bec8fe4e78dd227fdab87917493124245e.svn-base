#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat3 normalModel;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;
out vec3 result;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
	
	// 顶点观察坐标
	vec3 FragPos = vec3(view * model * vec4(aPos, 1.0f));

	// 计算要用到的向量
    vec3 Normal = normalize(normalModel * aNormal);
	vec3 viewDir = normalize(-FragPos);
	vec3 lightDir = normalize(lightPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, Normal);

	// 计算系数
	float ambientStrength = 0.1f;
	float diff = max(dot(lightDir, Normal), 0.0f);
	float spec = pow(max(dot(reflectDir, viewDir), 0.0f), 32);

	// 计算结果
	result = (ambientStrength + diff + spec) * lightColor * objectColor;
}