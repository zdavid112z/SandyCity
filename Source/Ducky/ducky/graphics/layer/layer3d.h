#pragma once

#include "simpleLayer3d.h"

namespace ducky { namespace graphics {

	struct DrawCall3D : SimpleDrawCall
	{
		Shader* shader;
	};

	class Layer3D : public Layer
	{
	public:
		Layer3D();
		virtual ~Layer3D();

		SimpleLayer3D* GetLayer(Shader*);
		void AddDrawCall(const DrawCall3D&);
		void Render(LightSetup*);
		static string GetLayerName() { return "Layer3d"; }
	protected:
		vector<SimpleLayer3D*> m_Layers;
	};

} }