#include "supch.h"
#include "platform/opengl/opengl_shader.h"

#include <fstream>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace susumu
{
    static GLenum ShaderTypeFromString(const std::string& type)
    {
        if (type == "vertex") return GL_VERTEX_SHADER;
        if (type == "fragment" || type == "pixel") return GL_FRAGMENT_SHADER;

        SU_CORE_ASSERT(false, "Unknown shader type!");
        return 0;
    }

    OpenGLShader::OpenGLShader(const std::string& filepath)
    {
        SU_PROFILE_FUNCTION();

        // extract shader name from filepath (assets/shaders/texture.glsl -> texture)
        auto lastSlashIndex = filepath.find_last_of("/\\");
        lastSlashIndex = lastSlashIndex == std::string::npos ? 0 : lastSlashIndex + 1;
        auto lastDotIndex = filepath.rfind('.');
        auto length = lastDotIndex == std::string::npos ? filepath.size() - 1 - lastSlashIndex : lastDotIndex - lastSlashIndex;
        m_Name = filepath.substr(lastSlashIndex, length);

        std::string source = ReadFile(filepath);
        auto shaderSources = PreProcess(source);
        Compile(shaderSources);
    }

    OpenGLShader::OpenGLShader(const std::string& name, const std::string& filepath)
        : m_Name(name)
    {
        SU_PROFILE_FUNCTION();

        std::string source = ReadFile(filepath);
        auto shaderSources = PreProcess(source);
        Compile(shaderSources);
    }

    OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource)
        : m_Name(name)
    {
        SU_PROFILE_FUNCTION();

        std::unordered_map<GLenum, std::string> shaderSources;
        shaderSources[GL_VERTEX_SHADER] = vertexSource;
        shaderSources[GL_FRAGMENT_SHADER] = fragmentSource;
        Compile(shaderSources);
    }

    OpenGLShader::~OpenGLShader()
    {
        SU_PROFILE_FUNCTION();

        glDeleteProgram(m_RendererID);
    }


    void OpenGLShader::Bind() const
    {
        SU_PROFILE_FUNCTION();

        glUseProgram(m_RendererID);
    }

    void OpenGLShader::Unbind() const
    {
        SU_PROFILE_FUNCTION();

        glUseProgram(0);
    }

    void OpenGLShader::SetIntArray(const std::string& name, int* values, uint32_t count)
    {
        UploadUniformIntArray(name, values, count);
    }

    void OpenGLShader::SetInt(const std::string& name, int value)
    {
        UploadUniformInt(name, value);
    }

    void OpenGLShader::SetFloat(const std::string& name, float value)
    {
        UploadUniformFloat(name, value);
    }

    void OpenGLShader::SetFloat2(const std::string& name, const glm::vec2& value)
    {
        UploadUniformFloat2(name, value);
    }

    void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value)
    {
        UploadUniformFloat3(name, value);
    }

    void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value)
    {
        UploadUniformFloat4(name, value);
    }

    void OpenGLShader::SetMat3(const std::string& name, const glm::mat3& value)
    {
        UploadUniformMat3(name, value);
    }

    void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value)
    {
        UploadUniformMat4(name, value);
    }


    void OpenGLShader::UploadUniformIntArray(const std::string& name, int* values, uint32_t count)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform1iv(location, count, values);
    }

    void OpenGLShader::UploadUniformInt(const std::string& name, int value)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform1i(location, value);
    }

    void OpenGLShader::UploadUniformFloat(const std::string& name, float value)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform1f(location, value);
    }

    void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& value)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform2f(location, value.x, value.y);
    }

    void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& value)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform3f(location, value.x, value.y, value.z);
    }

    void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& value)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform4f(location, value.x, value.y, value.z, value.w);
    }

    void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& value)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }

    void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& value)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }


    std::string OpenGLShader::ReadFile(const std::string& filepath)
    {
        SU_PROFILE_FUNCTION();

        std::string result;
        std::ifstream in(filepath, std::ios::in | std::ios::binary);
        if (in)
        {
            in.seekg(0, std::ios::end);
            size_t size = in.tellg();
            if (size != -1)
            {
                result.resize(size);
                in.seekg(0, std::ios::beg);
                in.read(&result[0], result.size());
                in.close();
            }
            else
            {
                SU_CORE_ERROR("Could not read from file '{0}'!", filepath);
            }
        }
        else
        {
            SU_CORE_ERROR("Could not open file '{0}'!", filepath);
        }
        return result;
    }

    std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
    {
        SU_PROFILE_FUNCTION();

        std::unordered_map<GLenum, std::string> shaderSources;

        const char* typeToken = "#type";
        size_t typeTokenLength = strlen(typeToken);
        size_t pos = source.find(typeToken, 0); //start of shader type declaration line
        while (pos != std::string::npos)
        {
            size_t eol = source.find_first_of("\r\n", pos); //end of shader type declaration line
            SU_CORE_ASSERT(eol != std::string::npos, "Syntax error");
            size_t begin = pos + typeTokenLength + 1; //start of shader type name (after "#type " keyword)
            std::string type = source.substr(begin, eol - begin);
            SU_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified!");

            size_t nextLinePos = source.find_first_of("\r\n", eol); // start of shader code after shader type declaration line
            SU_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error");
            pos = source.find(typeToken, nextLinePos); //start of next shader type declaration line
            shaderSources[ShaderTypeFromString(type)] = (pos == std::string::npos)
                ? source.substr(nextLinePos)
                : source.substr(nextLinePos, pos - nextLinePos);
        }
        return shaderSources;
    }

    void OpenGLShader::Compile(std::unordered_map<GLenum, std::string>& shaderSources)
    {
        SU_PROFILE_FUNCTION();

        GLuint program = glCreateProgram();
        const int maxShadersInFile = 4;
        SU_CORE_ASSERT(shaderSources.size() <= maxShadersInFile, "Cannot have more than {0} shaders in file!", maxShadersInFile);
        std::array<GLint, maxShadersInFile> glShaderIDs;
        int glShaderIDIndex = 0;

        for (auto& kv : shaderSources)
        {
            GLenum type = kv.first;
            const std::string& source = kv.second;

            GLuint shader = glCreateShader(type);
            const GLchar* sourceCStr = source.c_str();
            glShaderSource(shader, 1, &sourceCStr, 0);
            glCompileShader(shader);

            GLint isCompiled = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
            if (isCompiled == GL_FALSE)
            {
                GLint maxLength = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

                std::vector<GLchar> infoLog(maxLength);
                glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

                glDeleteShader(shader);

                SU_CORE_ERROR("{0}", infoLog.data());
                SU_CORE_ASSERT(false, "Shader compilation failed!");
                break;
            }
            glAttachShader(program, shader);
            glShaderIDs[glShaderIDIndex++] = shader;
        }

        glLinkProgram(program);
        GLint isLinked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

            glDeleteProgram(program);
            for (auto id : glShaderIDs)
            {
                glDeleteShader(id);
            }
            SU_CORE_ERROR("{0}", infoLog.data());
            SU_CORE_ASSERT(false, "Shader linking failed!");
            return;
        }

        for (auto id : glShaderIDs)
        {
            glDetachShader(program, id);
            glDeleteShader(id);
        }

        m_RendererID = program;
    }
}
