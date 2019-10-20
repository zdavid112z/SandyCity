#pragma once

#include "../entity.h"
#include "../scene.h"
#include "graphics\texture2d.h"
#include "transformComponent.h"
#include "utils\debug.h"
#include "graphics\layer\layer2d.h"

namespace ducky { namespace core {

	class SpriteComponent : public Component
	{
	public:
		SpriteComponent(uint c1 = 0xffffffff, uint c2 = 0xffffffff, uint c3 = 0xffffffff, uint c4 = 0xffffffff, const glm::vec4& sizePosition = glm::vec4(1, 1, 0, 0), float zindex = 0, graphics::Texture2D* mask = nullptr, const glm::vec4& minmax = glm::vec4(0, 0, 1, 1))
		{
			m_Texture = nullptr;
			m_Mask = mask;
			m_TexCoords = minmax;
			m_SizePosition = sizePosition;
			m_Color1 = c1;
			m_Color2 = c2;
			m_Color3 = c3;
			m_Color4 = c4;
			m_ZIndex = zindex;
		}
		SpriteComponent(graphics::Texture2D* tex, graphics::Texture2D* mask = nullptr, uint spriteW = 1, uint spriteH = 1, uint spriteX = 0, uint spriteY = 0, const glm::vec4& sizePosition = glm::vec4(1, 1, 0, 0), uint c1 = 0xffffffff, uint c2 = 0xffffffff, uint c3 = 0xffffffff, uint c4 = 0xffffffff, float zindex = 0)
		{
			m_Texture = tex;
			m_Mask = mask;
			m_Color1 = c1;
			m_Color2 = c2;
			m_Color3 = c3;
			m_Color4 = c4;
			m_ZIndex = zindex;
			m_SpriteW = spriteW;
			m_SpriteH = spriteH;
			m_SpriteX = spriteX;
			m_SpriteY = spriteY;
			m_SizePosition = sizePosition;
		}

		void Init() override
		{
			TransformComponent* tr = m_Parent->GetComponent<TransformComponent>();
			if (tr == nullptr)
				Debug::Printer << ERR << TLOG << "Adding a sprite component requires a transform component!" << ENDL;
			else
			{
				//glm::vec3 v = glm::vec3(tr->GetTransform().GetScale().x, tr->GetTransform().GetScale().y, 0);
				//m_OffsetMatrix = glm::translate(v / 2.0f);
			}
		}

		string GetType() { return GetComponentType(); }
		static string GetComponentType() { return "Sprite"; }

		bool CullTest()
		{
			// not precise for efficiency
			float radius = sqrt(m_SizePosition.x * m_SizePosition.x + m_SizePosition.y * m_SizePosition.y);
			//float radius = m_SizePosition.z + m_SizePosition.w;
			TransformComponent* tr = m_Parent->GetComponent<TransformComponent>();
			glm::mat4 model = tr->GetTransform().GetLocalModel() * graphics::TransformStack::GetBack();
			glm::vec2 position = glm::vec2(model * glm::vec4(0, 0, 0, 1));

			//////////////////////////////////
			//                              //
			//  TODO: Cache camera details  //
			//                              //
			//////////////////////////////////

			glm::vec2 camPosition = glm::vec2(m_Parent->GetScene()->GetLayer<graphics::Layer2D>()->GetCamera()->GetPosition());
			
			glm::vec2 deltaPos = glm::vec2(
				abs(position.x - camPosition.x),
				abs(position.y - camPosition.y));
			if (deltaPos.x <= radius + 16 && deltaPos.y <= radius + 9)
				return false;
			return true;
		}

		void Render()
		{
			TransformComponent* tr = m_Parent->GetComponent<TransformComponent>();
			glm::mat4 model = tr->GetTransform().GetLocalModel() * graphics::TransformStack::GetBack();
			if (m_Texture == nullptr)
				m_Parent->GetScene()->GetLayer<graphics::Layer2D>()->DrawRect(model, m_SizePosition, m_Color1, m_Color2, m_Color3, m_Color4, m_ZIndex, m_Mask, m_TexCoords);
			else m_Parent->GetScene()->GetLayer<graphics::Layer2D>()->DrawImage(model, m_SizePosition, m_Texture, m_ZIndex, m_SpriteW, m_SpriteH, m_SpriteX, m_SpriteY, false, false, glm::vec2(0, 1), m_Mask, m_Color1, m_Color2, m_Color3, m_Color4);
		}
	private:
		graphics::Texture2D* m_Texture;
		graphics::Texture2D* m_Mask;
		glm::vec4 m_TexCoords;
		glm::vec4 m_SizePosition;
		uint m_SpriteW, m_SpriteH, m_SpriteX, m_SpriteY;
		uint m_Color1, m_Color2, m_Color3, m_Color4;
		float m_ZIndex;
	};

} }