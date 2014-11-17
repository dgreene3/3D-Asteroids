
#include "GraphicsGL.h"


//any other include files

//From arcsynthesis: can change the shader functions to accepts vectors of shaders and pass in 
//a shader list, will have to change a few things and add a loop to attach all shaders for example.


GLuint CompileShader(GLenum shaderType, const std::string &shaderText) {
	GLuint shader = glCreateShader(shaderType);
	GLint textLength = (GLint)shaderText.size();
	const GLchar* pText = static_cast<const GLchar*>(shaderText.c_str());
	glShaderSource(shader, 1, &pText, &textLength);
	glCompileShader(shader);

	//check for errors
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if(status == GL_FALSE) {
		GLint infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar* strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);

		const char* strShaderType = NULL;
		switch(shaderType) {
		case GL_VERTEX_SHADER: strShaderType = "vertex"; break;
		case GL_GEOMETRY_SHADER: strShaderType = "geometry"; break;
		case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
		}
		fprintf(stderr, "Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
		delete[] strInfoLog;
	}
	return shader;
}


GLuint LoadShader(GLenum shaderType, const std::string& strFilename) {
	//open file
	std::ifstream shaderFile(strFilename.c_str());
	//check for errors in opening file
	if(shaderFile == NULL) {
		printf("Error opening file %s\n", strFilename);
	}
	std::stringstream shaderData;
	shaderData << shaderFile.rdbuf();
	shaderFile.close();

	return CompileShader(shaderType,  shaderData.str());

}

GLuint LinkProgram(GLuint program, GLuint vert, GLuint frag) {
	glAttachShader(program, vert);
	glAttachShader(program, frag);

	glLinkProgram(program);

	//check for errors after linking
	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if(status == GL_FALSE) {
		GLint infoLogLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar * strInfoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
		fprintf(stderr, "Linker failure: %s\n", strInfoLog);
		delete[] strInfoLog;
	}

	glDetachShader(program, vert);
	glDetachShader(program, frag);

	return program;
}


GLuint CreateProgram(GLuint vert, GLuint frag) {
	GLuint program = glCreateProgram();
	program = LinkProgram(program, vert, frag);

	glDeleteShader(vert);
	glDeleteShader(frag);

	return program;
}


//glm::vec3 GetCameraPosition(glm::vec3& camSpherePos, glm::vec3& camOffset) {
//	float phi = camSpherePos.x * degToRad;
//	float theta = camSpherePos.y * degToRad;
//
//	float fSinTheta = sinf(theta);
//	float fCosTheta = cosf(theta);
//	float fSinPhi = sinf(phi);
//	float fCosPhi = cosf(phi);
//
//	glm::vec3 dirToCamera(fSinTheta * fCosPhi, fCosTheta, fSinTheta * fSinPhi);
//	glm::vec3 temp = (dirToCamera * camSpherePos.z) + camOffset;
//
//	return temp;
//}
//
//
//glm::mat4 CalcViewMatrix(const glm::vec3 &cameraPos, const glm::vec3 &cameraTarget, const glm::vec3 &up) {
//	glm::vec3 lookDir = glm::normalize(cameraTarget - cameraPos);
//
//	glm::vec3 upDir = glm::normalize(up);
//
//	glm::vec3 rightDir = glm::normalize( glm::cross(lookDir, upDir) );
//	glm::vec3 perpUpDir = glm::cross(rightDir, lookDir);
//
//	glm::mat4 rotMat(1.0f);
//	rotMat[0] = glm::vec4(rightDir, 0.0f);
//	rotMat[1] = glm::vec4(perpUpDir, 0.0f);
//	rotMat[2] = glm::vec4(-lookDir, 0.0f);
//	
//	rotMat = glm::transpose(rotMat);
//
//	glm::mat4 transMat(1.0f);
//	transMat[3] = glm::vec4(-cameraPos, 1.0f);
//
//	return rotMat * transMat;
//	
//}


float CalcFrustumScale(float fFovDeg) {
	float fFovRad = fFovDeg * degToRad;
	return 1.0f / tan(fFovRad / 2.0f);
}