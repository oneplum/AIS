#version 410 core

layout(location = 0) in vec3 vertexPosition;  // vertex position in object/model space
layout(location = 1) in vec3 vertexNormal;    // vertex normal in object/model space

uniform mat4 MVP; // model-view-projection Matrix

//uniform vec3 kd; // material diffuse color
uniform mat4 MV;
uniform mat4 MVit;

uniform vec3 ka = vec3(0.05, 0.05, 0.05);
uniform vec3 kd = vec3(0.8, 0.0, 0.0);
uniform vec3 ks = vec3(1, 1, 1);
uniform float shininess = 50;

uniform vec3 la = vec3(0.9, 0.9, 0.9);
uniform vec3 ld = vec3(0.9, 0.9, 0.9);
uniform vec3 ls = vec3(0.9, 0.9, 0.9);
uniform vec4 lightPosition;

flat out vec4 C;

void main() {
	gl_Position = MVP * vec4(vertexPosition, 1);

    vec3 positionViewSpace = vec3(MV * vec4(vertexPosition, 1));
    vec3 normalViewSpace = normalize((MVit * vec4(vertexNormal, 0)).xyz);
    vec3 L = normalize(lightPosition.xyz - positionViewSpace);
    
    vec3 ambient = ka * la;

    float d = max(0, dot(normalViewSpace, L));
    vec3 diffuse = d  * kd * ld;

    float s = 0;
    if(d > 0) {
        vec3 V = normalize(-positionViewSpace);
        vec3 R =  reflect(-L, normalViewSpace);
        s = pow(max(0, dot(V, R)), shininess);
    }
    vec3 specular = s * ks * ls;

    C = vec4(ambient + diffuse + specular, 1);
}
