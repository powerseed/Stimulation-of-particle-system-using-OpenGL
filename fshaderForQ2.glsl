#version 150

flat in vec4 color;
out vec4 fColor;

void main() 
{ 
   fColor = color;
   //fColor = vec4(1,1,1,1);
}