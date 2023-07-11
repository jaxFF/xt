#include "wglcontext.cpp"

//
// note: Data structures utilized by the OpenGL backend
//

#define DefineGLFunction(Func) global _##Func* Func

DefineGLFunction(glGetString);
DefineGLFunction(glGetStringi);

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
DefineGLFunction(glBufferSubData);
DefineGLFunction(glIsProgram);

// OpenGL 3.0
DefineGLFunction(glBindVertexArray);
DefineGLFunction(glDeleteVertexArrays);
DefineGLFunction(glGenVertexArrays);

// OpenGL 3.3
DefineGLFunction(glBindSampler);

// OpenGL 4.6
DefineGLFunction(glScissor);

typedef void* APIENTRY _glGetProcAddress(char*);

global HMODULE GLLibrary;
global _glGetProcAddress* GLGetProcAddress;

global gl_version GLVersion = {};
global u32 GL_VERSION_1_0;
global u32 GL_VERSION_1_1;
global u32 GL_VERSION_1_2;
global u32 GL_VERSION_1_3;
global u32 GL_VERSION_1_4;
global u32 GL_VERSION_1_5;
global u32 GL_VERSION_2_0;
global u32 GL_VERSION_2_1;
global u32 GL_VERSION_3_0;
global u32 GL_VERSION_3_1;
global u32 GL_VERSION_3_2;
global u32 GL_VERSION_3_3;
global u32 GL_VERSION_4_0;
global u32 GL_VERSION_4_1;
global u32 GL_VERSION_4_2;
global u32 GL_VERSION_4_3;
global u32 GL_VERSION_4_4;
global u32 GL_VERSION_4_5;
global u32 GL_VERSION_4_6;

global void GLFindAPIVersions() {
    char* Version = (char*)glGetString(GL_VERSION);
    if (!Version) {
        return;
    }

    u32 Major = atoi(Version);
    u32 Minor = atoi(Version + 2);

    GLVersion.Major = Major; 
    GLVersion.Minor = Minor;
	GL_VERSION_1_0 = (Major == 1 && Minor >= 0) || Major > 1;
	GL_VERSION_1_1 = (Major == 1 && Minor >= 1) || Major > 1;
	GL_VERSION_1_2 = (Major == 1 && Minor >= 2) || Major > 1;
	GL_VERSION_1_3 = (Major == 1 && Minor >= 3) || Major > 1;
	GL_VERSION_1_4 = (Major == 1 && Minor >= 4) || Major > 1;
    GL_VERSION_1_5 = (Major == 1 && Minor >= 5) || Major > 1;
	GL_VERSION_2_0 = (Major == 2 && Minor >= 0) || Major > 2;
	GL_VERSION_2_1 = (Major == 2 && Minor >= 1) || Major > 2;
	GL_VERSION_3_0 = (Major == 3 && Minor >= 0) || Major > 3;
	GL_VERSION_3_1 = (Major == 3 && Minor >= 1) || Major > 3;
	GL_VERSION_3_2 = (Major == 3 && Minor >= 2) || Major > 3;
	GL_VERSION_3_3 = (Major == 3 && Minor >= 3) || Major > 3;
	GL_VERSION_4_0 = (Major == 4 && Minor >= 0) || Major > 4;
	GL_VERSION_4_1 = (Major == 4 && Minor >= 1) || Major > 4;
	GL_VERSION_4_2 = (Major == 4 && Minor >= 2) || Major > 4;
	GL_VERSION_4_3 = (Major == 4 && Minor >= 3) || Major > 4;
	GL_VERSION_4_4 = (Major == 4 && Minor >= 4) || Major > 4;
	GL_VERSION_4_5 = (Major == 4 && Minor >= 5) || Major > 4;
	GL_VERSION_4_6 = (Major == 4 && Minor >= 6) || Major > 4;
}

global void* _GLGetFunction(char* Func) {
    void* Result = nullptr;
    if (GLGetProcAddress != nullptr) {
        Result = GLGetProcAddress(Func);
    }

    if (Result == nullptr) {
        Result = (void*)GetProcAddress(GLLibrary, Func);
    }

    return Result;
}

#define GLGetFunction(Func) Func = (_##Func*)_GLGetFunction(#Func)

