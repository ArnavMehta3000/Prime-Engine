#pragma once
#include "Prime/Core/IService.h"

namespace Prime
{
	class Locator
	{
	public:
		static Locator* Instance();

		template <typename T>
		static void RegisterService()
		{
			Instance()->Register<T>();
		}

		template <typename T>
		static T& ResolveService()
		{
			return Instance()->Resolve<T>();
		}

		template <typename T>
		static bool HasService()
		{
			return Instance()->Has<T>();
		}
	private:
		Locator() {}
		static Locator* s_instance;

		template <typename T>
		void Register()
		{
			auto& serviceType = typeid(T);
			m_services[serviceType.name()] = std::make_shared<T>();
			std::cout << "Registering Service: " << serviceType.name() << std::endl;			
		}

		template <typename T>
		T& Resolve()
		{
			IService* service = m_services.at(typeid(T).name()).get();
			std::cout << "Resolving Service: " << typeid(T).name() << std::endl;
			return *static_cast<T*>(service);
		}

		template <typename T>
		bool Has()
		{
			if (m_services.find(typeid(T).name()) == m_services.end())
				return false;

			return true;
		}

	private:
		 std::unordered_map<const char*, std::shared_ptr<IService>> m_services;
	};
}

