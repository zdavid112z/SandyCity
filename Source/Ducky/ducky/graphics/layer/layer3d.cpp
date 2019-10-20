#include "ducky_pch.h"
#include "layer3d.h"

namespace ducky { namespace graphics {

	Layer3D::Layer3D() :
		Layer(GetLayerName(), 0)
	{

	}

	Layer3D::~Layer3D()
	{
		for (uint i = 0; i < m_Layers.size(); i++)
			delete m_Layers[i];
		m_Layers.clear();
	}

	SimpleLayer3D* Layer3D::GetLayer(Shader* s)
	{
		for (uint i = 0; i < m_Layers.size(); i++)
			if (m_Layers[i]->GetShader() == s)
				return m_Layers[i];
		return nullptr;
	}

	void Layer3D::AddDrawCall(const DrawCall3D& dc)
	{
		for (uint i = 0; i < m_Layers.size(); i++)
		{
			if (dc.shader == m_Layers[i]->GetShader())
			{
				m_Layers[i]->AddDrawCall(dc);
				return;
			}
		}
		SimpleLayer3D* layer = new SimpleLayer3D(dc.shader);
		layer->AddDrawCall(dc);
		m_Layers.push_back(layer);
	}

	void Layer3D::Render(LightSetup* ls)
	{
		for (uint i = 0; i < m_Layers.size(); i++)
			m_Layers[i]->Render(ls);
	}

} }