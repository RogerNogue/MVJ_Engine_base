#ifndef __Serializer_H__
#define __Serializer_H__

#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/filereadstream.h"
#include "MathGeoLib.h"

class GameObject;
class ComponentMesh;
class ComponentMaterial;
class ComponentTransform;

struct JSON_Value
{
public:

	JSON_Value(rapidjson::Document::AllocatorType* allocator) : allocator(allocator)
	{
		value = new rapidjson::Value(rapidjson::kObjectType);
	}

	~JSON_Value();

	//This will remove all added elements of the JSON_Value
	void convertToArray();

	void addInt(const char* name, int value);
	void addUint(const char* name, unsigned int value);
	void addFloat(const char* name, float value);
	void addString(const char* name, const char* value);
	void addBool(const char* name, bool value);
	void addVector2(const char* name, float2 vec);
	void addVector3(const char* name, float3 vec);
	void addQuat(const char* name, Quat quat);
	void addTransformMat(const char* name, float4x4 mat);

	void setUint(const char* name, unsigned int value);

	int getInt(const char* name);
	unsigned int getUint(const char* name);
	float getFloat(const char* name);
	const char* getString(const char* name);
	bool getBool(const char* name);
	float2 getVector2(const char* name);
	float3 getVector3(const char* name);
	Quat getQuat(const char* name);
	float4x4 getTransform(const char* name);

	JSON_Value* createValue();
	void addValue(const char* name, JSON_Value* value);
	JSON_Value* getValue(const char* name);
	JSON_Value* getValueFromArray(int index);
	void setValue(const char* name, JSON_Value* value);

	void setValue(rapidjson::Value* value);
	rapidjson::Value* getRapidJSONValue();

private:

	rapidjson::Value* value = nullptr;
	rapidjson::Document::AllocatorType* allocator = nullptr;
	std::vector<JSON_Value*> allocatedValues;
};

struct JSON_File
{
public:

	JSON_File(rapidjson::FileWriteStream* os, FILE* fp);
	JSON_File(rapidjson::FileReadStream* is, FILE* fp);
	JSON_File();
	~JSON_File();

	bool Write();

	JSON_Value* createValue();
	void addValue(const char* name, JSON_Value* value);
	JSON_Value* getValue(const char* name);
	void setValue(const char* name, JSON_Value* value);

	void closeFile();

private:

	FILE* fp = nullptr;

	rapidjson::Document* document = nullptr;
	rapidjson::FileWriteStream* writeStream = nullptr;
	rapidjson::FileReadStream* readStream = nullptr;

	rapidjson::Document::AllocatorType* allocator = nullptr;
	std::vector<JSON_Value*> allocatedValues;
};

class Serializer
{
public:
	Serializer();
	~Serializer();

	void loadScene(const char* path, GameObject* newBase);
	void saveScene(const char* path, GameObject* oldBase);
	void saveMesh(ComponentMesh* mesh);
	void saveMaterial(ComponentMaterial* mat);
	void saveTransform(ComponentTransform* transf);

private:
	JSON_File* openReadJSON(const char* path);
	JSON_File* openWriteJSON(const char* path);
};

#endif