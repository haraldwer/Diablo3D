#pragma once
typedef int EntityID;

class Entity
{
	friend class Scene;
public:
	EntityID GetID() const;
private:
	void Construct(const EntityID anID);
	void Destruct();
	EntityID myID;
};

