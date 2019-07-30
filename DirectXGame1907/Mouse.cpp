// WHEEL_DELTA 使用のため
#include <Windows.h>
#include "Mouse.h"


bool Mouse::IsInWindow() const noexcept
{
	return isInWindow;
}

void Mouse::OnMouseMove(int newx, int newy) noexcept
{
	x = newx;
	y = newy;

	buffer.push(Mouse::Event(Mouse::Event::MouseType::MOVE, *this));
	TrimBuffer();
}

void Mouse::OnMouseEnter() noexcept
{
	isInWindow = true;
	buffer.push(Mouse::Event(Mouse::Event::MouseType::ENTER, *this));
	TrimBuffer();
}

void Mouse::OnMouseLeave() noexcept
{
	isInWindow = false;
	buffer.push(Mouse::Event(Mouse::Event::MouseType::LEAVE, *this));
	TrimBuffer();
}


void Mouse::OnLeftDowned(int x, int y) noexcept
{
	leftIsDowned = true;

	buffer.push(Mouse::Event(Mouse::Event::MouseType::LDOWN, *this));
	TrimBuffer();
}

void Mouse::OnLeftUped(int x, int y) noexcept
{
	leftIsDowned = false;

	buffer.push(Mouse::Event(Mouse::Event::MouseType::LUP, *this));
	TrimBuffer();
}

void Mouse::OnRightDowned(int x, int y) noexcept
{
	rightIsDowned = true;

	buffer.push(Mouse::Event(Mouse::Event::MouseType::RDOWN, *this));
	TrimBuffer();
}

void Mouse::OnRightUped(int x, int y) noexcept
{
	rightIsDowned = false;

	buffer.push(Mouse::Event(Mouse::Event::MouseType::RUP, *this));
	TrimBuffer();
}

void Mouse::OnWheelUp(int x, int y) noexcept
{
	buffer.push(Mouse::Event(Mouse::Event::MouseType::WHEELUP, *this));
	TrimBuffer();
}

void Mouse::OnWheelDown(int x, int y) noexcept
{
	buffer.push(Mouse::Event(Mouse::Event::MouseType::WHEELDOWN, *this));
	TrimBuffer();
}

void Mouse::TrimBuffer() noexcept
{
	while (buffer.size() > bufferSize)
	{
		buffer.pop();
	}
}

void Mouse::OnWheelDelta(int x, int y, int delta) noexcept
{
	wheelDeltaCarry += delta;
	// 120ごとにイベントを生成
	while (wheelDeltaCarry >= WHEEL_DELTA)
	{
		wheelDeltaCarry -= WHEEL_DELTA;
		OnWheelUp(x, y);
	}
	while (wheelDeltaCarry <= -WHEEL_DELTA)
	{
		wheelDeltaCarry += WHEEL_DELTA;
		OnWheelDown(x, y);
	}
}