#include "common.h"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <cstdlib>
#include <time.h>

const char *WINDOW_TITLE = "Minecraft fire";
const double FRAME_RATE_MS = 1000.0/60.0;

void drawACube(glm::vec3 tranForEachCube, glm::mat4 rotate, glm::vec3 scale, GLuint modelView);

typedef glm::vec4 color4;
typedef glm::vec4 point4;

GLuint ModelView, Projection, ModelViewForFire, ProjectionForFire;
glm::vec3 pushback ( 0.0, 0.0, -2.0 );

glm::mat4 model_view;
glm::vec3 tranForEachCube;

GLuint vPosition;
GLuint vColor;
GLuint vPositionForFire;
GLuint vColorForFire;

float eyeX = 1.0f;
float eyeZ = -1.0f;

glm::mat4 lookat;
glm::mat4 perspective;
glm::mat4 projection;

glm::vec3 ScaleofCube( 0.15, 0.15, 0.15 );

glm::vec3 ScaleofFire(0.075, 0.075, 0.075);

int numberOfFire = 40;

glm::vec3 positionOfFire[40];
glm::vec3 velocityOfFire[40];
float rotationVelocityOfFire[40];
int rotationAxisOfFire[40];
float lifespanOfFire[40];
glm::mat4 rotationOfFire[40];

GLuint program;
GLuint programForFire;

float lastTime = 0;
float nowTime = 0;
float elipse = 0;

int direction = 0;

point4 vertices[25] = {
   point4( -0.5, 0, -0.5, 1.0 ),
   point4( -0.5, 0, -0.25, 1.0 ),
   point4( -0.5, 0, 0, 1.0 ),
   point4( -0.5, 0, 0.25, 1.0 ),
   point4( -0.5, 0, 0.5, 1.0 ),

   point4(-0.25, 0, -0.5, 1.0),
   point4(-0.25, 0, -0.25, 1.0),
   point4(-0.25, 0, 0, 1.0),
   point4(-0.25, 0, 0.25, 1.0),
   point4(-0.25, 0, 0.5, 1.0),

   point4(0, 0, -0.5, 1.0),
   point4(0, 0, -0.25, 1.0),
   point4(0, 0, 0, 1.0),
   point4(0, 0, 0.25, 1.0),
   point4(0, 0, 0.5, 1.0),

   point4(0.25, 0, -0.5, 1.0),
   point4(0.25, 0, -0.25, 1.0),
   point4(0.25, 0, 0, 1.0),
   point4(0.25, 0, 0.25, 1.0),
   point4(0.25, 0, 0.5, 1.0),

   point4(0.5, 0, -0.5, 1.0),
   point4(0.5, 0, -0.25, 1.0),
   point4(0.5, 0, 0, 1.0),
   point4(0.5, 0, 0.25, 1.0),
   point4(0.5, 0, 0.5, 1.0),
};

GLuint indices[] = {
   // each group of 3 is a triangle face
	0, 1, 5,
	1, 6, 5, // 5

	1, 2, 6, 
	2, 7, 6, // 11

	2, 3, 7,
	3, 8, 7, // 17

	3, 4, 8,
	4, 9, 8, // 23 

	5, 6, 10,
	6, 11, 10, // 29

	6, 7, 11,
	7, 12, 11, // 35

	7, 8, 12,
	8, 13, 12, // 41

	8, 9, 13,
	9, 14, 13, // 47

	10, 11, 15,
	11, 16, 15, // 53

	11, 12, 16,
	12, 17, 16, // 59

	12, 13, 17,
	13, 18, 17, // 65

	13, 14, 18,
	14, 19, 18, // 71

	15, 16, 20,
	16, 21, 20, // 77

	16, 17, 21,
	17, 22, 21, // 83

	17, 18, 22,
	18, 23, 22, // 89

	18, 19, 23,
	19, 24, 23 // 95
};

color4 colors[25] = {
	color4(0.0, 0.0, 0.0, 1.0),		// 0
	color4(1.0, 1.0, 1.0, 1.0),		// 1
	color4(0.0, 0.0, 0.0, 1.0),		// 2
	color4(1.0, 1.0, 1.0, 1.0),		// 3
	color4(1.0, 1.0, 1.0, 1.0),		// 4

	color4(0, 0, 0, 1.0),	// 5
	color4(1, 0, 0, 1.0),	// 6
	color4(0, 0, 0, 1.0),	// 7
	color4(1, 0, 0, 1.0),	// 8

	color4(1, 1, 1, 1.0),			// 9

	color4(1, 0, 0, 1.0),	// 11
	color4(0, 0, 0, 1.0),	// 10
	color4(1, 0, 0, 1.0),	// 13
	color4(0, 0, 0, 1.0),	// 12

	color4(1, 1, 1, 1.0),	// 14

	color4(0, 0, 0, 1.0),	// 10
	color4(1, 0, 0, 1.0),	// 11
	color4(0, 0, 0, 1.0),	// 12
	color4(1, 0, 0, 1.0),	// 13

	color4(1, 1, 1, 1.0),	// 19

	color4(1, 0, 0, 1.0),	// 11
	color4(0, 0, 0, 1.0),	// 10
	color4(1, 0, 0, 1.0),	// 13
	color4(0, 0, 0, 1.0),	// 12

	color4(1, 1, 1, 1.0),	// 24
};

