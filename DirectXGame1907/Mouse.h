#pragma once
#include <queue>
#include <optional>

class Mouse
{
	friend class Window;
public:
	class Event
	{
	public:
		enum class MouseType
		{
			// 左
			LDOWN,
			LUP,
			// 右
			RDOWN,
			RUP,
			// ホイール
			WHEELUP,
			WHEELDOWN,
			// 動く
			MOVE,
			// 入る
			ENTER,
			// 出る
			LEAVE,
		};
	private:
		MouseType mouseType;
		bool leftIsDowned;
		bool rightIsDowned;
		int x;
		int y;
	public:
		Event(MouseType type, const Mouse& parent) noexcept
			:
			mouseType(type),
			leftIsDowned(parent.leftIsDowned),
			rightIsDowned(parent.rightIsDowned),
			x(parent.x),
			y(parent.y)
		{}
		MouseType GetType() const noexcept
		{
			return mouseType;
		}
		std::pair<int, int> GetPos() const noexcept
		{
			return{ x,y };
		}
		int GetPosX() const noexcept
		{
			return x;
		}
		int GetPosY() const noexcept
		{
			return y;
		}
		bool LeftIsPressed() const noexcept
		{
			return leftIsDowned;
		}
		bool RightIsPressed() const noexcept
		{
			return rightIsDowned;
		}
	};

public:
	Mouse() = default;
	Mouse(const Mouse&) = delete;
	Mouse& operator=(const Mouse&) = delete;
	bool IsInWindow() const noexcept;

private:
	void OnMouseMove(int x, int y) noexcept;
	void OnMouseEnter() noexcept;
	void OnMouseLeave() noexcept;	
	void OnLeftDowned(int x, int y) noexcept;
	void OnLeftUped(int x, int y) noexcept;
	void OnRightDowned(int x, int y) noexcept;
	void OnRightUped(int x, int y) noexcept;
	void OnWheelUp(int x, int y) noexcept;
	void OnWheelDown(int x, int y) noexcept;
	void TrimBuffer() noexcept;
	void OnWheelDelta(int x, int y, int delta) noexcept;

private:
	static constexpr unsigned int bufferSize = 16u;
	int x;
	int y;
	bool leftIsDowned = false;
	bool rightIsDowned = false;
	bool isInWindow = false;
	int wheelDeltaCarry = 0;
	std::queue<Event> buffer;
};

