#version 410 core

in vec3 posViewSpace;
in vec3 normalViewSpaceInterpolated;
in vec3 tangentViewSpaceInterpolated;
in vec3 binormalViewSpaceInterpolated;
in vec2 texCoordsInterpolated;

uniform sampler2D td;
uniform sampler2D ts;
uniform sampler2D tn;

uniform vec4 lightPosition;

uniform vec3 ka = vec3(0.05f, 0.05f, 0.05f); // material ambient color
uniform float shininess = 50.0f;

uniform vec3 la = vec3(0.9f, 0.9f, 0.9f); // light ambient color
uniform vec3 ld = vec3(0.9f, 0.9f, 0.9f); // light diffuse color
uniform vec3 ls = vec3(0.9f, 0.9f, 0.9f); // light specular color

out vec4 color;

void main() {
  vec3 kd = texture(td, texCoordsInterpolated).rgb;
  vec3 ks = texture(ts, texCoordsInterpolated).rgb;

  vec3 normalViewSpace = normalize(normalViewSpaceInterpolated);
  vec3 tangentViewSpace = normalize(tangentViewSpaceInterpolated);
  vec3 binormalViewSpace = normalize(binormalViewSpaceInterpolated);
    
  vec3 texCoords = texture(tn, texCoordsInterpolated).xyz;
  if(texCoords != vec3(0, 0, 0)) {
      texCoords = normalize(2 * (texCoords - vec3(0.5)));
      normalViewSpace = normalize(mat3(tangentViewSpace, binormalViewSpace, normalViewSpace) * texCoords);
    }
  
  vec3 lightVec = normalize(lightPosition.xyz - posViewSpace);

  // ambient color
  vec3 ambient = ka * la;

  // diffuse color
  float d = max(0, dot(normalViewSpace, lightVec));
  vec3 diffuse = d * kd * ld;

  // specular color
  vec3 viewVec =  normalize(-posViewSpace);
  vec3 reflected =  reflect(-lightVec, normalViewSpace);
  float s = pow(max(0, dot(viewVec, reflected)), shininess);
  vec3 specular = s * ks * ls;

  color = vec4(ambient + diffuse + specular, 1);
}
