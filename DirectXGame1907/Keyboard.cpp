#include "Keyboard.h"


// キーリピートを有効にする
void Keyboard::EnableAutoRepeat() noexcept
{
	autoRepeatEnabled = true;
}

// キーリピートを無効にする
void Keyboard::DisableAutoRepeat() noexcept
{
	autoRepeatEnabled = false;
}

// キーリピートが有効か？
bool Keyboard::IsAutoRepeatIsEnabled() const noexcept
{
	return autoRepeatEnabled;
}

void Keyboard::OnKeyDowned(unsigned char keyCode) noexcept
{
	keyStates[keyCode] = true;
	keybuffer.push(Keyboard::Event(Keyboard::Event::KeyType::KEYDOWN, keyCode));
	TrimBuffer(keybuffer);
}

void Keyboard::OnKeyUped(unsigned char keyCode) noexcept
{
	keyStates[keyCode] = false;
	keybuffer.push(Keyboard::Event(Keyboard::Event::KeyType::KEYUP, keyCode));
	TrimBuffer(keybuffer);
}

void Keyboard::OnChar(char character) noexcept
{
	charbuffer.push(character);
	TrimBuffer(charbuffer);
}

void Keyboard::ClearState() noexcept
{
	keyStates.reset();
}

template<typename T>
void Keyboard::TrimBuffer(std::queue<T>& buffer) noexcept
{
	while (buffer.size() > bufferSize)
	{
		buffer.pop();
	}
}
