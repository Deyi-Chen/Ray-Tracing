#ifndef VEC3_H
#define VEC3_H

#include <cmath>

class vec3{
    public:
        double e[3];        vec3(){
            for(int i=0;i<3;i++){
                e[i]=0.0;
            }
        }
        vec3(double x, double y, double z){
            e[0]=x;
            e[1]=y;
            e[2]=z;
        }
        vec3& operator+=(const vec3&v){
            for (int i=0;i<3;i++){
                e[i]+=v.e[i];
            }
            return *this;
        }
        vec3& operator*=(double t){
            for (int i=0;i<3;i++){
                e[i]*=t;
            }
            return *this;
        }
        vec3& operator/=(double t){
            for (int i=0;i<3;i++){
                e[i]/=t;
            }
            return *this;
        }
        double length_squared()const{
            double result=0.0;
            for (int i=0;i<3;i++){
                result+=e[i]*e[i];
            }
            return result;
        }
        double length() const{
            return sqrt(length_squared());
        }
        double x() const{
            return e[0];
        }
        double y() const{
            return e[1];
        }
        double z()const{
            return e[2];
        }
        bool near_zero()const{
            auto s=1e-8;
            return (std::fabs(e[0])<s)&&(std::fabs(e[1])<s)&&(std::fabs(e[2])<s);
        }

};

vec3 operator+(const vec3&u,const vec3&v){
    vec3 result=vec3();
    for(int i=0;i<3;i++){
        result.e[i]=u.e[i]+v.e[i]; //addition for doubles
    }
    return result; 
}

vec3 unit_vector(const vec3&v){
    vec3 result=v;
    double v_length=v.length();
    for(int i=0;i<3;i++){
        result.e[i]/=v_length;
    }
    return result;
}

double dot(const vec3&u,const vec3&v){
    double result=0.0;
    for(int i=0;i<3;i++){
        result+=u.e[i]*v.e[i];
    }
    return result;
}

vec3 operator-(const vec3&u,const vec3&v){
    vec3 result=vec3();
    for(int i=0;i<3;i++){ 
        result.e[i]=u.e[i]-v.e[i];
    }
    return result;
}

vec3 operator*(const vec3&u,double t){
    vec3 result=vec3();
    for(int i=0;i<3;i++){ 
        result.e[i]=u.e[i]*t;
    }
    return result;
}

vec3 operator*(double t,const vec3&u){ 
    vec3 result=vec3();
    for(int i=0;i<3;i++){ 
        result.e[i]=u.e[i]*t;
    }
    return result;
}

vec3 operator*(const vec3& u, const vec3& v) {
    return vec3(u.x()*v.x(), u.y()*v.y(), u.z()*v.z());
}

vec3 reflected(const vec3 normal,const vec3& v){
    return v-2*dot(v,normal)*normal;
}

#endif