void
init()
{
   GLuint vao = 0;
   glGenVertexArrays( 1, &vao );
   glBindVertexArray( vao );

   GLuint buffer;

   // Create and initialize a buffer object
   glGenBuffers( 1, &buffer );
   glBindBuffer( GL_ARRAY_BUFFER, buffer );
   glBufferData( GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors), NULL, GL_STATIC_DRAW );
   glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
   glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors), colors);

   // Another for the index buffer
   glGenBuffers( 1, &buffer );
   glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, buffer );
   glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), NULL, GL_STATIC_DRAW );
   glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(indices), indices);

   // shaders for rest
   program = InitShader("vshaderForQ2.glsl", "fshaderForQ2.glsl");
   glUseProgram(program);
   vPosition = glGetAttribLocation(program, "vPosition");
   glEnableVertexAttribArray(vPosition);
   glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

   vColor = glGetAttribLocation(program, "vColor");
   glEnableVertexAttribArray(vColor);
   glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vertices)));

   ModelView = glGetUniformLocation(program, "ModelView");
   Projection = glGetUniformLocation(program, "Projection");

   // shaders for fire
   programForFire = InitShader("vshaderForQ2Fire.glsl", "fshaderForQ2Fire.glsl");
   glUseProgram(programForFire);
   vPositionForFire = glGetAttribLocation( programForFire, "vPositionForFire" );
   glEnableVertexAttribArray( vPositionForFire );
   glVertexAttribPointer( vPositionForFire, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

   vColorForFire = glGetAttribLocation(programForFire, "vColorForFire");
   glEnableVertexAttribArray(vColorForFire);
   glVertexAttribPointer(vColorForFire, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vertices)));

   ModelViewForFire = glGetUniformLocation(programForFire, "ModelViewForFire");
   ProjectionForFire = glGetUniformLocation(programForFire, "ProjectionForFire");

   srand((unsigned int)time(NULL));

   for (int i = 0; i < numberOfFire; i++)
   {
	   positionOfFire[i] = glm::vec3((double)rand() / RAND_MAX / 3 - 0.2, -0.5f + 2 * ScaleofCube.y, (double)rand() / RAND_MAX / 3 - 0.2);
   }

   for (int i = 0; i < numberOfFire; i++)
   {
	   BOOLEAN isXPositive;
	   BOOLEAN isZPositive;

	   if (rand() % 2 == 0)
	   {
		   isXPositive = true;
	   }
	   else
	   {
		   isXPositive = false;
	   }

	   if (rand() % 2 == 0)
	   {
		   isZPositive = true;
	   }
	   else
	   {
		   isZPositive = false;
	   }

	   int parameter = 5000;

	   if (isXPositive)
	   {
		   if (isZPositive)
		   {
			   velocityOfFire[i] = glm::vec3((double)rand() / RAND_MAX / parameter, (double)rand() / RAND_MAX / parameter, (double)rand() / RAND_MAX / parameter);
		   }
		   else
		   {
			   velocityOfFire[i] = glm::vec3((double)rand() / RAND_MAX / parameter, (double)rand() / RAND_MAX / parameter, -1 * (double)rand() / RAND_MAX / parameter);
		   }
	   }
	   else
	   {
		   if (isZPositive)
		   {
			   velocityOfFire[i] = glm::vec3( -1 * (double)rand() / RAND_MAX / parameter, (double)rand() / RAND_MAX / parameter, (double)rand() / RAND_MAX / parameter);
		   }
		   else
		   {
			   velocityOfFire[i] = glm::vec3( -1 * (double)rand() / RAND_MAX / parameter, (double)rand() / RAND_MAX / parameter, -1 * (double)rand() / RAND_MAX / parameter);
		   }
	   }
   }

   for (int i = 0; i < numberOfFire; i++)
   {
	   rotationVelocityOfFire[i] = (double)rand() / RAND_MAX;
   }

   for (int i = 0; i < numberOfFire; i++)
   {

	   rotationAxisOfFire[i] = rand() % (3 - 1 + 1) + 1;
   }

   for (int i = 0; i < numberOfFire; i++)
   {
	   lifespanOfFire[i] = rand() % (8 - 6 + 1) + 6;
   }

   glEnable( GL_DEPTH_TEST );
}

