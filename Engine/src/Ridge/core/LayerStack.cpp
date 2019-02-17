#include "RidgePCH.h"
#include "LayerStack.h"

Ridge::LayerStack::LayerStack()
{
	m_layerInsert = m_layers.begin();
}

Ridge::LayerStack::~LayerStack()
{
	for (Layer* layer : m_layers)
		delete layer;
}

void Ridge::LayerStack::PushLayer(Layer* layer)
{
	m_layerInsert = m_layers.emplace(m_layerInsert, layer);
	layer->OnAttach();
}

void Ridge::LayerStack::PushOverlay(Layer* overlay)
{
	m_layers.emplace_back(overlay);
	overlay->OnAttach();
}

void Ridge::LayerStack::PopLayer(Layer* layer)
{
	auto it = std::find(m_layers.begin(), m_layers.end(), layer);
	if (it != m_layers.end())
	{
		m_layers.erase(it);
		m_layerInsert--;
	}
	layer->OnDetach();
}

void Ridge::LayerStack::PopOverlay(Layer* overlay)
{
	auto it = std::find(m_layers.begin(), m_layers.end(), overlay);
	if (it != m_layers.end())
	{
		m_layers.erase(it);
	}
	overlay->OnDetach();
}
