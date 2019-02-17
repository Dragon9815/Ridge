#pragma once

#include "Ridge/Common.h"
#include "Ridge/Math/Matrix.h"

#include <vector>
#include <string>
#include <fstream>
#include <sstream>

namespace Ridge {
	class RIDGE_API ShaderProgram {
	public:
		virtual ~ShaderProgram() {}

		bool LoadFromFile(const std::string& path) {
			std::ifstream is(path);
			if (!is.is_open()) {
				RIDGE_CORE_ERROR("Cannot open shader file: {}", path);
				return false;
			}

			std::stringstream ss;
			ss << is.rdbuf();

			return LoadFromString(ss.str());
		}

		virtual bool LoadFromString(const std::string& sourceCode) = 0;

		virtual void SetUniform(const std::string& name, const Matrix4f& value) = 0;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;
	};
}