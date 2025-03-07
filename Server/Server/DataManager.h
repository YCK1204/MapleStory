#pragma once

class DataManager
{
public:
	typedef struct {
		uint8 TargetId;
		string SceneId;
		float X;
		float Y;
	} Portal;
	typedef shared_ptr<Portal> PortalRef;
private:
	map<uint8, shared_ptr<Monster>> _monsters;
	map<uint8, PortalRef> _portals;
	static DataManager* _instance;
public:
	static DataManager& Instance();
	void Init(json& dataJson);
	shared_ptr<Monster> MonsterClone(uint8 monsterId);
	PortalRef FindPortal(uint8& id);
};
