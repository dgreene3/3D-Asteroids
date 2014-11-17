#ifndef OBJECT_H
#define OBJECT_H


#include "Precompiled.h"

#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"

#include "Type.h"



class Object {
public:
	Object();
	Object(Mesh* m, Shader* s, bool textureEnabled_);
	~Object(); /* virtual or not? */


	/* For testing, going to just use update in object, but should be overridden in derviced class and should
	   be abstract */

	
	virtual void Update(float dt);
	virtual void Render(); 
	virtual void FlagToDelete();


	glm::vec3 GetPos();


	virtual bool Initialize(); 


	void SetMesh(Mesh* meshPtr);
	void EnableMesh();
	void DisableMesh();

	void SetShader(Shader* shaderPtr);
	void EnableShader();
	void DisableShader();

	void SetTexture(Texture* tex);


	void SetTransformation(glm::mat4& transform);

	bool GetActive();


	/* Maybe have method to update per object uniforms */


private:
	bool hasTexture();



protected:

	Type type;
	std::string objFile;
	std::string meshName;
	std::string texName;


	/* Graphics related members */
	Shader* shaderProgram; /* remove this, object doesn't need a pointer to shader program */
	Mesh* mesh;		
	Texture* texture; /* could be a vector of textures if object is rendered with more than one texture! */


	/* Object data */
	
				

	/* Transformation matrix */
	glm::mat4 transformMatrix;			// MVP matrix:	proj * view * world

	/* World Position */
	glm::vec3 PosW;						// Position of object in World Space

	/* Active */
	bool isActive; /* when set to false, ready to be released */

	bool textureEnabled;

};




#endif