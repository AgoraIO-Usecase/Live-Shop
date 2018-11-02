#include"stdafx.h"
#include "ProductAdvertise.h"
#include "IHQDataBase.h"
#include "commonFun.h"

CProductAdvertise::CProductAdvertise(int id)
	: type("prod")
	, id(id)
	, productId("")
	, title("")
	, sold(0)
	, price(0.0f)
	, currrency("$")
	, groupDeal(0.0f)
	, streamId(2)
{
	
}

CProductAdModule* getProductAdModule()
{
	unsigned int uid = 0;
	static CProductAdModule prodModule;
	return &prodModule;
}
CProductAdModule::CProductAdModule()
	: cur_index(-1)
	, count(0)
	, streamId(0)
{
	streamId = strtoul(gHQConfig.getAdvertiseUid().c_str(), NULL, 0);
	count = getHQDataBase()->sqlGetProductAdvertiseCount();
	if (count > 0){
		_getFirstProductAdvertise();
	}
}

int CProductAdModule::_getFirstProductAdvertise()
{
	CProductAdvertise productAdvertise(cur_index + 1);
	if (getHQDataBase()->sqlGetProductAdvertise(productAdvertise.id, &productAdvertise)){
		//m_vecProducts.push_back(productAdvertise);
	}
	return 0;
}

int CProductAdModule::_getNextProductAdvertise()
{
	CProductAdvertise productAdvertise(cur_index + 1);
	if (getHQDataBase()->sqlGetProductAdvertise(productAdvertise.id, &productAdvertise)){
		m_vecProducts.push_back(productAdvertise);
		cur_index++;
	}
	return cur_index;
}

std::string CProductAdModule::getCurrentProductJson()
{
	if (count == 0 || cur_index == -1)
		return "";

	CProductAdvertise advertise = m_vecProducts[cur_index];
	
	rapidjson::Document document;
	document.SetObject();
	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
	
	document.AddMember("type", advertise.type.c_str(), allocator);
	
	rapidjson::Value product(rapidjson::kObjectType);
	product.AddMember("id", advertise.productId.c_str(), allocator);
	product.AddMember("title", advertise.title.c_str(), allocator);

	rapidjson::Value variants(kArrayType);
	rapidjson::Value Color(kObjectType);
	rapidjson::Value colorValues(kArrayType);
	Color.AddMember("name", "Color", allocator);
	int num = advertise.m_vecColors.size();
	for (size_t i = 0; i < num; i++){
		colorValues.PushBack(advertise.m_vecColors[i].c_str(), allocator);
	}
	Color.AddMember("values", colorValues, allocator);
	variants.PushBack(Color, allocator);

	product.AddMember("variants", variants, allocator);

	product.AddMember("sold", advertise.sold, allocator);
	product.AddMember("price", advertise.price, allocator);
	product.AddMember("currency", advertise.currrency.c_str(), allocator);
	product.AddMember("groupDeal", advertise.groupDeal, allocator);
	product.AddMember("streamId", advertise.streamId, allocator);

	document.AddMember("product", product, allocator);

	rapidjson::StringBuffer buffer;
	rapidjson::PrettyWriter<StringBuffer> pwriter(buffer);
	document.Accept(pwriter);

	return buffer.GetString();
}

CProductAdvertise CProductAdModule::next()
{
	_getNextProductAdvertise();
	return m_vecProducts[cur_index];
}

CProductAdvertise CProductAdModule::current()
{
	return m_vecProducts[cur_index];
}

bool CProductAdModule::IsLastProduct()
{
	if (count == 0)
		return false;
	return cur_index == count - 1;
}

