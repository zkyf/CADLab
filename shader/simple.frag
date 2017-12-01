struct Light
{
    int type;
    highp vec3 position;
    highp vec3 color;
    highp vec3 direction;
    highp float angle2;
    highp float strength;
};

uniform highp mat4 modelToWorld;
uniform highp mat4 worldToCamera;
uniform highp mat4 cameraToView;
uniform Light lightInfo;
uniform highp mat3 rotation;
uniform highp vec3 cameraPos;
uniform sampler2D Texture;
uniform int withText;
uniform int mode;

varying highp vec4 qt_TexCoord0;
varying highp vec4 vAmbient;
varying highp vec4 vDiffuse;
varying highp vec4 vSpecular;
varying highp vec3 n;
varying highp vec4 pos;
varying highp vec2 vTexCoord;

highp vec4 MakeColor(highp vec4 a, highp vec4 b)
{
    return vec4(a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w);
}

highp vec4 Ambient(highp vec3 color)
{
    return MakeColor(highp vec4(color, 1.0), vAmbient);
}

highp vec4 Diffuse(highp vec3 lightDir, highp vec3 color)
{
    lightDir = normalize(lightDir);
    highp float intn = dot(-lightDir, n);
    if(intn < 0.0) intn=-intn;
    intn = clamp(intn, 0.1, 1.0);
    return MakeColor(highp vec4(color*intn, 1.0), vDiffuse);
}

highp vec4 Specular(highp vec3 position, highp vec3 color)
{
    highp vec3 lightDir = position - pos.xyz;
    highp float dist = sqrt(dot(lightDir, lightDir));
    lightDir = normalize(lightDir);
    highp float intn = dot(lightDir, n) / (1.0 + dist*0.7 + dist*dist*1.8);
    intn = clamp(intn, 0.1, 1.0);

    highp vec3 viewDir = cameraPos - pos.xyz;
    viewDir = normalize(viewDir);
    highp vec3 reflectDir = reflect(-lightDir, n);
    reflectDir = normalize(reflectDir);
    highp float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    highp float specularStrength = lightInfo.strength;
    highp float specular = specularStrength * spec;
    return MakeColor(vec4(color*(intn+specular), 1.0), vSpecular);
}

highp vec4 DirectionalLight()
{
    return 0.2*Ambient(lightInfo.color)+0.5*Diffuse(lightInfo.direction,lightInfo.color);
}

highp vec4 PointLight()
{
    return 0.2*Ambient(lightInfo.color)+0.5*Diffuse(pos.xyz-lightInfo.position, lightInfo.color)+0.5*Specular(lightInfo.position, lightInfo.color);
}

void main()
{
  if(withText==1)
  {
    highp vec4 texColor;
    texColor = texture2D(Texture, vTexCoord);
    if(texColor.a<0.1)
    {
				discard;
    }
  }
    highp vec4 resultColor = vec4(1.0, 1.0, 1.0, 1.0);
    if(lightInfo.type == 0)
    {
        resultColor = DirectionalLight();
    }
    if(lightInfo.type == 1)
    {
        resultColor = PointLight();
    }
    if(lightInfo.type == 3)
    {
      resultColor = Ambient(lightInfo.color);
    }
    if(withText==1 && mode == 0)
    {
      highp vec4 texColor;
      texColor = texture2D(Texture, vTexCoord);
      if(texColor.a<0.1)
      {
					discard;
      }
      gl_FragColor = MakeColor(texColor, resultColor);
    }
    else if(mode == 2)
    {
      highp vec4 normColor = vec4(normalize(n)/2.0+vec3(0.5, 0.5, 0.5), 1.0);
      gl_FragColor = MakeColor(normColor, resultColor);
    }
    else
        gl_FragColor = resultColor;
}