void GLLoadFunctions() {
	GLGetFunction(glGetString);
    GLGetFunction(glGetStringi);

    GLFindAPIVersions();
    if (GL_VERSION_1_0) {
        GLGetFunction(glClear);
        GLGetFunction(glClearColor);
        GLGetFunction(glEnable);
        GLGetFunction(glDisable);
        GLGetFunction(glViewport);
        GLGetFunction(glGetBooleanv);
        GLGetFunction(glGetDoublev);
        GLGetFunction(glGetFloatv);
        GLGetFunction(glGetIntegerv);
        GLGetFunction(glPolygonMode);
        GLGetFunction(glIsEnabled);
        GLGetFunction(glTexParameterf);
        GLGetFunction(glTexParameteri);
        GLGetFunction(glPixelStoref);
        GLGetFunction(glPixelStorei);
        GLGetFunction(glTexImage2D);
    } if (GL_VERSION_1_1) {
        GLGetFunction(glBindTexture);
        GLGetFunction(glDrawElements);
        GLGetFunction(glDeleteTextures);
        GLGetFunction(glBindTexture);
        GLGetFunction(glGenTextures);
    } if (GL_VERSION_1_2) {

    } if (GL_VERSION_1_3) {
        GLGetFunction(glActiveTexture);
    } if (GL_VERSION_1_4) {
        GLGetFunction(glBlendFunc);
        GLGetFunction(glBlendFuncSeparate);
        GLGetFunction(glBlendFuncSeparatei);
    } if (GL_VERSION_1_5) {
        GLGetFunction(glBindBuffer);
        GLGetFunction(glBufferData);
        GLGetFunction(glGenBuffers);
        GLGetFunction(glDeleteBuffers);
    } if (GL_VERSION_2_0) {
        GLGetFunction(glBlendEquation);
        GLGetFunction(glUseProgram);
        GLGetFunction(glUniform1f);
        GLGetFunction(glUniform2f);
        GLGetFunction(glUniform3f);
        GLGetFunction(glUniform4f);
        GLGetFunction(glUniform1i);
        GLGetFunction(glUniform2i);
        GLGetFunction(glUniform3i);
        GLGetFunction(glUniform4i);
        GLGetFunction(glUniform1ui);
        GLGetFunction(glUniform2ui);
        GLGetFunction(glUniform3ui);
        GLGetFunction(glUniform4ui);
        GLGetFunction(glUniform1fv);
        GLGetFunction(glUniform2fv);
        GLGetFunction(glUniform3fv);
        GLGetFunction(glUniform4fv);
        GLGetFunction(glUniform1iv);
        GLGetFunction(glUniform2iv);
        GLGetFunction(glUniform3iv);
        GLGetFunction(glUniform4iv);
        GLGetFunction(glUniform1uiv);
        GLGetFunction(glUniform2uiv);
        GLGetFunction(glUniform3uiv);
        GLGetFunction(glUniform4uiv);
        GLGetFunction(glUniformMatrix2fv);
        GLGetFunction(glUniformMatrix3fv);
        GLGetFunction(glUniformMatrix4fv);
        GLGetFunction(glUniformMatrix2x3fv);
        GLGetFunction(glUniformMatrix3x2fv);
        GLGetFunction(glUniformMatrix2x4fv);
        GLGetFunction(glUniformMatrix4x2fv);
        GLGetFunction(glUniformMatrix3x4fv);
        GLGetFunction(glUniformMatrix4x3fv);
        GLGetFunction(glBlendEquationSeparate);
        GLGetFunction(glBlendEquationSeparatei);
        GLGetFunction(glEnableVertexAttribArray);
        GLGetFunction(glDisableVertexAttribArray);
        GLGetFunction(glVertexAttribPointer);
        GLGetFunction(glVertexAttribIPointer);
        GLGetFunction(glVertexAttribLPointer);
        GLGetFunction(glUseProgram);
        GLGetFunction(glGetShaderiv);
        GLGetFunction(glGetProgramiv);
        GLGetFunction(glDetachShader);
        GLGetFunction(glCompileShader);
        GLGetFunction(glAttachShader);
        GLGetFunction(glLinkProgram);
        GLGetFunction(glGetUniformLocation);
        GLGetFunction(glGetAttribLocation);
        GLGetFunction(glDeleteShader);
        GLGetFunction(glDeleteProgram);
        GLGetFunction(glGetShaderInfoLog);
        GLGetFunction(glGetProgramInfoLog);
        GLGetFunction(glShaderSource);
        GLGetFunction(glCreateShader);
        GLGetFunction(glCreateProgram);
        GLGetFunction(glBufferSubData);
        GLGetFunction(glIsProgram);
    } if (GL_VERSION_2_1) {
    } if (GL_VERSION_3_0) {
        GLGetFunction(glBindVertexArray);
        GLGetFunction(glDeleteVertexArrays);
        GLGetFunction(glBindVertexArray);
        GLGetFunction(glGenVertexArrays);
    } if (GL_VERSION_3_1) {

    } if (GL_VERSION_3_2) {

    } if (GL_VERSION_3_3) {
        GLGetFunction(glBindSampler);
    } if (GL_VERSION_4_0) {

    } if (GL_VERSION_4_1) {

    } if (GL_VERSION_4_2) {

    } if (GL_VERSION_4_3) {

    } if (GL_VERSION_4_4) {

    } if (GL_VERSION_4_5) {

    } if (GL_VERSION_4_6) {
        GLGetFunction(glScissor);
    }

}

gl_info GLGetInfo() {
    gl_info Result = { };
    Result.Vendor = (char*)glGetString(GL_VENDOR);
    Result.Renderer = (char*)glGetString(GL_RENDERER);
    Result.Extensions = (char*)glGetString(GL_EXTENSIONS);
    Result.Version = (char*)glGetString(GL_VERSION);
    Result.GLSLVersion = (char*)glGetString(GL_SHADING_LANGUAGE_VERSION);

    return Result;
}

bool GLInit(gl_info* Info, void* Handle) {    
    bool Result = GLInitContext(Handle);
    if (!Result) {
        return Result;
    }

    GLLibrary = (HMODULE) LoadLibraryExA("opengl32.dll", nullptr, 0);
    GLGetProcAddress = (_glGetProcAddress*) GetProcAddress(GLLibrary, "wglGetProcAddress");

    GLLoadFunctions();
	FreeLibrary(GLLibrary);

    *Info = GLGetInfo();
	printf("OpenGL %s (GLSL %s)\n", Info->Version, Info->GLSLVersion);
	printf("GPU: %s %s\n", Info->Vendor, Info->Renderer);
    return Result;
}
