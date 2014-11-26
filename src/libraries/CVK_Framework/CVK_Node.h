#ifndef __CVK_NODE_H
#define __CVK_NODE_H

#include "CVK_Defs.h"
#include <string>
#include "CVK_Geometry.h"
#include "CVK_Material.h"
#include "CVK_State.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace CVK
{

// forward declaration
class ShaderMinimal;

/**
 * @brief Class for Nodes in the scene graph.
 * 
 * A Node in the scene graph represents the hierarchical structure
 * of the scene. One node consists of a list of subnodes, representing
 * all childs of this node. It consists of a 4x4 model matrix, for 
 * the current localization, rotation and scale of the represented
 * object and all childs. 
 * 
 * Furthermore a node may have a connection to a geometry, which can
 * be rendered and a material, which can be used while rendering by
 * the current shader. Additionally a node may have a name, which can
 * be used to search this specific node in the hierarchy.
 * 
 * When a node is created, standard values for all members are set,
 * which can be set manually later. An example creation of a node is:
 * CVK::Node node();
 * node.setGeometry( geometry);
 * node.setMaterial( material);
 * scene.addChild( &node);
 * 
 * Another way to create a node is to load a scene with ASSIMP. This
 * is done by using the path of the file, which has to be loaded. The
 * loaded scene is then represented in this node and its child nodes. An 
 * example for this is:
 * CVK::Node* scene = new CVK::Node("Scene", "/pathToFile.obj");
 * 
 * While rendering the scene, the current node accumulates the model
 * matrix to the current model matrix and sends it to the bound shader.
 * Also a shader update is called with this node. If the node has a
 * geometry, then it is rendered. At the end the render function is 
 * called in every child, with a copy of the current accumulated model 
 * matrix. An example of rendering the scene is:
 * scene->render();
 */
class Node
{
public:
	Node();
	Node(std::string name);
	Node(std::string name, std::string path);
	~Node();
	
	// called by constructors
	void init();
	void load(std::string path);
	
	// main render call
	void render();
	// used by render();
	void render( glm::mat4 modelMatrix);

	Node* find(std::string name);
	
	void setName(std::string name);
	std::string* getName();

	void setGeometry(CVK::Geometry* geometry);
	CVK::Geometry* getGeometry();
	bool hasGeometry();

	void setMaterial(CVK::Material* material);
	CVK::Material* getMaterial();
	bool hasMaterial();

	void setModelMatrix(glm::mat4 modelMatrix);
	glm::mat4* getModelMatrix();

	void setParent(Node* node);
	Node* getParent();

	void addChild(Node* node);
	std::vector<Node*>* getChilds();

private:
	std::string m_name;
	CVK::Geometry* m_geometry;
	CVK::Material* m_material;
	glm::mat4 m_modelMatrix;
	Node* m_parent;
	std::vector<Node*> m_childs;
};

};

#endif /* __CVK_NODE_H */

