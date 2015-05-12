attribute vec4 position;
//uniform vec4 newColor;

uniform mat4 modelViewProjectionMatrix;

void main(){
	gl_Position = modelViewProjectionMatrix * position;
    gl_FragColor = newColor;

}