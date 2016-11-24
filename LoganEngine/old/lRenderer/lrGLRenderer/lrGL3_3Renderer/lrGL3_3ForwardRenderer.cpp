
#include "lrGL3_3ForwardRenderer.h"

extern "C" {

	liRenderer *lrCreateGL3ForwardRenderer()
	{
		return new lrGL3_3ForwardRenderer;
	}

}
