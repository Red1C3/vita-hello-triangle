#version 100

attribute vec2 pos;
attribute vec3 color;

varying vec3 colorOut;

void main(){
  gl_Position=vec4(pos.xy,0,1);
  colorOut=color;
}