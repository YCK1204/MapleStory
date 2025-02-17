#pragma once

class Channel
{
private:
	uint8 _id;
	uint16 _userCount = 0;
public:
	Channel();
	~Channel();
public:
	void Init(uint8 id);
	const uint16& GetUserCount() const;
};