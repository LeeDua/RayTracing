#ifndef IMAGE_H
#define IMAGE_H


#include<fstream>
#include<vector>
#include"Vec3.h"
#include"stb_image.h"
#include"stb_image_write.h"

namespace RayTracing {
    
        template <typename T=RGB_t>
        class Image {
        public:
            explicit Image(int w, int h){
                _width = w;
                _height = h;
                data = new T[w*h];
            }
            ~Image() { delete[] data; }
            int height(){return _height;}
            int width(){return _width;}
            T& at(int i, int j){
                return data[get_index(i,j)];
            }
            T at(int i, int j) const{
                return data(get_index(i,j));
            }
            void setColor(int i, int j, T value){
                at(i,j) = value;
            }
            void setColor(int i, int j, MatColor value){
                setColor(i, j, value[0], value[1], value[2]);
            }
            void setColor(int i, int j, dtype r, dtype g, dtype b){
                at(i,j) = T(img_dtype(r*255.99), img_dtype(g*255.99), img_dtype(g*255.99));
            }
            void setColor(int i, int j, img_dtype r, img_dtype g, img_dtype b){
                at(i,j) = T(r, g, b);
            }
            void setOutputFile(std::string img_path){
                fs.open(img_path, std::fstream::out);
                if(fs.fail()){
                    std::cerr << "illegal output file path: " << img_path << std::endl;
                    abort();
                }
            }
            void dumpPPM(std::string img_path="out.ppm"){
                if(!fs.is_open()){
                    setOutputFile(img_path);
                }
                fs  << "P3" << std::endl 
                    << _width << ' ' << _height << std::endl
                    << 255 << std::endl;
                for(int j=0; j<_height; j++){
                    for(int i=0; i<_width; i++){
                        fs  << int(at(i,j)[0]) << ' '
                            << int(at(i,j)[1]) << ' '
                            << int(at(i,j)[2]) << std::endl;
                    }                
                }
                fs.close();
            }
            void dumpJPEG(std::string img_path="out.jpeg"){
                stbi_write_jpg(img_path.c_str(), _width, _height, 3, (void*)data, 100);                
            }


        private:
            //data should be in range [0, 1]
            T* data;
            int _width, _height;
            std::fstream fs;

            inline int get_index(int i,int j){return _width*j + i;}

        };
    
}  // namespace RayTracing

#endif