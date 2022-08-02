#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class SeenObject :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "SeenObject";
		}
		void OnUpdate()override;
		void OnSet()override;
		void Start()override;
		Value_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}

		void AddObserverCount()
		{
			m_observerCount++; 
			SetIsRemove(false);
		}
		void RemoveObserverCount()
		{
			m_observerCount--; 
			if (m_observerCount <= 0)
			{
				SetIsRemove(true);
			}
		}

	private:
		Value_ptr<RelativeTimer> m_vlp_auraIntervalTimer;
		Value_weak_ptr<GameObject> m_vwp_observer;

		std::int8_t m_observerCount;
	};

}
BUTI_REGIST_GAMECOMPONENT(SeenObject, true)