#version 120

varying vec2 texCoord0;
varying vec3 normal0;

uniform sampler2D diffuse;

float lambert(vec3 N, vec3 L)
{
  vec3 nrmN = normalize(N);
  vec3 nrmL = normalize(L);
  float result = dot(nrmN, nrmL);
  return max(result, 0.0);
}

void main()
{
    gl_FragColor = 

    texture2D(diffuse, texCoord0) * lambert( -vec3(0,0,1), normal0 );
    
}
