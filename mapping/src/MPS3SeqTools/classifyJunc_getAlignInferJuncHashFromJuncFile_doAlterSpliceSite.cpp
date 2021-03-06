// This file is a part of MapSplice3. Please refer to LICENSE.TXT for the LICENSE
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <stack>
#include <vector>
#include <hash_map>
#include <map>
#include <set>

#include "../general/read_block_test.h"
#include "../general/index_info.h"
#include "../general/alignInferJunctionHash_info.h"

using namespace std;

int main(int argc, char**argv)
{
	if(argc != 5)
	{
		cout << "Executable <InputIndexInfo> <OutputFolderPath> <inputJunc_1> <spliceSiteOffset>" << endl;
		exit(1);
	}
	string alterSpliceSiteOffsetStr = argv[4];
	int alterSpliceSiteOffset = atoi(alterSpliceSiteOffsetStr.c_str());

	string indexFolderPath = argv[1];
	indexFolderPath += "/";
	string indexStr = indexFolderPath;
	string chrom_bit_file = indexStr; chrom_bit_file.append("_chrom"); ifstream chrom_bit_file_ifs(chrom_bit_file.c_str(),ios::binary);
	string indexParameterFileStr = indexFolderPath + "/_parameter";
	ifstream parameter_ifs(indexParameterFileStr.c_str());
	cout << "initiate indexInfo" << endl;
	Index_Info* indexInfo = new Index_Info(parameter_ifs);
	char *chrom; chrom = (char*)malloc((indexInfo->returnIndexSize()) * sizeof(char)); chrom_bit_file_ifs.read((char*)chrom, (indexInfo->returnIndexSize()) * sizeof(char)); 
	cout << "indexSize: " << indexInfo->returnIndexSize() << endl;
	indexInfo->readGenome(chrom);
	cout << "chromSize: " << indexInfo->returnChromStringLength() << endl;
	cout << "start to load every chromosome" << endl;
	indexInfo->initiate();	
	cout << "start to initiate chrNameIndexArray" << endl;
	indexInfo->initiateChrNameIndexArray(1000);
	cout << "finish loading chromosomes" << endl;

	string outputFolderStr = argv[2];
	outputFolderStr += "/";
	string mkdir = "mkdir -p " + outputFolderStr;
	system(mkdir.c_str());	

	string outputSJpath_all = outputFolderStr + "all.alignInferJuncHash";
	string outputSJpath_extended = outputFolderStr + "extendedSJ.alignInferJuncHash";
	string outputSJpath_nonExtended_nonAlterSpliceSite = outputFolderStr + "nonExtended_nonAlterSpliceSite.alignInferJuncHash";
	string outputSJpath_nonExtended_withAlterSpliceSite = outputFolderStr + "nonExtended_withAlterSpliceSite.alignInferJuncHash";
	string outputSJpath_nonExtended_withAlterSpliceSite_valid = outputFolderStr + "nonExtended_withAlterSpliceSite_valid.alignInferJuncHash";
	string outputSJpath_nonExtended_withAlterSpliceSite_invalid = outputFolderStr + "nonExtended_withAlterSpliceSite_invalid.alignInferJuncHash";	
	string outputSJpath_nonExtended_multiAnchorExactlyTheSame = outputFolderStr + "nonExtended_multiAnchorExactlyTheSame.alignInferJuncHash";
	string outputSJpath_kept = outputFolderStr + "keptSJ.alignInferJuncHash";
	string outputSJpath_filterOut = outputFolderStr + "filterOutSJ.alignInferJuncHash";
	string inputJuncFile = argv[3];

	cout << "start to initaite alignInferJunctionHashInfo " << endl;
	AlignInferJunctionHash_Info* alignInferJunctionHashInfo = new AlignInferJunctionHash_Info();
	int chromNum = indexInfo->returnChromNum();
	alignInferJunctionHashInfo->initiateAlignInferJunctionInfo(chromNum);
	cout << "start to insert SJ into SJmap" << endl;
	// insert SJ into SJmap
	alignInferJunctionHashInfo->insertJuncFromJuncFile(inputJuncFile, indexInfo);
	cout << "start to initaite SJhashInfo" << endl;
	SJhash_Info* SJhashInfo = new SJhash_Info();
	SJhashInfo->initiateAreaAndStringHash(chromNum);
	cout << "start to convert 2 SJhashInfo" << endl;
	alignInferJunctionHashInfo->convert2SJhashInfo(SJhashInfo, indexInfo);

	cout << "start to getAlter spliceSites" << endl;

	alignInferJunctionHashInfo->getAlterSpliceSites_compareAnchorSimilarity(SJhashInfo, alterSpliceSiteOffset, indexInfo);
	cout << "start to output SJ map" << endl;
	// output SJmap
	alignInferJunctionHashInfo->outputAlignInferInfoHashInfo_maxAnchorSize_alterSpliceSites_compareAnchorSimilarity(indexInfo, outputSJpath_all);
	cout << "start to output SJ filtering results ..." << endl;
	alignInferJunctionHashInfo->outputSJ_extended(outputSJpath_extended, indexInfo);
	alignInferJunctionHashInfo->outputSJ_nonExtended_nonAlterSpliceSite(outputSJpath_nonExtended_nonAlterSpliceSite, indexInfo);
	alignInferJunctionHashInfo->outputSJ_nonExtended_withAlterSpliceSite(outputSJpath_nonExtended_withAlterSpliceSite, indexInfo);
	//alignInferJunctionHashInfo->outputSJ_nonExtended_withAlterSpliceSite_valid(outputSJpath_nonExtended_withAlterSpliceSite_valid, indexInfo);
	//alignInferJunctionHashInfo->outputSJ_nonExtended_withAlterSpliceSite_invalid(outputSJpath_nonExtended_withAlterSpliceSite_invalid, indexInfo);
	alignInferJunctionHashInfo->outputSJ_kept(outputSJpath_kept, indexInfo);
	alignInferJunctionHashInfo->outputSJ_filterOut(outputSJpath_filterOut, indexInfo);
	alignInferJunctionHashInfo->outputSJ_nonExtended_multiAnchorExactlyTheSame(outputSJpath_nonExtended_multiAnchorExactlyTheSame, indexInfo);
	
	delete alignInferJunctionHashInfo;
	delete indexInfo;
	//sj_ofs.close();
	parameter_ifs.close();
	return 0;
}
