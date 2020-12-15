#ifndef TEXTURE_H
#define TEXTURE_H

#include "Common.h"
#include "Image.h"


namespace RayTracing{
    class ITexture{
        public:
        virtual MatColor at(dtype u, dtype v) const = 0;
    };

    class SolidColorTexture: public ITexture{
        public:
        SolidColorTexture(MatColor color):color(color){}
        MatColor at(dtype u, dtype v) const override{
            return color;
        }        
        private:
        MatColor color;
    };

    class ImgTexture: public ITexture, protected virtual Image<RGB_t>{
        public:
        ImgTexture(std::string img_path):Image(img_path){};
        MatColor at(dtype u, dtype v) const override{
            return toDouble(Image::at(u*(dtype)Image::width(),(1.0-v)*(dtype)Image::height()));
        }
    };

    typedef std::shared_ptr<ITexture> tex_ptr;
    typedef std::shared_ptr<ImgTexture> img_tex_ptr;

}

#endif