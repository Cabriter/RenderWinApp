#pragma once

#include <assert.h>
#include "egl\egl.h"
#include "gles2\gl2.h"

class ShaderId
{
public:
	int _shaderId;
	ShaderId() {
		_shaderId = -1;
	}
};


class ProgramId
{
public:
	int _programId;
	ShaderId _vertex;
	ShaderId _fragment;
	ProgramId() {
		_programId = -1;
	};

	bool createProgram(const char* vertex, const char* fragment) {
		bool error = false;
		do {
			if (vertex) {
				_vertex._shaderId = glCreateShader(GL_VERTEX_SHADER);
				glShaderSource(_vertex._shaderId, 1, &vertex, 0);
				glCompileShader(_vertex._shaderId);

				GLint complieStatus;
				glGetShaderiv(_vertex._shaderId, GL_COMPILE_STATUS, &complieStatus);
				error = complieStatus == GL_FALSE;
				if (error) {
					GLchar messages[256];
					glGetShaderInfoLog(_vertex._shaderId, sizeof(messages), 0, messages);
					assert(messages && 0 != 0);
					break;
				}
			}
			if (fragment) {
				_fragment._shaderId = glCreateShader(GL_FRAGMENT_SHADER);
				glShaderSource(_fragment._shaderId, 1, &vertex, 0);
				glCompileShader(_fragment._shaderId);

				GLint complieStatus;
				glGetShaderiv(_fragment._shaderId, GL_COMPILE_STATUS, &complieStatus);
				error = complieStatus == GL_FALSE;
				if (error) {
					GLchar messages[256];
					glGetShaderInfoLog(_fragment._shaderId, sizeof(messages), 0, messages);
					assert(messages && 0 != 0);
					break;
				}
			}

			_programId = glCreateProgram();

			if (_vertex._shaderId) {
				glAttachShader(_programId, _vertex._shaderId);
			}
			if (_fragment._shaderId) {
				glAttachShader(_programId, _fragment._shaderId);
			}

			glLinkProgram(_programId);
			GLint linkStatus;
			glGetProgramiv(_programId, GL_LINK_STATUS, &linkStatus);
			if (linkStatus == GL_FALSE) {
				GLchar messages[256];
				glGetProgramInfoLog(_programId, sizeof(messages), 0, messages);
				break;
			}
			glUseProgram(_programId);

		} while (false);

		if (error) {
			if (_fragment._shaderId) {
				glDeleteShader(_fragment._shaderId);
				_fragment._shaderId = 0;
			}
			if (_vertex._shaderId) {
				glDeleteShader(_vertex._shaderId);
				_vertex._shaderId = 0;
			}
			if (_programId) {
				glDeleteProgram(_programId);
				_programId = 0;
			}
		}
		return true;
	};

	virtual void begin() {
		glUseProgram(_programId);
	}

	virtual void end() {
		glUseProgram(0);
	}
};

class PROGRAM_P2_C4 :ProgramId
{
public:
	typedef int attribute;
	typedef int uniform;

	attribute _position;
	uniform _color;
	uniform _MVP;

	virtual bool initialize() {
		const char* vs = {

		};

		const char * ps = {

		};

		bool res = createProgram(vs, ps);
		if (res) {
			_position = glGetAttribLocation(_programId, "_position");
			_color = glGetAttribLocation(_programId, "_color");
			_MVP = glGetAttribLocation(_programId, "_MVP");
		}
		return res;
	}

	virtual void begin() {
		glUseProgram(_programId);
		glEnableVertexAttribArray(_position);
	}

	virtual void end() {
		glDisableVertexAttribArray(_programId);
		glUseProgram(0);
	}

	PROGRAM_P2_C4() {
		_position = -1;
		_color = -1;
		_MVP = -1;
	}
	~PROGRAM_P2_C4() {}
};