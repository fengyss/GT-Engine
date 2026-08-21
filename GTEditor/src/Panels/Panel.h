#pragma once
#include "gtpch.h"

namespace GT
{
	class Panel
	{
	public:
		Panel(const std::string& name) : m_Name(name) {}
		virtual ~Panel() = default;
		virtual void OnImGuiRender() = 0;
		const std::string& GetName() const { return m_Name; }
	protected:
		std::string m_Name;
	};
}