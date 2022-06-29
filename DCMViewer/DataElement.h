#pragma once

#include <vector>
#include <string>
#include <memory>
#include <stdexcept>

enum class DataElementTR : int16_t {
	  TR_EMPTY = 0x0000
	, TR_AE = 0x4541
	, TR_OW = 0x574f
	, TR_AS = 0x5341
	, TR_PN = 0x4e50
	, TR_AT = 0x5441
	, TR_SH = 0x4853
	, TR_CS = 0x5343
	, TR_SL = 0x4c53
	, TR_DS = 0x5344
	, TR_SQ = 0x5153
	, TR_DA = 0x4144
	, TR_SS = 0x5353
	, TR_DT = 0x5444
	, TR_ST = 0x5453
	, TR_FL = 0x4c46
	, TR_TM = 0x4d54
	, TR_FD = 0x4446
	, TR_UI = 0x4955
	, TR_IS = 0x5349
	, TR_UL = 0x4c55
	, TR_LO = 0x4f4c
	, TR_UN = 0x4e55
	, TR_LT = 0x544c
	, TR_US = 0x5355
	, TR_OB = 0x424f
};

struct DataElement;
using DataElementPtr = std::shared_ptr<DataElement>;

struct DataElement {
	__int32 _dicomTag;
	DataElementTR _tr;
	union {
		__int16 _vr16;
		__int32 _vr32;
	} _vr;

	std::vector<char> _dataArray;

	DataElement(__int32 dicomTag) : _dicomTag(dicomTag) ,
		_tr(DataElementTR::TR_EMPTY),
		_vr{ 0 },
		_dataArray(0)
	{

	}

	bool Load(CFile& file);

	unsigned long getLongValue() {
		switch (_tr) {
		case DataElementTR::TR_UL:
			if (_dataArray.size() >= sizeof(unsigned long)) {
				return *((unsigned long*)_dataArray.data());
			}
			break;
		}
		throw std::runtime_error("DataElementTR::TR_UL type xecption ");
	}

	std::string getStringValue() {
		switch (_tr) {
		case DataElementTR::TR_OB:
			return std::string(_dataArray.cbegin(), _dataArray.cend());
		}
		throw std::runtime_error("DataElementTR::TR_OB type xecption ");
	}

private: 
	bool readVr(CFile& file) {
		if (sizeof(_vr._vr16) != file.Read(&_vr._vr16, sizeof(_vr._vr16))) {
			TRACE("DataElementTR vr read error\n");
			return false;
		}
		return true;
	}
	bool readVr32(CFile& file) {
		if (sizeof(_vr._vr32) != file.Read(&_vr._vr32, sizeof(_vr._vr32))) {
			TRACE("DataElementTR vr read error\n");
			return false;
		}
		return true;
	}

	bool readVrContent(CFile& file) {
		if (_vr._vr16 == 0) return true;

		//저장 배열의 크기를 변경한다
		_dataArray.resize(_vr._vr16);
		if (_vr._vr16 != file.Read(_dataArray.data(), _vr._vr16)) {
			TRACE("DataElementTR content read error\n");
			return false;
		}
		return true;
	}

	bool readVrContent32(CFile& file) {
		if (_vr._vr32 == 0) return true;

		//저장 배열의 크기를 변경한다
		_dataArray.resize(_vr._vr32);
		if (_vr._vr32 != file.Read(_dataArray.data(), _vr._vr32)) {
			TRACE("DataElementTR content read 32 error\n");
			return false;
		}
		return true;
	}

};