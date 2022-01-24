#ifndef PERLIN_H
#define PERLIN_H

#include "utility.h"
#include "vector3.h"

class Perlin{
    public:
        Perlin() {
            randvec = new Vector3[point_count];

            for(int i=0; i<point_count; i++)
                randvec[i] = unit_vector(random(-1,1));

            xperm = perlin_genperm();
            yperm = perlin_genperm();
            zperm = perlin_genperm();
        }

        ~Perlin(){
            delete[] randvec;
            delete[] xperm;
            delete[] yperm;
            delete[] zperm;
        }

        double noise(const Point& p) const {
            auto u = p.x() - floor(p.x());
            auto v = p.y() - floor(p.y());
            auto w = p.z() - floor(p.z());
            auto i = static_cast<int>(floor(p.x()));
            auto j = static_cast<int>(floor(p.y()));
            auto k = static_cast<int>(floor(p.z()));
            Vector3 c[2][2][2];

            for (int di=0; di < 2; di++)
                for (int dj=0; dj < 2; dj++)
                    for (int dk=0; dk < 2; dk++){
                        

                        auto xp = xperm[(i+di) & 255];
                        auto yp = yperm[(j+dj) & 255];
                        auto zp = zperm[(k+dk) & 255];
                        auto perm = xp^yp^zp;

                        c[di][dj][dk] = randvec[perm];
                    }
            
            return trilinear_interpolate(c,u,v,w);            
        }

        double turbulence(const Point& p, int depth=7) const {
            auto acc = 0.0;
            auto temp = p;
            auto weight = 1.0;

            for(int i=0; i<depth; i++){
                acc += weight*noise(temp);
                weight *= 0.5;
                temp *= 2;
            }

            return fabs(acc);
        }

    private:
        static const int point_count = 256;
        Vector3* randvec;
        int* xperm;
        int* yperm;
        int* zperm;

        static int* perlin_genperm(){
            auto p = new int[point_count];

            for(int i=0; i<point_count; i++)
                p[i] = i;

            permute(p, point_count);

            return p;
        }

        static void permute(int* p, int n){
            for(int i= n-1; i>0; i--){
                int target = random_int(0,i);
                int temp = p[i];
                p[i] = p[target];
                p[target] = temp;
            }
        }

        static double trilinear_interpolate(Vector3 c[2][2][2], double u, double v, double w){
            auto acc = 0.0;
            for(int i=0; i<2; i++)
                for(int j=0; j<2; j++)
                    for(int k=0; k<2; k++){
                        Vector3 weight_v(u-i, v-j, w-k);
                        acc += (i*u + (1-i)*(1-u))
                             * (j*v + (1-j)*(1-v))
                             * (k*w + (1-k)*(1-w))
                             * dot(c[i][j][k], weight_v);
                    }
            return acc;
        }
};


#endif