void display( void )
{
   glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

   // switch to shaders for the rest geometries.
   glUseProgram(program);
   lookat = glm::lookAt(glm::vec3(eyeX, 1.0f, eyeZ), glm::vec3(0.0f, 0.0f, -2.0f), glm::vec3(0.0f, 1.0f, 0.0f));
   perspective = glm::perspective(glm::radians(45.0f), 1.0f, 1.0f, 3.0f);
   projection = perspective * lookat;
   glUniformMatrix4fv( Projection, 1, GL_FALSE, glm::value_ptr(projection) );

   // floor
   int countOfCubes = 0;

   for (float x = -1; x < 1; x = x + ScaleofCube.x)
   {
	   for (float z = -1; z < 1; z = z + ScaleofCube.x)
	   {
		   if ( countOfCubes % 13 == 0 )
		   {
			   for (int i = 0; i < sizeof(colors) / sizeof(color4); i++)
			   {
				   colors[i] = color4( 50.0/255, 205.0/255, 50.0/255, 1 );
			   }
		   }
		   else if (countOfCubes % 3 == 0 || countOfCubes % 17 == 0 )
		   {
			   for (int i = 0; i < sizeof(colors) / sizeof(color4); i++)
			   {
				   colors[i] = color4( 60.0/255, 179.0/255, 113.0/255, 1);
			   }
		   }
		   else if (countOfCubes % 5 == 0 || countOfCubes % 19 == 0 )
		   {
			   for (int i = 0; i < sizeof(colors) / sizeof(color4); i++)
			   {
				   colors[i] = color4( 46.0/255, 139.0/255, 87.0/255, 1);
			   }
		   }
		   else if (countOfCubes % 7 == 0 || countOfCubes % 23 == 0)
		   {
			   for (int i = 0; i < sizeof(colors) / sizeof(color4); i++)
			   {
				   colors[i] = color4(34.0/255, 139.0/255, 34.0/255, 1);
			   }
		   }
		   else if (countOfCubes % 11 == 0 || countOfCubes % 29 == 0)
		   {
			   for (int i = 0; i < sizeof(colors) / sizeof(color4); i++)
			   {
				   colors[i] = color4( 0, 128.0/255, 0, 1);
			   }
		   }
		   else
		   {
			   for (int i = 0; i < sizeof(colors) / sizeof(color4); i++)
			   {
				   colors[i] = color4( 0, 100.0/255, 0, 1);
			   }
		   }
		   glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors), colors);

		   tranForEachCube = glm::vec3(x, -0.5f, z);
		   drawACube( tranForEachCube, glm::rotate(glm::mat4(), glm::radians(0.0f), glm::vec3(0,1,0)), ScaleofCube, ModelView);
		   countOfCubes++;
	   }
   }

   // logs
   colors[5] = color4(139.0/255, 69.0/255, 19.0/255, 1);
   colors[6] = color4(139.0 / 255, 69.0 / 255, 19.0 / 255, 1);
   colors[7] = color4(139.0 / 255, 69.0 / 255, 19.0 / 255, 1);
   colors[8] = color4(162.0/255, 82.0/255, 45.0/255, 1);

   colors[10] = color4(139.0 / 255, 69.0 / 255, 19.0 / 255, 1);
   colors[11] = color4(139.0 / 255, 69.0 / 255, 19.0 / 255, 1);
   colors[12] = color4(162.0 / 255, 82.0 / 255, 45.0 / 255, 1);
   colors[13] = color4(162.0 / 255, 82.0 / 255, 45.0 / 255, 1);

   colors[15] = color4(139.0 / 255, 69.0 / 255, 19.0 / 255, 1);
   colors[16] = color4(162.0 / 255, 82.0 / 255, 45.0 / 255, 1);
   colors[17] = color4(162.0 / 255, 82.0 / 255, 45.0 / 255, 1);
   colors[18] = color4(210.0/255, 105.0/255, 30.0/255, 1);

   colors[20] = color4(162.0 / 255, 82.0 / 255, 45.0 / 255, 1);
   colors[21] = color4(162.0 / 255, 82.0 / 255, 45.0 / 255, 1);
   colors[22] = color4(210.0 / 255, 105.0 / 255, 30.0 / 255, 1);
   colors[23] = color4(210.0 / 255, 105.0 / 255, 30.0 / 255, 1);

   glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors), colors);

   for (float x = -0.5; x < 0.5; x = x + ScaleofCube.x)
   {
	   tranForEachCube = glm::vec3(x, -0.5f + ScaleofCube.y, 0);
	   drawACube(tranForEachCube, glm::rotate(glm::mat4(), glm::radians(0.0f), glm::vec3(0, 1, 0)), ScaleofCube, ModelView);
   }

   for (float z = -0.5; z < 0.5; z = z + ScaleofCube.z)
   {
	   tranForEachCube = glm::vec3(0, -0.5f + ScaleofCube.y, z);
	   drawACube(tranForEachCube, glm::rotate(glm::mat4(), glm::radians(0.0f), glm::vec3(0, 1, 0)), ScaleofCube, ModelView);
   }

   // fire
   colors[5] = color4( 139.0/255, 69.0/255, 0, 1 );
   colors[6] = color4(139.0 / 255, 69.0 / 255, 0, 1);
   colors[7] = color4(205.0/255, 102.0/255, 0, 1);
   colors[8] = color4(205.0 / 255, 102.0 / 255, 0, 1);

   colors[10] = color4(139.0 / 255, 69.0 / 255, 0, 1);
   colors[11] = color4(205.0 / 255, 102.0 / 255, 0, 1);
   colors[12] = color4(205.0 / 255, 102.0 / 255, 0, 1);
   colors[13] = color4(238.0/255, 118.0/255, 0, 1);

   colors[15] = color4(205.0 / 255, 102.0 / 255, 0, 1);
   colors[16] = color4(205.0 / 255, 102.0 / 255, 0, 1);
   colors[17] = color4(238.0 / 255, 118.0 / 255, 0, 1);
   colors[18] = color4(255.0 / 255, 127.0 / 255, 0, 1);

   colors[20] = color4(205.0 / 255, 102.0 / 255, 0, 1);
   colors[21] = color4(238.0 / 255, 118.0 / 255, 0, 1);
   colors[22] = color4(255.0 / 255, 127.0 / 255, 0, 1);
   colors[23] = color4(255.0 / 255, 127.0 / 255, 0, 1);

   glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors), colors);

   // switch to shaders for fire
   glUseProgram(programForFire);
   glUniformMatrix4fv(ProjectionForFire, 1, GL_FALSE, glm::value_ptr(projection));
   //

   nowTime = glutGet(GLUT_ELAPSED_TIME);
   elipse = nowTime - lastTime;
   lastTime = nowTime;

   for (int i = 0; i < numberOfFire; i++)
   {
	   lifespanOfFire[i] -= 0.2;

	   if (lifespanOfFire[i] <= 0) // disappear
	   {
		   lifespanOfFire[i] = rand() % (8 - 6 + 1) + 6;
		   positionOfFire[i] = glm::vec3( (double)rand() / RAND_MAX / 3 - 0.2, -0.5f + 2 * ScaleofCube.y, (double)rand() / RAND_MAX / 3 - 0.2);
	   }
	   else
	   {
		   positionOfFire[i].x += velocityOfFire[i].x * elipse;
		   positionOfFire[i].y += velocityOfFire[i].y * elipse;
		   positionOfFire[i].z += velocityOfFire[i].z * elipse;

		   if (rotationAxisOfFire[i] == 1) //x
		   {
			   rotationOfFire[i] = glm::rotate(rotationOfFire[i], glm::radians(rotationVelocityOfFire[i] * elipse), glm::vec3(1, 0, 0));
		   }
		   else if (rotationAxisOfFire[i] == 2) //y
		   {
			   rotationOfFire[i] = glm::rotate(rotationOfFire[i], glm::radians(rotationVelocityOfFire[i] * elipse), glm::vec3(0, 1, 0));
		   }
		   else // z
		   {
			   rotationOfFire[i] = glm::rotate(rotationOfFire[i], glm::radians(rotationVelocityOfFire[i] * elipse), glm::vec3(0, 0, 1));
		   }
	   }
   }

   for ( int i = 0; i < numberOfFire; i++ )
   {
	   tranForEachCube = positionOfFire[i];
	   drawACube(tranForEachCube, rotationOfFire[i], ScaleofFire, ModelViewForFire);
   }

   if (direction == 0)
   {
	   eyeX += 0.02;

	   if (eyeX > 1)
	   {
		   eyeX = 1;
		   direction = -1;
	   }
   }
   else if (direction == 1)
   {
	   eyeZ -= 0.02;

	   if (eyeZ < -3)
	   {
		   eyeZ = -3;
		   direction = -1;
	   }
   }
   else if (direction == 2)
   {
	   eyeX -= 0.02;

	   if (eyeX < -1)
	   {
		   eyeX = -1;
		   direction = -1;
	   }
   }
   else if (direction == 3)
   {
	   eyeZ += 0.02;

	   if ( eyeZ > -1 )
	   {
		   eyeZ = -1; 
		   direction = -1; 
	   }
   }

   glutSwapBuffers();
}

