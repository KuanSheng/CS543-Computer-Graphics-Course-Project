// Starter program for HW 0. 
// Program draws a triangle. Study the program first
// Then modify the function generateGeometry to draw a two-Dimensional Sierpinski Gasket       
// Generated using randomly selected vertices and bisection

// Angel.h is homegrown include file that also includes glew and freeglut

#include "Angel.h"

// Number of points in polyline
const int NumPoints = 3;

//----------------------------------------------------------------------------

// remember to prototype
void generateGeometry( void );
void initGPUBuffers( void );
void shaderSetup( void );
void display( void );
void keyboard( unsigned char key, int x, int y );

typedef vec2 point2;

using namespace std;

// Array for polyline
point2 points[NumPoints];
GLuint program;


void generateGeometry( void )
{
	// ***************** Important note ***************** //
	// please refer to OpenGL documentation before coding
	// many old functions have been depricated
	// though they will probably still work on most machines others
	// will require compatiability mode to be used

	// Specifiy the vertices for a triangle
	// for 3d points use vec3 and change your vPosition attribute appropriately

	points[0] = point2( -1.5, -0.5 );
	points[1] = point2( 0.0, 0.5 );
	points[2] = point2( 0.5, -0.5 );
}


void initGPUBuffers( void )
{
	// Create a vertex array object
    GLuint vao;
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    // Create and initialize a buffer object
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW );


}


void shaderSetup( void )
{
	    // Load shaders and use the resulting shader program
    program = InitShader( "vshader1.glsl", "fshader1.glsl" );
    glUseProgram( program );

    // Initialize the vertex position attribute from the vertex shader
    GLuint loc = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( loc );
    glVertexAttribPointer( loc, 2, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(0) );

	// sets the default color to clear screen
    glClearColor( 1.0, 1.0, 1.0, 1.0 ); // white background

}



//----------------------------------------------------------------------------
// this is where the drawing should happen
void display( void )
{
	// TIP 1: remember to enable depth buffering when drawing in 3d

	// TIP 2: If you want to be sure your math functions are right, 
	// prototype your operations in Matlab or Mathematica first to verify correct behavior
	// both programs support some sort of runtime interface to C++ programs

	// TIP3: check your graphics specs. you have a limited number of loop iterations, storage, registers, texture space etc.
	

	// TIP4: avoid using glTranslatex, glRotatex, push and pop
	// pass your own view matrix to the shader directly
	// refer to the latest OpenGL documentation for implementation details

    glClear( GL_COLOR_BUFFER_BIT );     // clear the window
    glDrawArrays( GL_LINE_LOOP, 0, NumPoints );    // draw the points
    glFlush(); // force output to graphics hardware

}

//----------------------------------------------------------------------------

// keyboard handler
void keyboard( unsigned char key, int x, int y )
{
    switch ( key ) {
    case 033:
        exit( EXIT_SUCCESS );
        break;
    }
}

//----------------------------------------------------------------------------
// entry point
int main( int argc, char **argv )
{
	// init glut
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_SINGLE );
    glutInitWindowSize( 1280, 800 );

    // If you are using freeglut, the next two lines will check if 
    // the code is truly 3.2. Otherwise, comment them out
    
    //glutInitContextVersion( 3, 3 );
    //glutInitContextProfile( GLUT_CORE_PROFILE );

	// create GLUT window for drawing
    glutCreateWindow( "Starting App Of Hao Zhou" );

	// init glew
    glewInit();

    generateGeometry( );
    initGPUBuffers( );
    shaderSetup( );

	// assign handlers
    glutDisplayFunc( display );
    glutKeyboardFunc( keyboard );
	// should add menus
	// add mouse handler
	// add resize window functionality (should probably try to preserve aspect ratio)

	// enter the drawing loop
	// frame rate can be controlled with 
    glutMainLoop();
    return 0;
}
