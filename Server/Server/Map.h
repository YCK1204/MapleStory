#pragma once

class Map
{
public:
	int32 MinX;
	int32 MinY;
	int32 MaxX;
	int32 MaxY;

	vector<vector<uint8>> Cells;
private:
	const int32 GetXLen() const;
	const int32 GetYLen() const;
	Vector2 PosToCell(Vector2& pos);
public:
	Map(ifstream& mapInfo);
	bool CanGo(Vector2& pos, bool cell = false);
};