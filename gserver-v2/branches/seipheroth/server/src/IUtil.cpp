#include "IUtil.h"
#include "CString.h"

static const char* const clientVersions[] =
{
	"",		// CLVER_1_32,
	"",		// CLVER_1_322,
	"",		// CLVER_1_323,
	"",		// CLVER_1_324,
	"",		// CLVER_1_33,
	"",		// CLVER_1_331,
	"",		// CLVER_1_34,
	"",		// CLVER_1_341,
	"",		// CLVER_1_35,
	"",		// CLVER_1_36,
	"",		// CLVER_1_37,
	"",		// CLVER_1_371,
	"",		// CLVER_1_38,
	"",		// CLVER_1_381,
	"",		// CLVER_1_39,
	"",		// CLVER_1_391,
	"",		// CLVER_1_392,
	"",		// CLVER_1_4,
	"",		// CLVER_1_41,
	"",		// CLVER_1_411,
	"",		// CLVER_2,
	"",		// CLVER_2_01,
	"",		// CLVER_2_02,
	"",		// CLVER_2_021,
	"",		// CLVER_2_03,
	"",		// CLVER_2_04,
	"",		// CLVER_2_05,
	"",		// CLVER_2_052,
	"",		// CLVER_2_1,
	"",		// CLVER_2_101,
	"",		// CLVER_2_102,
	"",		// CLVER_2_103,
	"",		// CLVER_2_12,
	"",		// CLVER_2_121,
	"",		// CLVER_2_13,
	"",		// CLVER_2_133,
	"",		// CLVER_2_134,
	"",		// CLVER_2_14,
	"",		// CLVER_2_144,
	"",		// CLVER_2_145,
	"",		// CLVER_2_146,
	"",		// CLVER_2_147,
	"",		// CLVER_2_15,
	"",		// CLVER_2_152,
	"",		// CLVER_2_16,
	"",		// CLVER_2_161,
	"",		// CLVER_2_162,
	"",		// CLVER_2_163,
	"",		// CLVER_2_164,
	"",		// CLVER_2_165,
	"",		// CLVER_2_166,
	"",		// CLVER_2_167,
	"",		// CLVER_2_17,
	"GNW22122",		// CLVER_2_171,
	"",		// CLVER_2_18,
	"",		// CLVER_2_181,
	"",		// CLVER_2_19,
	"",		// CLVER_2_191,
	"",		// CLVER_2_2,
	"",		// CLVER_2_21,
	"",		// CLVER_2_22,
	"",		// CLVER_2_3,
	"GNW28015",		// CLVER_2_31,

	"G3D14097",		// CLVER_5_12
	0
};

int getVersionID(const CString& version)
{
	int i = 0;
	while (clientVersions[i] != 0)
	{
		if (version == CString(clientVersions[i]))
			return i;
		++i;
	}
	return -1;
}
