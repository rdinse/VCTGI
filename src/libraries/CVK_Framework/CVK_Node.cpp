#include "CVK_Node.h"
#include "CVK_ShaderMinimal.h"

CVK::Node::Node()
{
	init();
}

CVK::Node::Node(std::string name)
{
	init();
	m_name = name;
}

CVK::Node::Node(std::string name, std::string path)
{
	init();
	m_name = name;
	load(path);
}

CVK::Node::~Node()
{
}

void CVK::Node::init()
{
	m_name = "";
	m_geometry = 0;
	m_material = 0;
	m_modelMatrix = glm::mat4(1.0f);
	m_parent = 0;
}

void CVK::Node::load(std::string path)
{
	// load a scene with ASSIMP
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_GenSmoothNormals | aiProcess_Triangulate);
	if(scene)
		printf("SUCCESS: Loaded Model from Path: \"%s\"\n", path.c_str());
	else 
	{
		printf("ERROR: Loading failed from Path: \"%s\"\n", path.c_str());
		return;
	}

	std::vector<CVK::Material*> materials;
	
	// load all materials in this file
	for(unsigned int i=0; i < scene->mNumMaterials; i++)
	{
		aiMaterial* mat = scene->mMaterials[i];

		aiColor3D diffColor (0.f,0.f,0.f);
		mat->Get(AI_MATKEY_COLOR_DIFFUSE, diffColor);
		aiColor3D specColor (0.f,0.f,0.f);
		mat->Get(AI_MATKEY_COLOR_SPECULAR, specColor);
		float shininess = 0.0f;
		mat->Get(AI_MATKEY_SHININESS, shininess);
		glm::vec3 diffuseColor(diffColor.r, diffColor.g, diffColor.b);
		glm::vec3 specularColor(specColor.r, specColor.g, specColor.b);

		aiString fileName;  // filename
		mat->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), fileName);
		std::string s = path.substr(0, path.rfind("/")) + "/";
		s += fileName.data;
		
		//materials.push_back(new CVK::Material(glm::vec3(0.0,1.0,0.0), glm::vec3(0.0,0.0,1.0), 10));
		if (fileName.length>0) 
			materials.push_back(new CVK::Material(const_cast<char*> ( s.c_str() ), diffuseColor, specularColor, shininess));
		else 
			materials.push_back(new CVK::Material(diffuseColor, specularColor, shininess));
	}
	
	// load all meshes in this file
	for(unsigned int i=0; i < scene->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[i];

		CVK::Geometry* geometry = new CVK::Geometry();
		
		// load geometry information of the current mesh
		for(unsigned int vCount = 0; vCount < mesh->mNumVertices; vCount++)
		{
			// vertices
			aiVector3D v = mesh->mVertices[vCount];
			geometry->getVertices()->push_back( glm::vec4(v.x, v.y, v.z, 1.0f));

			// normals
			if (mesh->HasNormals())
			{
				v = mesh->mNormals[vCount];
				geometry->getNormals()->push_back( glm::vec3(v.x, v.y, v.z));
			}

			// texture coordinates
			if (mesh->HasTextureCoords(0))
			{
				v = mesh->mTextureCoords[0][vCount];
				geometry->getUVs()->push_back( glm::vec2(v.x, v.y));
			}
		}

		for(unsigned int fCount = 0; fCount < mesh->mNumFaces; fCount++)
		{
			aiFace f = mesh->mFaces[fCount];
			// index numbers
			for(unsigned int iCount = 0; iCount < f.mNumIndices; iCount++)
			{
				geometry->getIndex()->push_back(f.mIndices[iCount]);
			}
		}

		geometry->createBuffers();
		
		// new child node with the geometry and a connection to material
		CVK::Node* child = new CVK::Node();
		child->setGeometry(geometry);
		child->setMaterial(materials[mesh->mMaterialIndex]);
		addChild(child);
	}
}

void CVK::Node::render()
{
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	CVK::Node* parentNode = m_parent;
	while(parentNode != 0)
	{
		modelMatrix = *parentNode->getModelMatrix() * modelMatrix;
		parentNode = parentNode->getParent();
	}
	render( modelMatrix);
}

void CVK::Node::render( glm::mat4 modelMatrix)
{
	// accumulate model matrix
	modelMatrix = modelMatrix * *getModelMatrix();
	
	// update shader
	CVK::State::getInstance()->getShader()->update(this);
	CVK::State::getInstance()->getShader()->updateModelMatrix(modelMatrix);
	
	// render geometry
	if ( hasGeometry())
		m_geometry->render();
	
	// render childs
	for(unsigned int i=0; i<m_childs.size(); i++)
	{
		m_childs[i]->render( modelMatrix);
	}
}

CVK::Node* CVK::Node::find(std::string name)
{
	if(m_name == name) return this;
	for(unsigned int i=0; i<m_childs.size(); i++)
	{
		CVK::Node* result = m_childs[i]->find(name);
		if(result != 0) return result;
	}
	return 0;
}

void CVK::Node::setName(std::string name)
{
	m_name = name;
}

std::string* CVK::Node::getName()
{
	return &m_name;
}

void CVK::Node::setGeometry(CVK::Geometry* geometry)
{
	m_geometry = geometry;
}

CVK::Geometry* CVK::Node::getGeometry()
{
	return m_geometry;
}

bool CVK::Node::hasGeometry()
{
	return (m_geometry != NULL);
}

void CVK::Node::setMaterial(CVK::Material* material)
{
	m_material = material;
}

CVK::Material* CVK::Node::getMaterial()
{
	return m_material;
}

bool CVK::Node::hasMaterial()
{
	return (m_material != NULL);
}

void CVK::Node::setModelMatrix(glm::mat4 modelMatrix)
{
	m_modelMatrix = modelMatrix;
}

glm::mat4* CVK::Node::getModelMatrix()
{
	return &m_modelMatrix;
}

void CVK::Node::setParent( CVK::Node* node)
{
	m_parent = node;
}

CVK::Node* CVK::Node::getParent()
{
	return m_parent;
}

void CVK::Node::addChild(CVK::Node* node)
{
	m_childs.push_back(node);
	node->setParent(this);
}

std::vector<CVK::Node*>* CVK::Node::getChilds()
{
	return &m_childs;
}
