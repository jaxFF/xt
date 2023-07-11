#ifndef OPENGL_IMPORTS_H
#define OPENGL_IMPORTS_H

typedef const GLubyte* APIENTRY _glGetString(GLenum Name);

// OpenGL 1.0
typedef void APIENTRY _glClear(GLbitfield mask);
typedef void APIENTRY _glClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
typedef void APIENTRY _glEnable(GLenum cap);
typedef void APIENTRY _glDisable(GLenum cap);
typedef void APIENTRY _glViewport(GLint x, GLint y, GLsizei width, GLsizei height);
typedef void APIENTRY _glGetBooleanv(GLenum pname, GLboolean * params);
typedef void APIENTRY _glGetDoublev(GLenum pname, GLdouble * params);
typedef void APIENTRY _glGetFloatv(GLenum pname, GLfloat * params);
typedef void APIENTRY _glGetIntegerv(GLenum pname, GLint* params);
typedef void APIENTRY _glPolygonMode(GLenum face, GLenum mode);
typedef GLboolean APIENTRY _glIsEnabled(GLenum cap);
typedef void APIENTRY _glTexParameterf(GLenum target, GLenum pname, GLfloat param);
typedef void APIENTRY _glTexParameteri(GLenum target, GLenum pname, GLint param);
typedef void APIENTRY _glPixelStoref(GLenum pname, GLint param);
typedef void APIENTRY _glPixelStorei(GLenum pname, GLint param);
typedef void APIENTRY _glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void * data);

// OpenGL 1.1
typedef void APIENTRY _glBindTexture(GLenum target, GLuint texture);
typedef void APIENTRY _glDrawElements(GLenum mode, GLsizei count, GLenum type, const void * indices);
typedef void APIENTRY _glDeleteTextures(GLsizei n, const GLuint * textures);
typedef void APIENTRY _glBindTexture(GLenum target, GLuint texture);
typedef void APIENTRY _glGenTextures(GLsizei n, GLuint * textures);

// OpenGL 1.3
typedef void APIENTRY _glActiveTexture(GLenum texture);

