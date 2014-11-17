#ifndef GRAPHICSGL_H
#define GRAPHICSGL_H

#include "Precompiled.h"





/* Takes a shader type(vertex or fragment) and a file name for the shader code
   and opens the file and then calles compile shader. The file name is the full path name.
   Returns a compiled shader in a shader object */
GLuint LoadShader(GLenum shaderType, const std::string& strFilename);

/* Takes a shader type and the code for the loaded from the file and compiles it into
   a shader object and then returns it. */
GLuint CompileShader(GLenum shaderType, const std::string& shaderText);

/* Takes two shaders: vertex and fragment and creates a program and links the shader objects
   with the program. Returns program. */
GLuint LinkProgram(GLuint program, GLuint vert, GLuint frag);

/* Creates a program and calles LinkProgram and returns the program */
GLuint CreateProgram(GLuint vert, GLuint frag);





//
////get current camera position and return
const float degToRad = (float)3.14159 * 2.0f / 360.0f;

//glm::vec3 GetCameraPosition(glm::vec3& camSpherePos, glm::vec3& camOffset);
//
////calculate and return view matrix: need camera position, target, up dir
//glm::mat4 CalcViewMatrix(const glm::vec3 &cameraPos, const glm::vec3 &cameraTarget, const glm::vec3 &up);
//
////get frustum scale for proj matrix
float CalcFrustumScale(float fFovDeg);


#endif
