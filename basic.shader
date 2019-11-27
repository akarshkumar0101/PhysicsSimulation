#shader vertex
//vec3->vec4
#version 120

uniform float sinVal;
uniform mat4 rotMat;

attribute vec3 vPos;
attribute vec3 vCol;

varying vec4 col;

void main()
{
    vec4 newPos = rotMat * vec4(vPos, 1.0);
    vec3 vPosp = vec3(newPos.x, newPos.y, newPos.z);
    //vPos = newPos.xyz;
    gl_Position = vec4(vPosp, 1.0);
    col = vec4(sinVal*vCol.x, sinVal*vCol.y, sinVal*vCol.z, 1.0);
}

#shader fragment
//?->vec4(color)
#version 120

varying vec4 col;

void main()
{
    gl_FragColor = col;
}