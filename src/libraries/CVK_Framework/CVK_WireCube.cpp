#include "CVK_WireCube.h"

CVK::WireCube::WireCube() : Cube()
{
	create( 1.0f);
	
	m_index.clear();

	for(int i=0; i<6; i++)
	{
		m_index.push_back( i*4+0);
		m_index.push_back( i*4+1);
		m_index.push_back( i*4+2);
		m_index.push_back( i*4+3);
	}
	
	m_indices = m_index.size();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexlist);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices * sizeof(unsigned int), &m_index[0] , GL_STATIC_DRAW);
}

CVK::WireCube::WireCube(float size) : Cube(size)
{
	m_index.clear();

	for(int i=0; i<6; i++)
	{
		m_index.push_back( i*4+0);
		m_index.push_back( i*4+1);
		m_index.push_back( i*4+1);
		m_index.push_back( i*4+2);
		m_index.push_back( i*4+2);
		m_index.push_back( i*4+3);
		m_index.push_back( i*4+3);
		m_index.push_back( i*4+0);
	}
	
	m_indices = m_index.size();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexlist);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices * sizeof(unsigned int), &m_index[0] , GL_STATIC_DRAW);
}

CVK::WireCube::~WireCube()
{
}

void CVK::WireCube::render()
{
	glBindVertexArray(m_vao);

	glDrawElements(GL_LINES, m_indices, GL_UNSIGNED_INT, 0);
}

