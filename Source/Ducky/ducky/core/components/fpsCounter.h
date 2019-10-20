#pragma once

#include "../entity.h"
#include "../scene.h"
#include "io/fontio.h"
#include "graphics/texture2d.h"
#include "graphics\layer\layer2d.h"
#include "graphics\window.h"
#include "utils\stringUtils.h"

namespace ducky { namespace core {

	class FPSCounterComponent : public Component
	{
	public:
		FPSCounterComponent(io::FontData* fd, graphics::Texture2D* atlas) :
			m_FontData(fd),
			m_Atlas(atlas)
		{
			m_Timer = 0;
		}

		string GetType() { return GetComponentType(); }
		static string GetComponentType() { return "FPSCounter"; }

		void RenderGUI()
		{
			m_CurrentFPS++;
			m_Timer += graphics::Window::Instance->GetDeltaTimeRaw();
			if (m_Timer >= 1)
			{
				m_Timer--;
				m_DisplayFPS = m_CurrentFPS;
				m_CurrentFPS = 0;
			}
			//glm::mat4 model;
			//m_Parent->GetScene()->GetLayer<graphics::Layer2D>()->DrawString(, m_FontData, m_Atlas, nullptr, model, 0.02f, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, true, 0, glm::vec2(0.45f, 0.55f));
			/*m_Parent->GetScene()->GetLayer<graphics::Layer2D>()->DrawString(
				("FPS: " + utils::StringUtils::ToString(m_DisplayFPS, 10)).c_str(),
				m_FontData,
				m_Atlas,
				nullptr,
				glm::translate(glm::vec3(10.5f, 7.9f, 0)),
				0.02f,
				0xffffffff,
				0xffffffff,
				0xffffffff,
				0xffffffff,
				false,
				0,
				glm::vec2(0.47f, 0.57f),
				0xff000000,
				glm::vec4(0.22f, 0.72f, 0.007f, 0.007f))
				;*/
			
			m_Parent->GetScene()->GetLayer<graphics::Layer2D>()->DrawStringSimple(
				("FPS: " + utils::StringUtils::ToString(m_DisplayFPS, 10)).c_str(),
				m_FontData,
				m_Atlas,
				glm::vec3(10.5f, 7.9f, 0),
				0.02f,
				0xffffffff,
				false,
				0,
				glm::vec2(0.47f, 0.57f));

			m_Parent->GetScene()->GetLayer<graphics::Layer2D>()->DrawStringSimple(
				("FPS: " + utils::StringUtils::ToString(m_DisplayFPS, 10)).c_str(),
				m_FontData,
				m_Atlas,
				glm::vec3(10.6f, 7.8f, 0),
				0.02f,
				0xff000000,
				false,
				1,
				glm::vec2(0.32f, 0.62f));
		}
	private:
		uint m_CurrentFPS = 0;
		uint m_DisplayFPS = 0;
		float m_Timer;
		io::FontData* m_FontData;
		graphics::Texture2D* m_Atlas;
	};
	
} }