#version 100

precision lowp float;

varying vec3 colorOut;

void main(){
  gl_FragColor=vec4(colorOut,1);
}