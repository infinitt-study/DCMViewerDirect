#include "pch.h"
#include "DataElement.h"

bool DataElement::Load(CFile& file) {
	if (sizeof(_tr) != file.Read(&_tr, sizeof(_tr))) {
		TRACE("DataElement::Load tr read error\n");
		return false;
	}

	switch (_tr) {
	case DataElementTR::TR_UL:
	case DataElementTR::TR_UI:
	case DataElementTR::TR_SH:
	case DataElementTR::TR_CS:
	case DataElementTR::TR_DA:
	case DataElementTR::TR_TM:
	case DataElementTR::TR_LO:
	case DataElementTR::TR_PN:
	case DataElementTR::TR_DS:
	case DataElementTR::TR_IS:
	case DataElementTR::TR_LT:
	case DataElementTR::TR_US:
	case DataElementTR::TR_AE:
	case DataElementTR::TR_AS:
	case DataElementTR::TR_ST:
		if (false == readVr(file)) {
			return false;
		}
		if (false == readVrContent(file)) {
			return false;
		}
		break;
	case DataElementTR::TR_SQ:
		if (false == readVr(file)) {
			return false;
		}
		if (false == readVrContent(file)) {
			return false;
		}
		file.Seek(4, CFile::current);
		break;
	case DataElementTR::TR_OB:
	case DataElementTR::TR_OW:
		if (false == readVr(file)) {
			return false;
		}
		if (false == readVr32(file)) {
			return false;
		}
		if (false == readVrContent32(file)) {
			return false;
		}
		break;
	}
	return true;
}
