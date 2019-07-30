#include "Keyboard.h"


// �L�[���s�[�g��L���ɂ���
void Keyboard::EnableAutoRepeat() noexcept
{
	autoRepeatEnabled = true;
}

// �L�[���s�[�g�𖳌��ɂ���
void Keyboard::DisableAutoRepeat() noexcept
{
	autoRepeatEnabled = false;
}

// �L�[���s�[�g���L�����H
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
