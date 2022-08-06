#pragma once
#include "Prime/Core/Service Locator/IService.h"

namespace Prime
{
	class Locator
	{
	public:
		static Locator* Instance();
		static void Shutdown();

		template <typename T>
		static void RegisterService()
		{
			Instance()->Register<T>();
		}

		template <typename T>
		static T* ResolveService()
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

			// Check if service already exists in map
			if (Has<T>())
			{
				WARN("Service type: " + std::string(serviceType.name()) + " already exists. Returning...");
				return;
			}

			m_services[serviceType.name()] = std::make_shared<T>();
			std::cout << "-----Registering Service: " << serviceType.name()
					  << "Service ID : " << m_services[serviceType.name()]->GetServiceId()
					  << "-----" << std::endl;
		}

		template <typename T>
		T* Resolve()
		{
			if (!Has<T>())
			{
				WARN("Service type: " + std::string(typeid(T).name()) + " not found. Returning nullptr");
				return nullptr;
			}

			IService* service = m_services.at(typeid(T).name()).get();
			return static_cast<T*>(service);
		}

		template <typename T>
		bool Has()
		{
			if (m_services.find(typeid(T).name()) == m_services.end())
				return false;

			return true;
		}

		void ShutdownImpl();

	private:
		 std::unordered_map<const char*, std::shared_ptr<IService>> m_services;
	};
}

