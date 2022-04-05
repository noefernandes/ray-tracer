#ifndef BLINNMATERIAL_H
#define BLINNMATERIAL_H

using Color = vec3<float>;

class BlinnMaterial : public Material {
	public:

		BlinnMaterial(Color kd, Color ks, Color ka, std::string type, std::string name, 
								vec3<float> mirror, float glossiness){
			kd_c = kd;
			ks_c = ks;
			ka_c = ka;
			this->type = type;
			this->name = name;
			this->mirror = mirror;
			this->glossiness = glossiness;
		}

		~BlinnMaterial(){};

		void scatter(Surfel *isect, Ray* r){}
		Color kd(){ return kd_c; }
		Color ks(){ return ks_c; }
		Color ka(){ return ka_c; }
		Color km(){ return mirror; }

		std::string get_type(){ return type; }
		std::string get_name(){ return name; }

		std::string type;
		std::string name;
		float glossiness;
		Color kd_c;		//diffuse
		Color ks_c;		//specular
		Color ka_c;		//ambient
		vec3<float> mirror;
};

#endif