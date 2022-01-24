#ifndef CAM_H
#define CAM_H

#include "utility.h"

class Camera {
    public:
        Camera(Point lookfrom, Point lookat, Vector3 vup, 
                    double vfov, double aspect_ratio){
            auto theta = deg2rad(vfov);
            auto h = tan(theta/2);
            auto viewport_height = 2.0 * h;
            auto viewport_width = aspect_ratio * viewport_height;

            auto focal_len = 1.0;

            auto w = unit_vector(lookfrom-lookat);
            auto u = unit_vector(cross(vup, w));
            auto v = cross(w,u);

            origin = lookfrom;
            horz = viewport_width*u;
            vert = viewport_height*v;
            llc = origin-(horz/2)-(vert/2)-w;
        }

        Ray get_ray(double s, double t) const {
            return Ray(origin, llc+s*horz+t*vert-origin);
        }
    private:
        Point origin, llc;
        Vector3 horz, vert;
};


#endif