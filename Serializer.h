#ifndef __Serializer_H__
#define __Serializer_H__

class GameObject;

class Serializer
{
public:
	Serializer();
	~Serializer();

	void saveObject(GameObject* obj);

private:

};

#endif