// OpenGL 1.4
typedef void APIENTRY _glBlendFunc(GLenum sfactor, GLenum dfactor);
typedef void APIENTRY _glBlendFuncSeparate(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
typedef void APIENTRY _glBlendFuncSeparatei(GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);

// OpenGL 1.5
typedef void APIENTRY _glBindBuffer(GLenum target, GLuint buffer);
typedef void APIENTRY _glBufferData(GLenum target, GLsizeiptr size, const void * data, GLenum usage);
typedef void APIENTRY _glGenBuffers(GLsizei n, GLuint * buffers);
typedef void APIENTRY _glDeleteBuffers(GLsizei n, const GLuint * buffers);

// OpenGL 2.0
typedef void APIENTRY _glBlendEquation(GLenum mode);
typedef void APIENTRY _glUseProgram(GLuint program);
typedef void APIENTRY _glUniform1f(GLint location, GLfloat v0);
typedef void APIENTRY _glUniform2f(GLint location, GLfloat v0, GLfloat v1);
typedef void APIENTRY _glUniform3f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
typedef void APIENTRY _glUniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
typedef void APIENTRY _glUniform1i(GLint location, GLint v0);
typedef void APIENTRY _glUniform2i(GLint location, GLint v0, GLint v1);
typedef void APIENTRY _glUniform3i(GLint location, GLint v0, GLint v1, GLint v2);
typedef void APIENTRY _glUniform4i(GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
typedef void APIENTRY _glUniform1ui(GLint location, GLuint v0);
typedef void APIENTRY _glUniform2ui(GLint location, GLuint v0, GLuint v1);
typedef void APIENTRY _glUniform3ui(GLint location, GLuint v0, GLuint v1, GLuint v2);
typedef void APIENTRY _glUniform4ui(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
typedef void APIENTRY _glUniform1fv(GLint location, GLsizei count, const GLfloat *value);
typedef void APIENTRY _glUniform2fv(GLint location, GLsizei count, const GLfloat *value);
typedef void APIENTRY _glUniform3fv(GLint location, GLsizei count, const GLfloat *value);
typedef void APIENTRY _glUniform4fv(GLint location, GLsizei count, const GLfloat *value);
typedef void APIENTRY _glUniform1iv(GLint location, GLsizei count, const GLint *value);
typedef void APIENTRY _glUniform2iv(GLint location, GLsizei count, const GLint *value);
typedef void APIENTRY _glUniform3iv(GLint location, GLsizei count, const GLint *value);
typedef void APIENTRY _glUniform4iv(GLint location, GLsizei count, const GLint *value);
typedef void APIENTRY _glUniform1uiv(GLint location, GLsizei count, const GLuint *value);
typedef void APIENTRY _glUniform2uiv(GLint location, GLsizei count, const GLuint *value);
typedef void APIENTRY _glUniform3uiv(GLint location, GLsizei count, const GLuint *value);
typedef void APIENTRY _glUniform4uiv(GLint location, GLsizei count, const GLuint *value);
typedef void APIENTRY _glUniformMatrix2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void APIENTRY _glUniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void APIENTRY _glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void APIENTRY _glUniformMatrix2x3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void APIENTRY _glUniformMatrix3x2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void APIENTRY _glUniformMatrix2x4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void APIENTRY _glUniformMatrix4x2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void APIENTRY _glUniformMatrix3x4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void APIENTRY _glUniformMatrix4x3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void APIENTRY _glBlendEquationSeparate(GLenum modeRGB, GLenum modeAlpha);
typedef void APIENTRY _glBlendEquationSeparatei(GLuint buf, GLenum modeRGB, GLenum modeAlpha);
typedef void APIENTRY _glEnableVertexAttribArray(GLuint index);
typedef void APIENTRY _glDisableVertexAttribArray(GLuint index);
typedef void APIENTRY _glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void * pointer);
typedef void APIENTRY _glVertexAttribIPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const void * pointer);
typedef void APIENTRY _glVertexAttribLPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const void * pointer);
typedef void APIENTRY _glUseProgram(GLuint program);
typedef void APIENTRY _glGetShaderiv(GLuint shader, GLenum pname, GLint *params);
typedef void APIENTRY _glGetProgramiv(GLuint program, GLenum pname, GLint *params);
typedef void APIENTRY _glDetachShader(GLuint program, GLuint shader);
typedef void APIENTRY _glCompileShader(GLuint shader);
typedef void APIENTRY _glAttachShader(GLuint program, GLuint shader);
typedef void APIENTRY _glLinkProgram(GLuint program);
typedef GLint APIENTRY _glGetUniformLocation(GLuint program, const GLchar *name);
typedef GLint APIENTRY _glGetAttribLocation(GLuint program, const GLchar *name);
typedef void APIENTRY _glDeleteShader(GLuint shader);
typedef void APIENTRY _glDeleteProgram(GLuint program);
typedef void APIENTRY _glGetShaderInfoLog(GLuint shader, GLsizei maxLength, GLsizei *length, GLchar *infoLog);
typedef void APIENTRY _glGetProgramInfoLog(GLuint program, GLsizei maxLength, GLsizei *length, GLchar *infoLog);
typedef void APIENTRY _glShaderSource(GLuint shader, GLsizei count, const GLchar **string, const GLint *length);
typedef GLuint APIENTRY _glCreateShader(GLenum shaderType);
typedef GLuint APIENTRY _glCreateProgram();

// OpenGL 3.0
typedef void APIENTRY _glBindVertexArray(GLuint array);
typedef void APIENTRY _glDeleteVertexArrays(GLsizei n, const GLuint *arrays);
typedef void APIENTRY _glBindVertexArray(GLuint array);
typedef void APIENTRY _glGenVertexArrays(GLsizei n, GLuint *arrays);

// OpenGL 3.3
typedef void APIENTRY _glBindSampler(GLuint unit, GLuint sampler);

// OpenGL 4.6
typedef void APIENTRY _glScissor(GLint x, GLint y, GLsizei width, GLsizei height);



typedef void APIENTRY _glNamedBufferData(GLuint buffer, GLsizeiptr size, const void *data, GLenum usage);
typedef void APIENTRY _glEnableVertexArrayAttrib(GLuint vaobj, GLuint index);
typedef void APIENTRY _glDisableVertexArrayAttrib(GLuint vaobj, GLuint index);



#endif