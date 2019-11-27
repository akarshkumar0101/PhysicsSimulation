//
// Created by Akarsh Kumar on 11/22/19.
//

#pragma once

#include <iostream>
#include <fstream>

#include <string>
#include <sstream>

#include <glad/glad.h>


struct ShaderProgramSouce {
    std::string vertexShader;
    std::string fragmentShader;
};


class ShaderProgram {
private:
    const unsigned int mVertexShader;
    const unsigned int mFragmentShader;
    const unsigned int mShaderProgram;

    const std::string mShaderSourceFilePath;

    ShaderProgramSouce mSps;

    void loadShadersSource() {
        std::ifstream fileStream(mShaderSourceFilePath);

        std::stringstream sources[2];

        enum ShaderType {
            NONE = -1, VERTEX = 0, FRAGMENT = 1
        };
        std::stringstream &vs = sources[VERTEX], &fs = sources[FRAGMENT];

        ShaderType currentType = NONE;
        std::string line;
        while (getline(fileStream, line)) {
            if (line.find("#shader") != std::string::npos) {
                if (line.find("vertex") != std::string::npos) {
                    currentType = VERTEX;
                } else if (line.find("fragment") != std::string::npos) {
                    currentType = FRAGMENT;
                }
            } else {
                sources[currentType] << line << std::endl;
            }
        }
        mSps = {vs.str(), fs.str()};
    }

public:
    ShaderProgram(const std::string &shaderSourceFilePath) : mVertexShader(glCreateShader(GL_VERTEX_SHADER)),
                                                             mFragmentShader(glCreateShader(GL_FRAGMENT_SHADER)),
                                                             mShaderProgram(glCreateProgram()),
                                                             mShaderSourceFilePath(shaderSourceFilePath) {
        this->loadShadersSource();

        const char *vsSource = mSps.vertexShader.c_str();
        const char *fsSource = mSps.fragmentShader.c_str();

        glShaderSource(mVertexShader, 1, &vsSource, NULL);
        glShaderSource(mFragmentShader, 1, &fsSource, NULL);

        glCompileShader(mVertexShader);
        glCompileShader(mFragmentShader);

        glAttachShader(mShaderProgram, mVertexShader);
        glAttachShader(mShaderProgram, mFragmentShader);
        glLinkProgram(mShaderProgram);

        checkCompilationLinkingIssues();

        glDeleteShader(mVertexShader);
        glDeleteShader(mFragmentShader);
    }

    void checkCompilationLinkingIssues() const{
        using namespace std;

        int successVS, successFS, successProgram;
        char infoLog[512];
        glGetShaderiv(mVertexShader, GL_COMPILE_STATUS, &successVS);
        glGetShaderiv(mFragmentShader, GL_COMPILE_STATUS, &successFS);
        glGetProgramiv(mShaderProgram, GL_LINK_STATUS, &successProgram);
        if (!successVS) {
            glGetShaderInfoLog(mVertexShader, 512, NULL, infoLog);
            cerr << "Vertex shader compilation error: " << endl;
            cerr << infoLog << endl;
        }
        if (!successFS) {
            glGetShaderInfoLog(mFragmentShader, 512, NULL, infoLog);
            cerr << "Fragment shader compilation error: " << endl;
            cerr << infoLog << endl;
        }
        if (!successProgram) {
            glGetProgramInfoLog(mShaderProgram, 512, NULL, infoLog);
            cerr << "Shader Program linking error: " << endl;
            cerr << infoLog << endl;
        }
    }

    void glUseThisProgram() const {
        glUseProgram(mShaderProgram);
    }

    unsigned int shaderProgram() const{
        return mShaderProgram;
    }

    void setUniformFloat(const std::string& name, const float val){
        glUniform1f(getUniformLocation((name)), val);
    }
    void setUniformMatrix4fv(const std::string& name, const glm::mat4& mat){
        glUniformMatrix4fv(getUniformLocation(name),1, GL_FALSE, glm::value_ptr(mat));
    }
    int getUniformLocation(const std::string& name){
        int location = glGetUniformLocation(mShaderProgram, name.c_str());
        if(location==-1){
            throw std::string("Invalid name for uniform");
        }
        return location;
    }

};