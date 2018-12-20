#include "Serializer.h"



Serializer::Serializer()
{
}


Serializer::~Serializer()
{
}

JSON_File* Serializer::openReadJSON(const char* path) {
	FILE* fp = fopen(path, "rb");
	char readBuffer[65536];

	if (fp != nullptr)
		return new JSON_File(new rapidjson::FileReadStream(fp, readBuffer, sizeof(readBuffer)), fp);
	else
		return nullptr;
}

JSON_File* Serializer::openWriteJSON(const char* path) {
	FILE* fp = fopen(path, "wb");
	char writeBuffer[65536];

	if (fp != nullptr)
		return new JSON_File(new rapidjson::FileWriteStream(fp, writeBuffer, sizeof(writeBuffer)), fp);
	else
		return nullptr;
}

void Serializer::loadScene(const char* path, GameObject* newBase) {
	JSON_File* json = openReadJSON(path);
	//now we go over the json puting the values into the game object
}

void Serializer::saveScene(const char* path, GameObject* oldBase) {
	JSON_File* json = openWriteJSON(path);
	//now we go over the object writing in the json
}

//Structure functions
JSON_File::JSON_File(rapidjson::FileWriteStream* os, FILE* fp) : os(os), fp(fp)
{
	document = new rapidjson::Document();
	document->SetObject();

	allocator = &document->GetAllocator();
}

JSON_File::JSON_File(rapidjson::FileReadStream* is, FILE* fp) : is(is), fp(fp)
{
	document = new rapidjson::Document();
	document->ParseStream(*is);

	allocator = &document->GetAllocator();
}

JSON_File::JSON_File()
{
	document = new rapidjson::Document();
	document->SetObject();

	allocator = &document->GetAllocator();
}

JSON_File::~JSON_File()
{
	int size = allocatedValues.size();
	for (int i = 0; i < size; i++)
	{
		RELEASE(allocatedValues[i]);
	}
	allocatedValues.clear();

	RELEASE(document);

	RELEASE(is);
	RELEASE(os);
}

bool JSON_File::Write()
{
	if (os != nullptr)
	{
		//rapidjson::Writer<rapidjson::FileWriteStream> writer(*os);
		rapidjson::PrettyWriter<rapidjson::FileWriteStream> writer(*os);
		document->Accept(writer);

		return true;
	}
	else
	{
		if (is != nullptr)
		{
			LOG("Cannot write a read only file!");
		}
		else
		{
			LOG("Cannot write a virtual file!");
		}

		return false;
	}
}
JSON_Value* JSON_File::createValue()
{
	JSON_Value* ret = new JSON_Value(allocator);
	allocatedValues.push_back(ret);
	return ret;
}

void JSON_File::addValue(const char * name, JSON_Value* value)
{
	std::string str = name;
	rapidjson::Value index(str.c_str(), str.size(), *allocator);
	document->AddMember(index, *value->getRapidJSONValue(), *allocator);
}

JSON_Value* JSON_File::getValue(const char* name)
{
	if (document->IsObject() && document->HasMember(name))
	{
		rapidjson::Value& value = document->operator[](name);
		JSON_Value* ret = new JSON_Value(allocator);
		allocatedValues.push_back(ret);
		ret->getRapidJSONValue()->CopyFrom(value, *allocator, false);

		return ret;
	}

	return nullptr;
}

void JSON_File::setValue(const char * name, JSON_Value * value)
{
	if (this->document->HasMember(name))
		this->document->operator[](name) = *value->getRapidJSONValue(); //If it exists modify it
	else
		this->addValue(name, value); //if not, set a new one
}

JSON_Value::~JSON_Value()
{
	int size = allocatedValues.size();
	for (int i = 0; i < size; i++)
	{
		RELEASE(allocatedValues[i]);//deletes buffer
	}
	allocatedValues.clear();

	RELEASE(value);
}

void JSON_Value::convertToArray()
{
	RELEASE(value);
	value = new rapidjson::Value(rapidjson::kArrayType);
}

void JSON_Value::addInt(const char* name, int value)
{
	std::string str = name;
	rapidjson::Value index(str.c_str(), str.size(), *allocator);
	this->value->AddMember(index, value, *allocator);
}