#include "pch.h"
#include "Map.h"

const int32 Map::GetXLen() const
{
	return MaxX - MinX;
}

const int32 Map::GetYLen() const
{
	return MaxY - MinY;
}

Vector2 Map::PosToCell(Vector2& pos)
{
	return Vector2((int32)pos.X - MinX, MaxY - (int32)pos.Y);
}

Map::Map(ifstream& mapInfo)
{
	string line;
	getline(mapInfo, line);
	MinX = stoi(line);
	getline(mapInfo, line);
	MaxX = stoi(line);
	getline(mapInfo, line);
	MinY = stoi(line);
	getline(mapInfo, line);
	MaxY = stoi(line);
	while (getline(mapInfo, line))
	{
		vector<uint8> vec;
		for (auto c : line)
			vec.push_back(c - '0');
		Cells.push_back(vec);
	}
	mapInfo.clear();
	mapInfo.seekg(0, ios::beg);
}

bool Map::CanGo(Vector2& pos, bool cell) 
{
	Vector2 p(pos);
	if (cell == false)
		p = PosToCell(pos);
	if (!(p.X >= 0 && p.X < Cells[p.Y].size()))
		return false;
	return Cells[p.Y][p.X] == 2;
}

vector<uint8>& Map::operator[](uint32 index)
{
	return Cells[index];
}