void drawACube(glm::vec3 tranForEachCube, glm::mat4 rotate, glm::vec3 scale, GLuint modelView)
{
	// base
	model_view = glm::translate(glm::mat4(), tranForEachCube) * glm::translate(glm::mat4(), pushback) * rotate * glm::scale(glm::mat4(), scale);
	glUniformMatrix4fv(modelView, 1, GL_FALSE, glm::value_ptr(model_view));
	for (int i = 0; i < 96; i += 3)
	{
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void *)(i * sizeof(GLuint)));
	}

	//back
	model_view = glm::translate(glm::mat4(), tranForEachCube) * glm::translate(glm::mat4(), pushback) * rotate * glm::translate(glm::mat4(), glm::vec3(0, 0, -scale.x / 2)) * glm::rotate(glm::mat4(), glm::radians(90.0f), glm::vec3(1, 0, 0)) * glm::scale(glm::mat4(), scale);
	glUniformMatrix4fv(modelView, 1, GL_FALSE, glm::value_ptr(model_view));
	for (int i = 0; i < 96; i += 3)
	{
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void *)(i * sizeof(GLuint)));
	}

	//front
	model_view = glm::translate(glm::mat4(), tranForEachCube) * glm::translate(glm::mat4(), pushback) * rotate * glm::translate(glm::mat4(), glm::vec3(0, 0, scale.x / 2)) * glm::rotate(glm::mat4(), glm::radians(90.0f), glm::vec3(1, 0, 0)) * glm::scale(glm::mat4(), scale);
	glUniformMatrix4fv(modelView, 1, GL_FALSE, glm::value_ptr(model_view));
	for (int i = 0; i < 96; i += 3)
	{
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void *)(i * sizeof(GLuint)));
	}

	// left
	model_view = glm::translate(glm::mat4(), tranForEachCube) * glm::translate(glm::mat4(), pushback) * rotate * glm::translate(glm::mat4(), glm::vec3(-scale.x / 2, 0, 0)) * glm::rotate(glm::mat4(), glm::radians(90.0f), glm::vec3(0, 0, 1)) * glm::scale(glm::mat4(), scale);
	glUniformMatrix4fv(modelView, 1, GL_FALSE, glm::value_ptr(model_view));
	for (int i = 0; i < 96; i += 3)
	{
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void *)(i * sizeof(GLuint)));
	}

	// right
	model_view = glm::translate(glm::mat4(), tranForEachCube) * glm::translate(glm::mat4(), pushback) * rotate * glm::translate(glm::mat4(), glm::vec3(scale.x / 2, 0, 0)) * glm::rotate(glm::mat4(), glm::radians(90.0f), glm::vec3(0, 0, 1)) * glm::scale(glm::mat4(), scale);
	glUniformMatrix4fv(modelView, 1, GL_FALSE, glm::value_ptr(model_view));
	for (int i = 0; i < 96; i += 3)
	{
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void *)(i * sizeof(GLuint)));
	}

	// up
	model_view =  glm::translate(glm::mat4(), tranForEachCube) * glm::translate(glm::mat4(), pushback) * rotate * glm::translate(glm::mat4(), glm::vec3(0, scale.x / 2, 0)) * glm::scale(glm::mat4(), scale);
	glUniformMatrix4fv(modelView, 1, GL_FALSE, glm::value_ptr(model_view));
	for (int i = 0; i < 96; i += 3)
	{
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void *)(i * sizeof(GLuint)));
	}
}

void mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN)
	{
		switch (button)
		{
			case GLUT_LEFT_BUTTON:
			{
				if (direction == -1)
				{
					if ( eyeX == 1.0f && eyeZ == -1.0f )
					{
						direction = 1;
					}
					else if ( eyeX == 1.0f && eyeZ == -3.0f )
					{
						direction = 2;
					}
					else if ( eyeX == -1.0f && eyeZ == -3.0f )
					{
						direction = 3;
					}
					else if ( eyeX == -1.0f && eyeZ == -1.0f )
					{
						direction = 0;
					}
				}
			}
		}
	}
}

void update(void)
{

}

void reshape( int width, int height )
{
   glViewport( 0, 0, width, height );
}