#version 150

flat in vec4 colorForFire;
out vec4 fColorForFire;

void main() 
{ 
   fColorForFire = colorForFire;
   //fColorForFire = vec4(1,1,1,1);
}