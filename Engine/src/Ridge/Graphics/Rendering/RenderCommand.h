#pragma once

#include "Ridge/Common.h"

namespace Ridge { namespace Graphics {

	class Renderer;

	class RIDGE_API RenderCommand 
	{ 
	public:
		virtual void Execute( Renderer * renderer ) = 0;

	protected:

	private:
		
	};

} }