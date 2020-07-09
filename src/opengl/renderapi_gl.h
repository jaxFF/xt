#ifndef RENDERAPI_GL_H
#define RENDERAPI_GL_H

//
// note: Data structures utilized by the OpenGL backend
//

#define DefineGLFunction(Func) global _##Func* Func

DefineGLFunction(glGetString);

// OpenGL 1.0
DefineGLFunction(glClear);
DefineGLFunction(glClearColor);
DefineGLFunction(glEnable);
DefineGLFunction(glDisable);
DefineGLFunction(glViewport);
DefineGLFunction(glGetBooleanv);
DefineGLFunction(glGetDoublev);
DefineGLFunction(glGetFloatv);
DefineGLFunction(glGetIntegerv);
DefineGLFunction(glPolygonMode);
DefineGLFunction(glIsEnabled);
DefineGLFunction(glTexParameterf);
DefineGLFunction(glTexParameteri);
DefineGLFunction(glPixelStoref);
DefineGLFunction(glPixelStorei);
DefineGLFunction(glTexImage2D);

// OpenGL 1.1
DefineGLFunction(glBindTexture);
DefineGLFunction(glDrawElements);
DefineGLFunction(glDeleteTextures);
DefineGLFunction(glGenTextures);

// OpenGL 1.3
DefineGLFunction(glActiveTexture);

// OpenGL 1.4
DefineGLFunction(glBlendFunc);
DefineGLFunction(glBlendFuncSeparate);
DefineGLFunction(glBlendFuncSeparatei);

// OpenGL 1.5
DefineGLFunction(glBindBuffer);
DefineGLFunction(glBufferData);
DefineGLFunction(glGenBuffers);
DefineGLFunction(glDeleteBuffers);

// OpenGL 2.0
DefineGLFunction(glBlendEquation);
DefineGLFunction(glUseProgram);
DefineGLFunction(glUniform1f);
DefineGLFunction(glUniform2f);
DefineGLFunction(glUniform3f);
DefineGLFunction(glUniform4f);
DefineGLFunction(glUniform1i);
DefineGLFunction(glUniform2i);
DefineGLFunction(glUniform3i);
DefineGLFunction(glUniform4i);
DefineGLFunction(glUniform1ui);
DefineGLFunction(glUniform2ui);
DefineGLFunction(glUniform3ui);
DefineGLFunction(glUniform4ui);
DefineGLFunction(glUniform1fv);
DefineGLFunction(glUniform2fv);
DefineGLFunction(glUniform3fv);
DefineGLFunction(glUniform4fv);
DefineGLFunction(glUniform1iv);
DefineGLFunction(glUniform2iv);
DefineGLFunction(glUniform3iv);
DefineGLFunction(glUniform4iv);
DefineGLFunction(glUniform1uiv);
DefineGLFunction(glUniform2uiv);
DefineGLFunction(glUniform3uiv);
DefineGLFunction(glUniform4uiv);
DefineGLFunction(glUniformMatrix2fv);
DefineGLFunction(glUniformMatrix3fv);
DefineGLFunction(glUniformMatrix4fv);
DefineGLFunction(glUniformMatrix2x3fv);
DefineGLFunction(glUniformMatrix3x2fv);
DefineGLFunction(glUniformMatrix2x4fv);
DefineGLFunction(glUniformMatrix4x2fv);
DefineGLFunction(glUniformMatrix3x4fv);
DefineGLFunction(glUniformMatrix4x3fv);
DefineGLFunction(glBlendEquationSeparate);
DefineGLFunction(glBlendEquationSeparatei);
DefineGLFunction(glEnableVertexAttribArray);
DefineGLFunction(glDisableVertexAttribArray);
DefineGLFunction(glVertexAttribPointer);
DefineGLFunction(glVertexAttribIPointer);
DefineGLFunction(glVertexAttribLPointer);
DefineGLFunction(glGetShaderiv);
DefineGLFunction(glGetProgramiv);
DefineGLFunction(glDetachShader);
DefineGLFunction(glCompileShader);
DefineGLFunction(glAttachShader);
DefineGLFunction(glLinkProgram);
DefineGLFunction(glGetUniformLocation);
DefineGLFunction(glGetAttribLocation);
DefineGLFunction(glDeleteShader);
DefineGLFunction(glDeleteProgram);
DefineGLFunction(glGetShaderInfoLog);
DefineGLFunction(glGetProgramInfoLog);
DefineGLFunction(glShaderSource);
DefineGLFunction(glCreateShader);
DefineGLFunction(glCreateProgram);

// OpenGL 3.0
DefineGLFunction(glBindVertexArray);
DefineGLFunction(glDeleteVertexArrays);
DefineGLFunction(glGenVertexArrays);

// OpenGL 3.3
DefineGLFunction(glBindSampler);

// OpenGL 4.6
DefineGLFunction(glScissor);

struct gl_functions {

};

struct gl_info {
    char* Vendor;
    char* Renderer;
    char* Extensions;

    char* Version;
    char* GLSLVersion;
};

bool GLInit(gl_info* Info, void* Handle);
gl_info GLGetInfo();

#// define GLCall(Function) Renderer->Functions.Function
// GLCall(glClear(GL_COLOR_BUFFER_BIT));
// GLCall(glClearColor(Clear->Color->R, Clear->Color->G, Clear->Color->B, Clear->Color->A));

#endif