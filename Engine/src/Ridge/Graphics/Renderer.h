#pragma once

#include "Ridge/Common.h"
#include "Ridge/Events/Event.h"
#include "Ridge/Graphics/Shader.h"

namespace Ridge {
	class RIDGE_API Renderer {
	public:
		virtual void OnEvent(Event& e) = 0;

		virtual void PrepareFrame() = 0;

		virtual void SetClearColor(float r, float g, float b, float a) = 0;
		
	public:
		void UseShader( ShaderProgram * program )
		{
			if( program != currentProgram )
			{
				currentProgram = program;
				program->Bind( );
			}
		}
		
	protected:
		ShaderProgram * currentProgram = nullptr;
	};
}