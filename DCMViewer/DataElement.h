#pragma once

#include <vector>
#include <string>

struct DataElement {
	__int32 _dicomTag;
	__int16 _tr;
	__int16 _vr;
	std::vector<char> _dataArray;
	
	DataElement(__int32 dicomTag,
		__int16 tr) : _dicomTag(dicomTag) ,
		_tr(tr), 
		_vr(0), 
		_dataArray(0)
	{

	}

	bool Load();
};