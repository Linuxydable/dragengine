// include only once
#ifndef _DETTEXMATRIX2_H_
#define _DETTEXMATRIX2_H_

// includes
#include "../detCase.h"

#include <dragengine/common/math/decMath.h>



// class detTexMatrix2
class detTexMatrix2 : public detCase{
public:
	detTexMatrix2();
	~detTexMatrix2();
	void Prepare();
	void Run();
	void CleanUp();
	const char *GetTestName();
	
private:
	void pInitMatrix( decTexMatrix2 &matrix, float base );
	void pTestInitial();
	void pTestCreation();
	void pTestSet();
	void pTestOperators();
	void pTestInvert();
};

// end of include only once
#endif

