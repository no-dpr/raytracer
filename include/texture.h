#ifndef TEXTURE_H
#define TEXTURE_H

#include "common.h"
#include "perlin.h"
#include "rtw_stb_image.h"

class texture {
    public:
        virtual ~texture() = default;

        virtual color value(double u, double v, const point3& p) const = 0;
};

class solid_color : public texture {
    public:
        solid_color(const color& albedo) : albedo(albedo) {}

        solid_color(double red, double green, double blue) : solid_color(color(red, green, blue)) {}

        color value(double u, double v, const point3& p) const override {
            return albedo;
        }

    private:
        color albedo;
};

class checker_texture : public texture {
    public:
        checker_texture(shared_ptr<texture> even, shared_ptr<texture> odd, double scale) 
            : even(even), odd(odd), inv_scale(1.0 / scale) {}
            
        checker_texture(color even, color odd, double scale) 
            : even(make_shared<solid_color>(even)), odd(make_shared<solid_color>(odd)), inv_scale(1.0 / scale) {}

        color value(double u, double v, const point3& p) const override {
            auto x_int = int(std::floor(p.x() * inv_scale));
            auto y_int = int(std::floor(p.y() * inv_scale));
            auto z_int = int(std::floor(p.z() * inv_scale));

            bool is_even = (x_int + y_int + z_int) % 2 == 0;
            return is_even ? even->value(u, v, p) : odd->value(u, v, p);
        }

    private:
        double inv_scale;
        shared_ptr<texture> even;
        shared_ptr<texture> odd;
};

class image_texture : public texture {
    public:
        image_texture(const char* filename) : image(filename) {}

        color value(double u, double v, const point3& p) const override {
            if (image.height() <= 0) return color(0, 1, 1);

            u = interval(0,1).clamp(u);
            v = 1.0 - interval(0,1).clamp(v);

            auto i = int(u * image.width());
            auto j = int(v * image.height());
            auto pixel = image.pixel_data(i, j);

            auto color_scale = 1.0 / 255.0;
            return color(pixel[0], pixel[1], pixel[2]) * color_scale;
        }
    private:
        rtw_image image;
};

class noise_texture : public texture {
    public:
        noise_texture(double scale) 
            : scale(scale), turb_value(10), turb_depth(7)  {}

        noise_texture(double scale, double turb_value, double turb_depth) 
            : scale(scale), turb_value(turb_value), turb_depth(turb_depth)  {}

        color value(double u, double v, const point3& p) const override {
            return color(1, 1, 1) * 0.5 * (1.0 + noise.noise(scale * p)) 
                * (1 + std::sin(scale * p.z() + turb_value * noise.turb(p, turb_depth)));
        }

    private:
        perlin noise;
        double scale;
        double turb_value;
        int turb_depth;
};

#endif