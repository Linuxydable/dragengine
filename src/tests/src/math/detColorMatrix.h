// include only once
#ifndef _DETCOLORMATRIX_H_
#define _DETCOLORMATRIX_H_

// includes
#include "../detCase.h"
#include "dragengine/common/math/decMath.h"



// class detColorMatrix
class detColorMatrix : public detCase{
public:
	detColorMatrix();
	~detColorMatrix();
	void Prepare();
	void Run();
	void CleanUp();
	const char *GetTestName();
	
private:
	void pRandomInitMatrix( decColorMatrix &matrix );
	void pTestInitial();
	void pTestCreationTranslation();
	void pTestCreationScaling();
	void pTestCreationFrom();
	void pTestAddition();
	void pTestMultiply();
	void pTestTransforming();
	void pTestConcatenation();
};

// end of include only once
#endif

