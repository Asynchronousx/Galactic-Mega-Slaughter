#include "ResourceManager.h"

#include <iostream>

/**
 * M_PI is not defined on strictly standard-compliant platforms
 */
#ifndef M_PI
#   define M_PI 3.14159265358979323846
#endif

 /**
  * Default OpenGL libs from XP to Seven are 1.1
  * Easier to define this than expect the user to update his
  * OpenGL headers.
  */
#ifndef GL_CLAMP_TO_EDGE
#   define GL_CLAMP_TO_EDGE 0x812F
#endif

namespace objloader
{
    ResourceManager::ResourceManager()
    {
    }

    ResourceManager::~ResourceManager()
    {
        //TODO: should release remaining resources here!
    }

    bool ResourceManager::load_texture(const std::string& filename, bool clamp)
    {
        // return false;
         // ---State preservation---
        GLuint previous_texture;
        glGetIntegerv(GL_TEXTURE_BINDING_2D, (GLint*)&previous_texture);

        // Generate a new texture id
        GLuint texture_id;
        glGenTextures(1, &texture_id);

        // Load the texture using GLFW
        glBindTexture(GL_TEXTURE_2D, texture_id);
        //int result = GL_FALSE;// glfwLoadTexture2D(filename.c_str(), GLFW_BUILD_MIPMAPS_BIT);
        //int width, height;
        int result = SOIL_load_OGL_texture(
            filename.c_str(),
            SOIL_LOAD_AUTO,
            texture_id,
            SOIL_FLAG_MIPMAPS
        );
        if (result == GL_FALSE)
        {
            // An error occured during loading
            std::cerr << "Warning: cannot load texture \"" << filename << "\". GLFW reported an error (GL_FALSE) with glfwLoadTexture2D." << std::endl;

            // ---State preservation---
            glBindTexture(GL_TEXTURE_2D, previous_texture);

            return false;
        }

        // Apply some standard parameters to the texture (linear filtering)
        //TODO: should let the user choose his own texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        if (clamp)
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }
        else // repeat
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        }

        // Add the texture to the list of loaded one
        textures_.insert(std::make_pair(filename, texture_id));

        // ---State preservation---
        glBindTexture(GL_TEXTURE_2D, previous_texture);
    }

    bool ResourceManager::release_texture(const std::string& filename)
    {
        // Retrieve texture id
        GLuint texture_id = get_texture(filename);

        if (texture_id != 0)
        {
            glDeleteTextures(1, &texture_id); // Texture found
            textures_.erase(filename);
            return true;
        }
        else
        {
            std::cerr << "Warning: cannot release non loaded texture \"" << filename << "\"" << std::endl;
            return false;
        }
    }

    GLuint ResourceManager::get_texture(const std::string& filename)
    {
        TextureList::const_iterator it = textures_.find(filename);

        if (it == textures_.end())
        {
            std::cerr << "Warning: cannot retrieve non loaded texture \"" << filename << "\"" << std::endl;
            return 0;   // Texture not found
        }
        else
            return it->second; // Texture found
    }
}