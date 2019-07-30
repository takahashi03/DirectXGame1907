#pragma once
#include <queue>
#include <bitset>
#include <optional>

class Keyboard
{
	friend class Window;
public:
	class Event {
	public:
		enum class KeyType
		{
			KEYDOWN,
			KEYUP,
		};
	private:
		KeyType keyType;
		unsigned char keyCode;
	public:
		Event(KeyType type, unsigned char code)noexcept :
			keyType(type),
			keyCode(code)
		{}
		bool IsKeyUP() const noexcept
		{
			return keyType == KeyType::KEYUP;
		}
		bool IsKeyDown() const noexcept
		{
			return keyType == KeyType::KEYDOWN;
		}
		unsigned char GetCode() const noexcept
		{
			return keyCode;
		}
	};
public:
	Keyboard() = default;
	Keyboard(const Keyboard&) = delete;
	Keyboard& operator = (const Keyboard&) = delete;

	// �J��Ԃ�����
	// �����J��Ԃ���L���ɂ���
	void EnableAutoRepeat() noexcept;
	// �����J��Ԃ��𖳌��ɂ���
	void DisableAutoRepeat() noexcept;
	// �����J��Ԃ����L�����H
	bool IsAutoRepeatIsEnabled() const noexcept;

private:
	void OnKeyDowned(unsigned char keycode) noexcept;
	void OnKeyUped(unsigned char keycode) noexcept;
	void OnChar(char character) noexcept;
	void ClearState() noexcept;
	template<typename T>
	static void TrimBuffer(std::queue<T>&buffer) noexcept;
	
private:
	// �����J��Ԃ��̗L��
	bool autoRepeatEnabled = false;
	static constexpr unsigned int nKey = 256u;
	static constexpr unsigned int bufferSize = 16u;
	std::bitset<nKey> keyStates;
	std::queue<Event> keybuffer;
	std::queue<char> charbuffer;
};
