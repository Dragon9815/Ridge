#pragma once

#include "Event.h"
#include "Ridge/Input/Key.h"

namespace Ridge {
	class RIDGE_API KeyEvent : public Event
	{
	public:
		inline Key GetKey() const { return m_key; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		KeyEvent(Key& key)
			: m_key(key) {}

		Key m_key;
	};

	class RIDGE_API KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(Key& key, int repeatCount)
			: KeyEvent(key), m_repeatCount(repeatCount) {}

		inline int GetRepeatCount() const { return m_repeatCount; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_key << " (" << m_repeatCount << " repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)
	private:
		int m_repeatCount;
	};

	class RIDGE_API KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(Key& key)
			: KeyEvent(key) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_key;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	class RIDGE_API CharTypedEvent : public Event
	{
	public:
		CharTypedEvent(int typedChar)
			: m_typedChar(typedChar) {}

		inline int GetTypedChar() { return m_typedChar; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "ChatInputEvent: " << static_cast<char>(m_typedChar);
			return ss.str();
		}
		
		EVENT_CLASS_TYPE(CharInput)
		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	private:
		int m_typedChar;
	};
}