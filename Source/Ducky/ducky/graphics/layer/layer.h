#pragma once

#include "ducky_pch.h"
#include "../opengl.h"
#include "../light.h"
#include "../camera.h"

namespace ducky { namespace graphics {

	class Layer
	{
	public:
		static void SortLayers(vector<Layer*>& list)
		{
			if(list.size() != 0)
				qsort(&list[0], list.size(), sizeof(Layer*), CompareLayers);
		}
	private:
		static int CompareLayers(const void* a, const void* b)
		{
			if ((*(Layer**)a)->m_ZIndex < (*(Layer**)b)->m_ZIndex)
				return 1;
			else if ((*(Layer**)a)->m_ZIndex > (*(Layer**)b)->m_ZIndex)
				return -1;
			return 0;
		}
	public:
		Layer(const string& n, float zindex) : m_Name(n), m_ZIndex(zindex) {}
		virtual ~Layer() {}

		string GetName() { return m_Name; }
		float GetZIndex() { return m_ZIndex; }
		Camera* GetCamera() { return m_Camera; }
		void SetCamera(Camera* c) { m_Camera = c; }

		virtual void Render(LightSetup*) = 0;
	protected:
		Camera* m_Camera;
		// The smaller this is, the closer it is to the camera
		float m_ZIndex = 0;
		string m_Name;
	};

} }