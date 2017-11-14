struct Light
{
    int type;
    highp vec3 position;
    highp vec3 color;
    highp vec3 direction;
    highp float angle2;
    highp float strength;
};

attribute highp vec3 vertex;
attribute highp vec3 ambient;
attribute highp vec3 diffuse;
attribute highp vec3 specular;
attribute highp vec3 normal;
attribute highp vec2 texcoord;

varying highp vec4 vAmbient;
varying highp vec4 vDiffuse;
varying highp vec4 vSpecular;
varying highp vec3 n;
varying highp vec4 pos;
varying highp vec2 vTexCoord;

uniform highp mat4 modelToWorld;
uniform highp mat4 worldToCamera;
uniform highp mat4 cameraToView;
uniform Light lightInfo;
uniform highp mat3 rotation;

void main()
{
  gl_Position = cameraToView * worldToCamera * modelToWorld * vec4(vertex, 1.0);
  n = (rotation * normal);
  n = normalize(n);
  vAmbient =  vec4(ambient, 1.0);
  vDiffuse =  vec4(diffuse, 1.0);
  vSpecular = vec4(specular, 1.0);
  pos = modelToWorld * vec4(vertex, 1.0);
  vTexCoord = texcoord;
}
