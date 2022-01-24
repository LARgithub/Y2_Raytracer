#ifndef TEXTURE_H
#define TEXTURE_H

#include "perlin.h"
#include "utility.h"

class Texture {
    public:
        virtual Colour value(double u, double v, const Point& p) const = 0;
};

class SolidColour : public Texture{
    public:
        SolidColour(){}
        SolidColour(Colour c) : colour(c) {}
        SolidColour(double r, double g, double b) : SolidColour(Colour(r,g,b)){}

        virtual Colour value(double u, double v, const Point& p) const override{
            return colour;
        }
    private:
        Colour colour;
};

class Checkerboard : public Texture {

    public:
        Checkerboard() {}
        Checkerboard(Colour even, Colour odd, double mult) : e(even), o(odd), m(mult) {}

        virtual Colour value(double u, double v, const Point& p) const override {
            auto sines = sin(m*p.x()) * sin(m*p.y()) * sin(m*p.z());
            if(sines > 0)
                return e;
            else
                return o;
        }

    private:
        Colour e,o;
        double m;
};


class PerlinNoise : public Texture {
    public:
        PerlinNoise() {}
        PerlinNoise(double s) : scale(s) {}

        virtual Colour value(double u, double v, const Point& p) const override{
            return Colour(1,1,1) * (1 + sin(scale*p.z() + 50*noise.turbulence(p)));
        }

    public:
        Perlin noise;
        double scale;
};


#endif