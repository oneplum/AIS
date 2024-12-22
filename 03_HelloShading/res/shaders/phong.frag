#version 410 core

in vec3 positionViewSpace;
in vec3 normalViewSpace;

uniform vec3 ka = vec3(0.1, 0.1, 0.1);
uniform vec3 kd = vec3(0.0, 0.0, 0.8);
uniform vec3 ks = vec3(1.0, 1.0, 1.0);
uniform float shininess = 50;

uniform vec3 la = vec3(0.9, 0.9, 0.9);
uniform vec3 ld = vec3(0.9, 0.9, 0.9);
uniform vec3 ls = vec3(0.9, 0.9, 0.9);
uniform vec4 lightPosition;

out vec4 color;

void main() {
	vec3 normalViewSpace = normalize(normalViewSpace);
	vec3 L = normalize(lightPosition.xyz - positionViewSpace);

	vec3 ambient = ka * la;

	float d = max(0, dot(normalViewSpace, L));
	vec3 diffuse = d * kd * ld;

	float s = 0;
	if(d > 0) {
		vec3 V = normalize(-positionViewSpace);
		vec3 R = reflect(-L, normalViewSpace);
		s = pow(max(0, dot(V, R)), shininess);
	}
	vec3 specular = s * ks * ls;

	color = vec4(ambient + diffuse + specular, 1);
}
